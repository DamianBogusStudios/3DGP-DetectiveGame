// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonCaseTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LLMBaseSubSystem.generated.h"

class UPromptConfigData;
class ILLMService;

/**
 * 
 */
UCLASS(Abstract)
class CASEGENERATOR_API ULLMBaseSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TScriptInterface<ILLMService> LLMService;

	UPROPERTY()
	TObjectPtr<UPromptConfigData> PromptConfig;

	FMessageDelegate MessageDelegate;
	FStructuredMessageDelegate StructuredMessageDelegate;

	/* virtual functions to be overriden */
	virtual void PostInit();	
	virtual void MessageReceived(FString& Message);
	virtual void StructuredMessageReceived(FString& Message, UScriptStruct* Schema);

	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void GetConfigFiles();
	void BindCallbacks();
	
	/* requests */
	void SendCustomInstructions(UObject* Caller, const FString& Prompt);
	void SendCustomInstructions(const FString& Prompt);
	void SendMessage(const FString& Prompt);
	void SendMessage(UObject* Caller, const FString& Prompt);
	void SendStructuredMessage(const FString& Prompt, UScriptStruct* Schema);

	
private:
	/*Callback Functions */
	UFUNCTION()
	void MessagedReceivedDelegate(FString& Message);
	UFUNCTION()
	void StructuredMessageReceivedDelegate(FString& Message, UScriptStruct* Struct);
	// UFUNCTION()
	// void FunctionCallReceived(UObject* Caller, FString& Message, FName& FunctionName, TArray<FString>& ArgNames, TArray<FString>& ArgValues);

};
