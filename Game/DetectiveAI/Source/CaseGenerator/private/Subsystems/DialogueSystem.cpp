// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Subsystems/DialogueSystem.h"

#include "InterchangeResult.h"
#include "Types/CommonCaseTypes.h"
#include "Handlers/LLMServiceLocator.h"
#include "Interfaces/LLMService.h"

#pragma region Setup
void UDialogueSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UDialogueSystem::Deinitialize()
{
    Super::Deinitialize();
}

void UDialogueSystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);

    LLMService = ULLMServiceLocator::GetService();
    BindServiceCallbacks();
    
#if UE_EDITOR
    FString Message = FString::Printf(TEXT("DialogueSystem Initialised"));
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif
    
}
#pragma endregion

#pragma region IDialogueProvider Interface

void UDialogueSystem::RequestSendMessage(UObject* Caller, FString& Message)
{
}

void UDialogueSystem::RequestDialogueOptions(UObject* Caller, FActorDescription& ActorDescription, FDialogueOptionsDelegate& Delegate)
{
    if(LLMService && LLMService.GetObject())
    {
        FString OutputString;
        if(FJsonObjectConverter::UStructToJsonObjectString(ActorDescription, OutputString))
        {
            OnDialogueOptionsReceived = Delegate;
            
            FString Message = "Generate dialogue options for the player to choose when interacting with this NPC using the structured output json format." + OutputString;
            LLMService->SendStructuredMessage(Caller, Message, FDialogueOptions::StaticStruct());
        }
    }
}
#pragma endregion

#pragma region Callbacks
void UDialogueSystem::BindServiceCallbacks()
{
    if(LLMService && LLMService.GetObject())
    {
        LLMService->GetMessageDelegate().AddDynamic(this, &UDialogueSystem::MessagedReceived);
        LLMService->GetStructuredMessageDelegate().AddDynamic(this, &UDialogueSystem::StructuredMessageReceived);
    }
}

void UDialogueSystem::MessagedReceived(UObject* Caller, FString& Message)
{
    
}
void UDialogueSystem::StructuredMessageReceived(UObject* Caller, FString& Message, UScriptStruct* Struct)
{
    if (Struct == FDialogueOptions::StaticStruct())
    {
        FDialogueOptions DialogueOptions;
        if (ParseFromJsonToStruct(Message, Struct, DialogueOptions))
        {
            OnDialogueOptionsReceived.ExecuteIfBound(DialogueOptions);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Unable to Parse Dialogue Options"));    
        }
    }
}
#pragma endregion

// #pragma region Conversion
// bool UDialogueSystem::ParseFromJsonToStruct(const FString& Content, UScriptStruct* Schema, StructType& StructInstance)
// {
//     TSharedPtr<FJsonObject> JsonObject;
//     TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
//
//     if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
//     {
//
//        // StructInstance = TSharedPtr<void>(FMemory::Malloc(Schema->GetStructureSize()), FMemory::Free);
//         //Schema->InitializeStruct(StructInstance.Get());
//
//         if(FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), Schema, &StructInstance, 0, 0))
//         {
//             return true;
//         }
//         //StructInstance.Reset();
// 		
//     }
//
//     return false;
// }
// #pragma endregion