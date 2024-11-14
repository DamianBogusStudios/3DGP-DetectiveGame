// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PromptConfigData.generated.h"

/**
 * UPromptConfigData
 * 
 * A data asset that stores configurable chat prompts to be used with an LLM to generate cases procedurally.
 */
UCLASS()
class CASEGENERATOR_API UPromptConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompts | Config", meta = (MultiLine = true, ToolTip = ""))
	FString CustomInstructions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompts | Config", meta = (MultiLine = true, ToolTip = ""))
	FString WitnessCustomInstructions;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompts | Actors")
	FString Victim =  "Based on the provided murder weapon and motive, generate the victim of the murder with the provided schema.";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompts | Actors")
	FString Perpetrator  = "Using the murder weapon, motive and victim generated, generate the perpetrator with the provided schema.";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompts | Actors")
	FString SecondaryActor  = "Using the details of the case and previous actors, generate a random secondary actor, using the provided schema.";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompts | Actors", meta = (MultiLine = true))
	FString Connections; //todo

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompts | Case Data",
		meta = (Tooltip = "Will require a int to be formatted in for the number of clues"))
	FString Clues  = "Generate a collection of %d clues related to the case with the provided schema";

	
	
};
