// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DeveloperSettings.h"
#include "InteractionSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Interaction Settings"))
class DETECTIVEAI_API UInteractionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
		AdvancedDisplay, meta = (AllowedClasses = "DialogueWidget"))
	TSubclassOf<UUserWidget> DialogueWidgetClass;
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "UI", 
		AdvancedDisplay)
	TSubclassOf<UUserWidget> LockpickMiniGameClass;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "UI", 
		AdvancedDisplay)
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	
public:

	
	// /*obselete*/
	// UFUNCTION(BlueprintCallable, Category = "UI")
	// TSubclassOf<UDialogueWidget> GetDialogueWidgetClass() const { return DialogueWidgetClass; }
	//
	// /*obselete*/
	// UFUNCTION(BlueprintCallable, Category = "UI")
	// TSubclassOf<ULockpickMiniGame> GetLockpickMiniGameClass() const { return LockpickMiniGameClass; }
};
