// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Subsystems/DialogueSystem.h"
#include "JsonObjectConverter.h"
#include "Types/CommonCaseTypes.h"
#include "Handlers/LLMServiceLocator.h"
#include "Interfaces/LLMService.h"

#pragma region Setup
void UDialogueSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    LLMService = ULLMServiceLocator::GetService();
    
#if UE_EDITOR
    FString Message = FString::Printf(TEXT("DialogueSystem Initialised"));
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif
    
}

void UDialogueSystem::Deinitialize()
{
    Super::Deinitialize();
}
#pragma endregion

#pragma region IDialogueProvider Interface

void UDialogueSystem::RequestDialogueOptions(UObject* Caller, FActorDescription& ActorDescription)
{
    if(LLMService && LLMService.GetObject())
    {
        FString OutputString;
        if(FJsonObjectConverter::UStructToJsonObjectString(ActorDescription, OutputString))
        {
            DialogueRequestCaller = Caller;

            FString Message = "Generate dialogue options for the player to choose when interacting with this NPC using the structured output json format." + OutputString;
            LLMService->SendStructuredMessage(Caller, Message, FDialogueOptions::StaticStruct());
        }
    }
}

FMessageDelegate& UDialogueSystem::GetResponseDelegate()
{
    return OnMessageReceived;
}
FDialogueOptionsDelegate& UDialogueSystem::GetDialogueOptionsDelegate()
{
    return OnDialogueOptionsReceived;
}

#pragma endregion

#pragma region Callbacks

#pragma endregion

#pragma region Conversion
bool UDialogueSystem::ParseFromJsonToStruct(const FString& Content, UScriptStruct* Schema, TSharedPtr<void> StructInstance)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // Allocate memory for the struct using TSharedPtr to manage the memory
        StructInstance = TSharedPtr<void>(FMemory::Malloc(Schema->GetStructureSize()), FMemory::Free);
        Schema->InitializeStruct(StructInstance.Get());

        if(FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), Schema, &StructInstance, 0, 0))
        {
            return true;
        }
        StructInstance.Reset();
		
    }

    return false;
}
#pragma endregion