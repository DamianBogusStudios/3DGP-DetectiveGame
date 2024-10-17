// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/DialogueSystem.h"

#include "JsonObjectConverter.h"
#include "ChatGPT/ChatGPT.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/FileSystemFuncLib.h"
#include "Types/CommonTypes.h"

void UDialogueSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    InitChatGPT();
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

    FJsonObjectConverter::UStructToJsonObjectString(FDialogueOptions::StaticStruct(), DialogueOptions, OutputString);

    
        // TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
        // FJsonSerializer::Serialize(DialogueOptions, JsonWriter);
    
    FChatCompletionResponseFormat ResponseFormat;
    ResponseFormat.Type = "";
    // UserMessage.Content = FString::Printf(TEXT(
    //    "Based on the situation where %s, please provide three dialogue options in the following format:\n"
    //    "{ \"options\": [ { \"id\": \"1\", \"text\": \"Option text 1\" }, { \"id\": \"2\", \"text\": \"Option text 2\" }, { \"id\": \"3\", \"text\": \"Option text 3\" } ] }"),
    //    *NPCDescription);
}

void UDialogueSystem::OnRequestCompleted()
{
}

void UDialogueSystem::OnRequestUpdated(const FMessage& Message, bool WasError)
{
    //return msg.
}

