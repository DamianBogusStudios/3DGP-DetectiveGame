// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonCaseTypes.h"
#include "UObject/Interface.h"
#include "DialogueProvider.generated.h"

struct FActorDescription;
class FMessageDelegate;
class FDialogueOptionsDelegate;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDialogueProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CASEGENERATOR_API IDialogueProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void RequestDialogueOptions(UObject* Caller, FActorDescription& ActorDescription, FDialogueOptionsDelegate& Callback) = 0;

	virtual void RequestSendMessage(UObject* Caller, FString& Message) = 0;
};
