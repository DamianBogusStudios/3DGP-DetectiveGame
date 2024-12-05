// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DeveloperSettings.h"
#include "WidgetClassSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Widget Class Settings"))
class DETECTIVEAI_API UWidgetClassSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
		AdvancedDisplay, meta = (AllowedClasses = "DialogueWidget"))
	TSubclassOf<UUserWidget> DialogueBox;
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "UI", 
		AdvancedDisplay)
	TSubclassOf<UUserWidget> LockpickMiniGame;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "UI", 
		AdvancedDisplay)
	TSubclassOf<UUserWidget> LoadingGame;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "UI", 
		AdvancedDisplay)
	TSubclassOf<UUserWidget> MicFeedback;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "UI", 
		AdvancedDisplay)
	TSubclassOf<UUserWidget> Pickup;
};
