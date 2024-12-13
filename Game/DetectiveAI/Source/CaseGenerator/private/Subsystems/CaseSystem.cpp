// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CaseSystem.h"
#include "Types/CommonCaseTypes.h"
#include "Data/PromptConfigData.h"
#include "Utilities/CaseGenLogger.h"
#include "Utilities/GenAIUtilities.h"

//todo error checking.
#pragma region Initalisation
void UCaseSystem::PostInit()
{
#if UE_EDITOR
	UE_LOG(LogTemp, Display, TEXT("CaseSystem Initialised"));
#endif

	Logger = NewObject<UCaseGenLogger>();
}
#pragma endregion

#pragma region Generation
void UCaseSystem::StartCaseGeneration()
{
	if(bSkipGeneration)
		OnCaseGenerationFinished.Broadcast(FPlatformTime::Seconds() - GenStartTime);
	else
		GenerateCase();
}
void UCaseSystem::GenerateCase()
{
	OnProgressUpdated.Broadcast("Started", 0);
	GenStartTime = FPlatformTime::Seconds();	
	RandomiseInitialParams();

	TotalTasks = NumOfActors + 2;
	ProgressIncr = 1.0f / TotalTasks;
	ProgressPerc = 0.0f;
	
	FString CustomInstructions = PromptConfig->CustomInstructions;
	CustomInstructions.ReplaceInline(TEXT("{Motive}"), *UEnum::GetValueAsString(CaseFile.Motive));
	CustomInstructions.ReplaceInline(TEXT("{MurderWeapon}"), *UEnum::GetValueAsString(CaseFile.MurderWeapon));
			
	SendCustomInstructions( CustomInstructions);
	GenerateActor();
}

void UCaseSystem::FinishGeneration()
{
	//TODO add some checks for the casefile to see if its valid.

	OnProgressUpdated.Broadcast("Generation Finished", 1);
	OnCaseGenerationFinished.Broadcast(FPlatformTime::Seconds() - GenStartTime);

#if UE_EDITOR
	UE_LOG(LogTemp, Display, TEXT("Case Generation Finished In: %f"), (FPlatformTime::Seconds() - GenStartTime));
#endif
	
	OnActorsGenerated.Broadcast(CaseFile.Actors);
	OnCluesGenerated.Broadcast(CaseFile.Clues);
	//OnCaseDetailsGenerated.Broadcast(CaseFile.Motive, CaseFile.MurderWeapon, CaseFile.Context);
}


void UCaseSystem::RandomiseInitialParams()
{
	CaseFile.Motive = static_cast<EMotive>(FMath::RandRange(0, static_cast<int>(EMotive::MAX) - 1));
	CaseFile.MurderWeapon = static_cast<EMurderWeapon>(FMath::RandRange(0, static_cast<int>(EMurderWeapon::MAX) - 1));
	CaseFile.MurderRoom = static_cast<ERoom>(FMath::RandRange(0, static_cast<int>(ERoom::MAX) - 1));
	NumOfActors = FMath::RandRange(4, 8);
	NumOfClues = FMath::RandRange(6, 12);
}

void UCaseSystem::GenerateActor()
{
	if(CaseFile.Actors.Num() >= NumOfActors)
	{
		Logger->GenFinished(EGenContext::Actor);
		GenerateClues();
		return;
	}

	FString ActorPrompt;
	switch (CaseFile.Actors.Num())
	{
	case 0:
			Logger->GenStarted(EGenContext::Actor);
			ActorPrompt = PromptConfig->Victim;
			break;
		case 1:
			ActorPrompt = PromptConfig->Perpetrator;
			break;
		default:
			ActorPrompt = PromptConfig->SecondaryActor;
			break;
	}
	
	SendStructuredMessage(ActorPrompt, FActorDescription::StaticStruct());
}
void UCaseSystem::GenerateClues()
{
	Logger->GenStarted(EGenContext::Clue);
	FString CluePrompt = PromptConfig->Clues;
	CluePrompt.ReplaceInline(TEXT("{NumOfClues}"), *FString::FromInt(NumOfClues));
	SendStructuredMessage(CluePrompt, FClueCollection::StaticStruct());
}

void UCaseSystem::GenerateConnections()
{
	Logger->GenStarted(EGenContext::Context);
	FString ConnectionsPrompt = PromptConfig->Connections;
	SendStructuredMessage(ConnectionsPrompt, FContextCollection::StaticStruct());
}

void UCaseSystem::GeneratePoliceContext()
{
	SendMessage("Generate the context knowledge base for the first responder,"
			 " They should not know who the suspect, weapon or motive is and should only have general information about the house, victim and actors.");
}
#pragma endregion 


#pragma region Callbacks
void UCaseSystem::MessageReceived(FString& Message)
{
	CaseFile.Context = Message;
	OnCaseDetailsGenerated.Broadcast(CaseFile.Motive, CaseFile.MurderWeapon, Message);
	Logger->WriteCaseFile(CaseFile);
}

void UCaseSystem::StructuredMessageReceived(FString& Message, UScriptStruct* Struct)
{
	if(Struct == FActorDescription::StaticStruct())
	{
		FActorDescription ActorDescription;
		if ( UGenAIUtilities::JsonToUStruct(Message, Struct, &ActorDescription))
		{
			ProgressPerc += ProgressIncr;
			OnProgressUpdated.Broadcast(FString::Printf(TEXT("Actor %d Generated"), CaseFile.Actors.Num()), ProgressPerc);

			
			CaseFile.Actors.Add(ActorDescription);
			GeneratedActorSchemas.Add(Message);
			GenerateActor();
		}
		else
		{
			Logger->GenFailed();
			UE_LOG(LogTemp, Warning, TEXT("JSON CONVERSION FAILED FActorDescription"));
			RetryLastMessage();
		}
	}
	else if(Struct == FClueCollection::StaticStruct())
	{
		FClueCollection ClueCollection;
		if (UGenAIUtilities::JsonToUStruct(Message, Struct, &ClueCollection))
		{
			ProgressPerc += ProgressIncr;
			OnProgressUpdated.Broadcast(FString::Printf(TEXT("Clues Generated")), ProgressPerc);

			CaseFile.Clues = ClueCollection.Clues;
			
			Logger->GenFinished(EGenContext::Clue);
			GenerateConnections();
		}
		else
		{
			Logger->GenFailed();
			UE_LOG(LogTemp, Warning, TEXT("JSON CONVERSION FAILED FClueCollection"));
			RetryLastMessage();
		}
	}
	else if(Struct == FContextCollection::StaticStruct())
	{
		FContextCollection ContextCollection;
		if (UGenAIUtilities::JsonToUStruct(Message, Struct, &ContextCollection))
		{
			ProgressPerc += ProgressIncr;
			OnProgressUpdated.Broadcast(FString::Printf(TEXT("Connections Generated")), ProgressPerc);

			for(int i = 0; i < ContextCollection.KnowledgeBases.Num(); i++)
			{
				if(i < CaseFile.Actors.Num())
				{
					CaseFile.Actors[i].Context += "\n[UPDATED]\n" + ContextCollection.KnowledgeBases[i].KnowledgeBase;
				}	
			}
			Logger->GenFinished(EGenContext::Context);
			GeneratePoliceContext();
			FinishGeneration();
		}
		else
		{
			Logger->GenFailed();
			UE_LOG(LogTemp, Warning, TEXT("JSON CONVERSION FAILED FContextCollection"));
			RetryLastMessage();
		}

	}
	else
	{
#if UE_EDITOR
		UE_LOG(LogTemp, Display, TEXT("CaseSystem: Schema not handled %s"), *Struct->GetName());
#endif
	}
}



// void UCaseSystem::FunctionCallReceived(UObject* Caller, FString& Message, FName& FunctionName, TArray<FString>& ArgNames, TArray<FString>& ArgValues)
// {
//
// 	
// }
#pragma endregion 