// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ServiceLocator.generated.h"

class ITTSService;
class ISTTService;
class ILLMService;
/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UServiceLocator : public UObject
{
	GENERATED_BODY()
    
public:

	static TScriptInterface<ILLMService> GetService_LLM();
	static TScriptInterface<ITTSService> GetService_TTS();
	static TScriptInterface<ISTTService> GetService_STT();

	static void Cleanup();

private:

	static TScriptInterface<ILLMService> LLMService;
	static TScriptInterface<ITTSService> TTSService;
	static TScriptInterface<ISTTService> STTService;

	static void InitializeLLMService();
	static void InitializeTTSService();
	static void InitializeSTTService();

	virtual void BeginDestroy() override;
};
