// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogueHandler.generated.h"

class UUserWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDialogueHandler : public UInterface
{
	GENERATED_BODY()
};
/**
 * 
 */
class DETECTIVEAI_API IDialogueHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void StartDialogue(AActor* Caller, TSubclassOf<UUserWidget> WidgetClass);
};
