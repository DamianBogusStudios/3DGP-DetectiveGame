// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CaseSystem.h"
#include "Handlers/LLMServiceLocator.h"
#include "Interfaces/LLMService.h"
#include "Settings/LLMSettings.h"
#include "Types/CommonCaseTypes.h"
#include "Data/PromptConfigData.h"
#include "Utilities/GenAIUtilities.h"

#pragma region Initalisation
void UCaseSystem::PostInit()
{
	// Super::Initialize(Collection);

	// LLMService = ULLMServiceLocator::GetService();

	// GetConfigFiles();
	// BindCallbacks();
	GenerateCase();
	
#if UE_EDITOR
	FString Message = FString::Printf(TEXT("CaseSystem Initialised"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
	UE_LOG(LogTemp, Display, TEXT("CaseSystem Initialised"));
#endif
}

// void UCaseSystem::Deinitialize()
// {
// 	Super::Deinitialize();
//
// 	ULLMServiceLocator::Cleanup();
// }
//
// void UCaseSystem::GetConfigFiles()
// {
// 	if(const ULLMSettings* Settings = GetDefault<ULLMSettings>())
// 	{
// 		PromptConfig = Settings->GetPromptConfigData();
// 	}
// }

// void UCaseSystem::BindCallbacks()
// {
// 	MessageDelegate.BindUObject(this, &UCaseSystem::MessagedReceived);
// 	StructuredMessageDelegate.BindUObject(this, &UCaseSystem::StructuredMessageReceived);
// }
#pragma endregion

#pragma region Generation
void UCaseSystem::GenerateCase()
{
	if(LLMService && LLMService.GetObject())
	{ 
		RandomiseInitialParams();
			
		FString CustomInstructions = PromptConfig->CustomInstructions;
		CustomInstructions.ReplaceInline(TEXT("{Motive}"), *UEnum::GetValueAsString(CaseFile.Motive));
		CustomInstructions.ReplaceInline(TEXT("{MurderWeapon}"), *UEnum::GetValueAsString(CaseFile.MurderWeapon));
			
		SendCustomInstructions( CustomInstructions);
		GenerateActor();
	}
}

void UCaseSystem::RandomiseInitialParams()
{
	CaseFile.Motive = static_cast<EMotive>(FMath::RandRange(0, static_cast<int>(EMotive::MAX) - 1));
	CaseFile.MurderWeapon = static_cast<EMurderWeapon>(FMath::RandRange(0, static_cast<int>(EMurderWeapon::MAX) - 1));
	NumOfActors = FMath::RandRange(4, 8);
	NumOfClues = FMath::RandRange(6, 12);
}

void UCaseSystem::GenerateActor()
{
	if(CaseFile.Actors.Num() >= NumOfActors)
	{
		GenerateClues();
		return;
	}

	FString ActorPrompt;
	switch (CaseFile.Actors.Num())
	{
		case 0:
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
	FString CluePrompt = PromptConfig->Clues;
	CluePrompt.ReplaceInline(TEXT("{NumOfClues}"), *FString::FromInt(NumOfClues));	
	SendStructuredMessage(CluePrompt, FClueCollection::StaticStruct());
}

void UCaseSystem::GenerateConnections()
{
	FString ConnectionsPrompt = PromptConfig->Connections;
	SendStructuredMessage(ConnectionsPrompt, FContextCollection::StaticStruct());
}
#pragma endregion 

#pragma region Requests

// void UCaseSystem::SendCustomInstructions(const FString& Prompt)
// {
// 	if(LLMService)
// 	{
// 		LLMService->SendCustomInstructions(this, Prompt);
// 	}
// }
//
// void UCaseSystem::SendMessage(const FString& Prompt)
// {
// 	if(LLMService)
// 	{
// 		LLMService->SendMessage(this, Prompt, MessageDelegate);
// 	}
// }
//
// void UCaseSystem::SendStructuredMessage(const FString& Prompt, UScriptStruct* Schema)
// {
// 	if(LLMService)
// 	{
// 		LLMService->SendStructuredMessage(this, Prompt, Schema, StructuredMessageDelegate);
// 	}
// }
//
// #pragma endregion 

#pragma region Callbacks
void UCaseSystem::MessageReceived(FString& Message)
{

}

void UCaseSystem::StructuredMessageReceived(FString& Message, UScriptStruct* Struct)
{
	if(Struct == FActorDescription::StaticStruct())
	{
		FActorDescription ActorDescription;
		if ( UGenAIUtilities::JsonToUStruct(Message, Struct, &ActorDescription))
		{
			CaseFile.Actors.Add(ActorDescription);
			GeneratedActorSchemas.Add(Message);
			GenerateActor();
		}
	}
	else if(Struct == FClueCollection::StaticStruct())
	{
		FClueCollection ClueCollection;
		if (UGenAIUtilities::JsonToUStruct(Message, Struct, &ClueCollection))
		{
			CaseFile.Clues = ClueCollection.Clues;
			GenerateConnections();
		}
	}
	else if(Struct == FContextCollection::StaticStruct())
	{
		FContextCollection ContextCollection;
		if (UGenAIUtilities::JsonToUStruct(Message, Struct, &ContextCollection))
		{
			for(int i = 0; i < ContextCollection.KnowledgeBases.Num(); i++)
			{
				if(i < CaseFile.Actors.Num())
				{
					CaseFile.Actors[i].Context = ContextCollection.KnowledgeBases[i].KnowledgeBase;
				}	
			}
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