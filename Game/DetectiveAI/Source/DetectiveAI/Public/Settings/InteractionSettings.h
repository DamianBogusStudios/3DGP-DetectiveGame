// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InteractionSettings.generated.h"


class UDialogueWidget;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Interaction Settings"))
class DETECTIVEAI_API UInteractionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

protected:

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI", AdvancedDisplay)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	TSubclassOf<UDialogueWidget> GetDialogueWidgetClass() const { return DialogueWidgetClass; }

};
