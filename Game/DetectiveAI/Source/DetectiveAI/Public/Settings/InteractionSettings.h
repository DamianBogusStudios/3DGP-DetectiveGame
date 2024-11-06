// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DeveloperSettings.h"
#include "InteractionSettings.generated.h"

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	NPCDialogue,
	LockpickMiniGame,
	PauseMenu,
	HUD
};

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Interaction Settings"))
class DETECTIVEAI_API UInteractionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI",
		AdvancedDisplay, meta = (AllowedClasses = "DialogueWidget"))
	TSubclassOf<UUserWidget> DialogueWidgetClass;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI", 
		AdvancedDisplay)
	TSubclassOf<UUserWidget> LockpickMiniGameClass;
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	TSubclassOf<UUserWidget> GetWidgetClass(EWidgetType WidgetType) const
	{
		switch (WidgetType)
		{
		case EWidgetType::NPCDialogue:
			return DialogueWidgetClass;
		case EWidgetType::LockpickMiniGame:
			return LockpickMiniGameClass;
		default:
			return nullptr;
		}		
	}
	
	// /*obselete*/
	// UFUNCTION(BlueprintCallable, Category = "UI")
	// TSubclassOf<UDialogueWidget> GetDialogueWidgetClass() const { return DialogueWidgetClass; }
	//
	// /*obselete*/
	// UFUNCTION(BlueprintCallable, Category = "UI")
	// TSubclassOf<ULockpickMiniGame> GetLockpickMiniGameClass() const { return LockpickMiniGameClass; }
};
