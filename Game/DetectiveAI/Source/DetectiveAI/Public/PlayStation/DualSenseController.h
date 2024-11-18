﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DualSenseController.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UDualSenseController : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "DualSense")
	static void SetLightBarColour(const FColor& LightColour);


	UFUNCTION(Blueprintable, Category = "DualSense")
	static void SetTriggerEffectProperty(int StartPos, int EndPos, int Strength, int Trigger);
private:

	UPROPERTY(EditAnywhere, Category = "Logging")
	FString PlatformWarningPrefix = "Platform not set to PS5: ";
};
