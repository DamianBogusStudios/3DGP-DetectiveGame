// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LLMService.generated.h"

class FStructuredMessageDelegate;
class FMessageDelegate;
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
	virtual void SendMessage(UObject* Caller, FString& Message) = 0;
	virtual void SendStructuredMessage(UObject* const WorldObject, const FString& Message, UScriptStruct* Struct) = 0;

	/* delegates */
	virtual FMessageDelegate& GetMessageDelegate() = 0;
	virtual FStructuredMessageDelegate& GetStructuredMessageDelegate() = 0;
};
