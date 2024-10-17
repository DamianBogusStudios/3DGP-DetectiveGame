// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Subsystems/DialogueSystem.h"
#include "JsonObjectConverter.h"
#include "ChatGPT/ChatGPT.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/FileSystemFuncLib.h"
#include "Types/CommonCaseTypes.h"


void UDialogueSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    InitChatGPT();

    
    FString Message = FString::Printf(TEXT("DialogueSystem Initialised"));
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
}

void UDialogueSystem::Deinitialize()
{
    Super::Deinitialize();
}


void UDialogueSystem::InitChatGPT()
{
    const auto Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append("OpenAIAuth.ini"));
    const auto Model = UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4O_Mini);
	
    ChatGPT = NewObject<UChatGPT>();
    check(ChatGPT);
    ChatGPT->SetAuth(Auth);
    ChatGPT->SetMaxTokens(MaxTokens);
    ChatGPT->SetModel(Model);
    
    ChatGPT->OnRequestCompleted().AddUObject(this, &ThisClass::OnRequestCompleted);
    ChatGPT->OnRequestUpdated().AddUObject(this, &ThisClass::OnRequestUpdated);
}

void UDialogueSystem::SendMessage(const FText& Text)
{
    FMessage Message;
    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::User);
    Message.Content = Text.ToString();

    ChatGPT->AddMessage(Message);

    /* double check if this is needed */
    FMessage AssistantMessage;
    AssistantMessage.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant);
    ChatGPT->SetAssistantMessage(AssistantMessage);
    /*********************************/

    ChatGPT->MakeRequest();
}
void UDialogueSystem::RequestDialogueOptions(const FString& ActorDescription)
{
    FDialogueOptions DialogueOptions;
    FString OutputString;

    if(FJsonObjectConverter::UStructToJsonObjectString(DialogueOptions, OutputString))
    {
        FChatCompletionResponseFormat ResponseFormat;
        ResponseFormat.Type = "json_schema";//ActorDescription;
               
        FMessage Message;
        Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::User);
        Message.Content = FString("Based on the current game state create dialogue options for interacting with this NPC: ")
        + ActorDescription;
        
        ChatGPT->AddMessage(Message);
        ChatGPT->MakeStructuredRequest(ResponseFormat);
    }
}
void UDialogueSystem::OnRequestCompleted()
{

}

void UDialogueSystem::OnRequestUpdated(const FMessage& Message, bool WasError)
{
    if(WasError)
    {
        UE_LOG(LogTemp, Error, TEXT("DialogueSystem Error: %s"), *Message.Content);
        return;
    }
    
    UE_LOG(LogTemp, Display, TEXT("DialogueSystem: %s"), *Message.Content);

    //TODO
    //OnDialogueOptionsReceived.Broadcast();
    //OnMessageReceived.Broadcast(LastActorDescription, "Placeholder Message");
}


#pragma region IDialogueProvider Interface

void UDialogueSystem::RequestDialogueOptions(UObject* Caller, FActorDescription& ActorDescription)
{
    FString OutputString;
    if(FJsonObjectConverter::UStructToJsonObjectString(ActorDescription, OutputString))
    {
        DialogueRequestCaller = Caller;
        RequestDialogueOptions(OutputString);
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