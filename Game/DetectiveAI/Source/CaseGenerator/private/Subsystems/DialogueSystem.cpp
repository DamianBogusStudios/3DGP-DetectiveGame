// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Subsystems/DialogueSystem.h"
#include "Utilities/GenAIUtilities.h"
#include "Types/CommonCaseTypes.h"
#include "Handlers/LLMServiceLocator.h"
#include "Interfaces/LLMService.h"

#pragma region Setup
void UDialogueSystem::PostInit()
{
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
        if(UGenAIUtilities::UStructToJsonObjectString(FActorDescription::StaticStruct(), &ActorDescription, OutputString))
        {
            OnDialogueOptionsReceived = Delegate;
            
            FString Message = "Generate dialogue options for the player to choose when interacting with this NPC using the structured output json format." + OutputString;
            SendStructuredMessage(Message, FDialogueOptions::StaticStruct());
            //LLMService->SendStructuredMessage(Caller, Message, FDialogueOptions::StaticStruct());
        }
    }
}
#pragma endregion

#pragma region Callbacks
void UDialogueSystem::MessageReceived(FString& Message)
{
    
}
void UDialogueSystem::StructuredMessageReceived(FString& Message, UScriptStruct* Struct)
{
    if (Struct == FDialogueOptions::StaticStruct())
    {
        FDialogueOptions DialogueOptions;
        if ( UGenAIUtilities::JsonToUStruct(Message, Struct, &DialogueOptions))
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