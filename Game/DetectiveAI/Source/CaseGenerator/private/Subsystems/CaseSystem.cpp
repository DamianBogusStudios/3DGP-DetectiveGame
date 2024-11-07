// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CaseSystem.h"
#include "Handlers/LLMServiceLocator.h"
#include "Interfaces/LLMService.h"
#include "Settings/LLMSettings.h"
#include "Types/CommonCaseTypes.h"
#include "Utilities/GenAIUtilities.h"


void UCaseSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LLMService = ULLMServiceLocator::GetService();
	BindServiceCallbacks();


	GenerateCase();
	
#if UE_EDITOR
	FString Message = FString::Printf(TEXT("CaseSystem Initialised"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif
}

void UCaseSystem::Deinitialize()
{
	Super::Deinitialize();

	ULLMServiceLocator::Cleanup();
}

void UCaseSystem::BindServiceCallbacks()
{
	if(LLMService && LLMService.GetObject())
	{
		LLMService->GetMessageDelegate().AddDynamic(this, &UCaseSystem::MessagedReceived);
		LLMService->GetStructuredMessageDelegate().AddDynamic(this, &UCaseSystem::StructuredMessageReceived);
		LLMService->GetFunctionCalledDelegate().AddDynamic(this, &UCaseSystem::FunctionCallReceived);
	}
}

void UCaseSystem::GenerateCase()
{
	if(LLMService && LLMService.GetObject())
	{
		if(const ULLMSettings* Settings = GetDefault<ULLMSettings>())
		{
			RandomiseInitialParams();
			
			FString CustomInstructions = Settings->CustomInstructions;
			LLMService->SendCustomInstructions(this, CustomInstructions);

			FString CaseDetails =  FString::Printf(TEXT(" Motive: %s, Murder Weapon: %s"),
			*UEnum::GetValueAsString(CaseFile.Motive),*UEnum::GetValueAsString(CaseFile.MurderWeapon));
			LLMService->SendCustomInstructions(this, CaseDetails);
			GenerateActor();
		}
	}
}

void UCaseSystem::GenerateActor()
{
	if(CaseFile.Actors.Num() >= NumberOfActors)
	{
		GenerateClues();
		return;
	}
	FString Prompt = "";
	
	if(CaseFile.Actors.Num() == 0)
	{
		Prompt = "Based on the provided murder weapon and motive generate the victim of the murder with the provided schema.\n";
	}
	else if(CaseFile.Actors.Num() == 1)
	{
		Prompt = "Using the murder weapon, motive and victim generated, generate the perpetrator with the provided schema.";
	}
	else if(CaseFile.Actors.Num() < NumberOfActors)
	{
		Prompt = "Using the details of the case and previous actors, generate a random secondary actor, using the provided schema.";
	}
	
	LLMService->SendStructuredMessage(this, Prompt, FActorDescription::StaticStruct());
}

void UCaseSystem::GenerateConnections()
{
	//FString Prompt = "";
    
}
void UCaseSystem::GenerateClues()
{
	int32 NumClues = FMath::RandRange(6, 12);

	FString Prompt = FString::Printf(TEXT("Generate a collection of %d clues related to the case with the provided schema"), NumClues);
	
	LLMService->SendStructuredMessage(this, Prompt, FClueCollection::StaticStruct());
}

void UCaseSystem::RandomiseInitialParams()
{
	CaseFile.Motive = static_cast<EMotive>(FMath::RandRange(0, static_cast<int>(EMotive::MAX) - 1));
	CaseFile.MurderWeapon = static_cast<EMurderWeapon>(FMath::RandRange(0, static_cast<int>(EMurderWeapon::MAX) - 1));
	NumberOfActors = FMath::RandRange(4, 8);
}

void UCaseSystem::MessagedReceived(UObject* Caller, FString& Message)
{

}

void UCaseSystem::StructuredMessageReceived(UObject* Caller, FString& Message, UScriptStruct* Struct)
{
	if(Caller != this)
		return;
	
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
	if(Struct == FClueCollection::StaticStruct())
	{
		FClueCollection ClueCollection;
		if (UGenAIUtilities::JsonToUStruct(Message, Struct, &ClueCollection))
		{
			CaseFile.Clues = ClueCollection.Clues;
		}
	}
}

void UCaseSystem::FunctionCallReceived(UObject* Caller, FString& Message, FName& FunctionName, TArray<FString>& ArgNames, TArray<FString>& ArgValues)
{

	
}




/* tried generating initial settings but it kept giving me the same values. 


			
CustomInstructions +=  "\nBased on the provided murder weapon and motive generate the victim of the murder with the provided schema.";
			CustomInstructions += "\nMotive: " + UEnum::GetValueAsString(CaseFile.Motive);
			CustomInstructions += "\nMurderWeapon: " + UEnum::GetValueAsString(CaseFile.MurderWeapon);

			LLMService->SendStructuredMessage(this, CustomInstructions, FActorDescription::StaticStruct());
*if(const ULLMSettings* Settings = GetDefault<ULLMSettings>())
		{
			FString CustomInstructions = Settings->CustomInstructions;

			CustomInstructions += "\nGenerate the motive and murder weapon for the crime";

			FName FuncName = FName("GenerateCrimeDetails");
			LLMService->AddFunction(FuncName, "Sets preliminary details about case including motive and weapon");

			auto ParamOne = StaticEnum<EMotive>();
			auto ParamTwo = StaticEnum<EMurderWeapon>();

			LLMService->AddFunctionParam(FuncName,
				"Motive", "Motive for the murder", ParamOne->GetName(), ParamOne);
			
			LLMService->AddFunctionParam(FuncName,
				"MurderWeapon", "Weapon used to act out murder", ParamTwo->GetName(), ParamTwo);

			LLMService->SendMessage(this,CustomInstructions);
		}
 * 
 */