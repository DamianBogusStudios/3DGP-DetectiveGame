// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/PromptConfigData.h"
#include "Engine/DeveloperSettings.h"
#include "LLMSettings.generated.h"

class UPromptConfigData;

UENUM(BlueprintType)
enum EActiveLLM
{
	ChatGPT,
	Llama,
	Claude	
};
// /**
//  * 
//  */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "LLM Settings"))
class CASEGENERATOR_API ULLMSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
private:

	UPROPERTY(Config, EditDefaultsOnly, Category = "LLM")
	TEnumAsByte<EActiveLLM> ActiveLLM;

	UPROPERTY(Config, EditDefaultsOnly, Category = "LLM")
	TSoftObjectPtr<UPromptConfigData> PromptConfigData;

public:

	TEnumAsByte<EActiveLLM> GetActiveLLM() const
	{
		return ActiveLLM;
	}
	
	UPromptConfigData* GetPromptConfigData() const
	{
		if (!PromptConfigData.IsValid())
		{
			return PromptConfigData.LoadSynchronous();
		}
		return PromptConfigData.Get();
	}
	// UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "LLM", meta = (MultiLine="true"))
	// FString CustomInstructions;
	
};
