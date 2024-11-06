// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LLMServiceLocator.generated.h"

class ILLMService;
/**
 * 
 */
UCLASS()
class CASEGENERATOR_API ULLMServiceLocator : public UObject
{
	GENERATED_BODY()
	
public:
	static TScriptInterface<ILLMService> GetService();
	static void Cleanup();
private:
	
	static TScriptInterface<ILLMService> Service;
	static TScriptInterface<ILLMService> NullService;
	
	static void InitializeService();

	virtual void BeginDestroy() override;
};
