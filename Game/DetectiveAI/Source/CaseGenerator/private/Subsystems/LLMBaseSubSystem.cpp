﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/LLMBaseSubSystem.h"
#include "Handlers/ServiceLocator.h"
#include "Interfaces/LLMService.h"
#include "Settings/LLMSettings.h"

#pragma region Initalisation
void ULLMBaseSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LLMService = UServiceLocator::GetService_LLM();

	GetConfigFiles();
	PostInit();
}

#pragma region Virtual Function Defaults
void ULLMBaseSubSystem::PostInit()
{
#if UE_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("ULLMBaseSubSystem:PostInit Called. Function should be overriden by derived class"));
#endif
}	
void ULLMBaseSubSystem::MessageReceived(FString& Message)
{
#if UE_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("ULLMBaseSubSystem:MessageReceived Called. Function should be overriden by derived class"));
#endif
}
void ULLMBaseSubSystem::StructuredMessageReceived(FString& Message, UScriptStruct* Schema)
{
#if UE_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("ULLMBaseSubSystem:StructuredMessageReceived Called. Function should be overriden by derived class"));
#endif
}
void ULLMBaseSubSystem::ErrorReceived(FString& Message, UScriptStruct* Schema)
{
#if UE_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("ULLMBaseSubSystem:ErrorReceived Called. Function should be overriden by derived class"));
#endif
}
#pragma endregion

void ULLMBaseSubSystem::Deinitialize()
{
	Super::Deinitialize();

	UServiceLocator::Cleanup();
}

void ULLMBaseSubSystem::GetConfigFiles()
{
	if(const ULLMSettings* Settings = GetDefault<ULLMSettings>())
	{
		PromptConfig = Settings->GetPromptConfigData();
	}
}

// void ULLMBaseSubSystem::BindCallbacks()
// {
// 	MessageDelegate.BindUObject(this, &ULLMBaseSubSystem::MessagedReceivedDelegate);
// 	StructuredMessageDelegate.BindUObject(this, &ULLMBaseSubSystem::StructuredMessageReceivedDelegate);
// }
#pragma endregion

#pragma region Requests

void ULLMBaseSubSystem::SendCustomInstructions(const FString& Prompt)
{
	if(LLMService)
	{
		LLMService->SendCustomInstructions(this, Prompt);
	}
}

void ULLMBaseSubSystem::SendMessage(const FString& Prompt)
{
	if(LLMService)
	{
		LLMService->SendMessage(this, Prompt,
			FMessageDelegate::CreateUObject(this, &ULLMBaseSubSystem::OnMessagedReceivedInternal),
			FErrorReceivedDelegate::CreateUObject(this, &ULLMBaseSubSystem::OnErrorReceivedInternal)
		);
	}
}

// void ULLMBaseSubSystem::SendMessage(const FString& Prompt)
// {
// 	if(LLMService)
// 	{
// 		LLMService->SendMessage(this, Prompt, MessageDelegate);
// 	}
// }

void ULLMBaseSubSystem::SendStructuredMessage(const FString& Prompt, UScriptStruct* Schema)
{
	if(LLMService)
	{
		LLMService->SendStructuredMessage(this, Prompt, Schema, 
		FStructuredMessageDelegate::CreateUObject(this, &ULLMBaseSubSystem::OnStructuredMessageReceivedInternal),
			FErrorReceivedDelegate::CreateUObject(this, &ULLMBaseSubSystem::OnErrorReceivedInternal)
		);
	}
}



#pragma endregion 

#pragma region Callbacks
void ULLMBaseSubSystem::OnMessagedReceivedInternal(FString& Message)
{
	this->MessageReceived(Message);
}

void ULLMBaseSubSystem::OnStructuredMessageReceivedInternal(FString& Message, UScriptStruct* Struct)
{
	this->StructuredMessageReceived(Message, Struct);
}

void ULLMBaseSubSystem::OnErrorReceivedInternal(FString& Message, UScriptStruct* Struct)
{
	this->ErrorReceived(Message, Struct);
}

// void ULLMBaseSubSystem::FunctionCallReceived(UObject* Caller, FString& Message, FName& FunctionName, TArray<FString>& ArgNames, TArray<FString>& ArgValues)
// {
//
// 	
// }
#pragma endregion 