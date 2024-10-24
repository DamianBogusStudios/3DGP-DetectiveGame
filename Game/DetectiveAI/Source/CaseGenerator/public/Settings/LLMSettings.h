// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LLMSettings.generated.h"

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
	
	
public:

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "LLM", AdvancedDisplay)
	TEnumAsByte<EActiveLLM> ActiveLLM;

};
