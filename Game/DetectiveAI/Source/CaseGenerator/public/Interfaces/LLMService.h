// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonCaseTypes.h"
#include "UObject/Interface.h"
#include "LLMService.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULLMService : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CASEGENERATOR_API ILLMService
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* requests */
	virtual void SendCustomInstructions(UObject* const Caller,const FString& Message) = 0;

	virtual void SendMessage( UObject* const Caller, const FString& Message,
				FMessageDelegate Callback,
				FErrorReceivedDelegate ErrorCallback) = 0;

	virtual void SendStructuredMessage(UObject* const Caller, const FString& Message, UScriptStruct* StructSchema,
				FStructuredMessageDelegate Callback,
				FErrorReceivedDelegate ErrorCallback) = 0;


	virtual void AddFunction(const FName& InName, const FString& InDescription) = 0;
	virtual void AddFunctionParam(const FName& FuncName, const FName& InName, const FString& InDescription, const FString& InType, const UEnum* InEnum) = 0;
	
};
