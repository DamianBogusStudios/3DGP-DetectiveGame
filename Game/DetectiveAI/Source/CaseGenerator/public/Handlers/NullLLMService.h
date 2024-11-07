// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/LLMService.h"
#include "UObject/Object.h"
#include "Types/CommonCaseTypes.h"
#include "NullLLMService.generated.h"

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UNullLLMService : public UObject, public ILLMService
{
	GENERATED_BODY()

public:
	virtual void SendCustomInstructions(UObject* const Caller, const FString& Message) override;
	virtual void SendMessage(UObject* const Caller, const FString& Message, FMessageDelegate& Delegate) override;
	virtual void SendStructuredMessage(UObject* const WorldObject, const FString& Message,
		UScriptStruct* Struct, FStructuredMessageDelegate& Delegate) override;

	
	virtual void AddFunction(const FName& InName, const FString& InDescription) override;
	virtual void AddFunctionParam(const FName& FuncName, const FName& InName, const FString& InDescription, const FString& InType, const UEnum* InEnum) override;
	
	
	// virtual FMessageDelegate& GetMessageDelegate() override;
	// virtual FStructuredMessageDelegate& GetStructuredMessageDelegate() override;
	// virtual FFunctionCallDelegate& GetFunctionCalledDelegate() override;

private:

	// FMessageDelegate MessageDelegate;
	// FStructuredMessageDelegate StructuredMessageDelegate;
	// FFunctionCallDelegate FunctionCalledDelegate;
};
