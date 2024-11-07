// Fill out your copyright notice in the Description page of Project Settings.


#include "Handlers/NullLLMService.h"

// void UNullLLMService::SendCustomInstructions(UObject* Caller, FString& Message)
// {
// }
//
// void UNullLLMService::SendMessage(UObject* Caller, FString& Message)
// {
// 	//null
// }
//
// void UNullLLMService::SendStructuredMessage(UObject* const WorldObject, const FString& Message, UScriptStruct* Struct)
// {
// 	//null;
// }

void UNullLLMService::SendCustomInstructions(UObject* const Caller, const FString& Message)
{
}

void UNullLLMService::SendMessage(UObject* const Caller, const FString& Message, FMessageDelegate& Delegate)
{
}

void UNullLLMService::SendStructuredMessage(UObject* const WorldObject, const FString& Message, UScriptStruct* Struct,
	FStructuredMessageDelegate& Delegate)
{
}

void UNullLLMService::AddFunction(const FName& InName, const FString& InDescription)
{
}

void UNullLLMService::AddFunctionParam(const FName& FuncName, const FName& InName, const FString& InDescription,
	const FString& InType, const UEnum* InEnum)
{
}

// FMessageDelegate& UNullLLMService::GetMessageDelegate()
// {
// 	return MessageDelegate;
// }
//
// FStructuredMessageDelegate& UNullLLMService::GetStructuredMessageDelegate()
// {
// 	return StructuredMessageDelegate;
// }
//
// FFunctionCallDelegate& UNullLLMService::GetFunctionCalledDelegate()
// {
// 	return FunctionCalledDelegate;
// }
