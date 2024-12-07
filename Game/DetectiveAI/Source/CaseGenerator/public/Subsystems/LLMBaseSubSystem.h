// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LLMBaseSubSystem.generated.h"

class UPromptConfigData;
class ILLMService;

USTRUCT()
struct FPromptBuffer
{
	GENERATED_BODY()

	UPROPERTY()
	FString Message;
	UPROPERTY()
	UScriptStruct* Schema = nullptr;

	FPromptBuffer()
	{
		Message = "";
	}

	FPromptBuffer(FString InMessage) : Message(InMessage)
	{
		
	}

	FPromptBuffer(FString InMessage, UScriptStruct* InSchema) : Message(InMessage), Schema(InSchema)
	{
		
	}
};


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
	UPROPERTY()
	bool bSkipGeneration;

	UPROPERTY()
	FPromptBuffer LastRequest;


	/* virtual functions to be overriden */
	virtual void PostInit();	
	virtual void MessageReceived(FString& Message) ;
	virtual void StructuredMessageReceived(FString& Message, UScriptStruct* Schema);
	virtual void ErrorReceived(FString& Message, UScriptStruct* Schema);

	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void GetConfigFiles();
	//void BindCallbacks();
	
	/* requests */
	void SendCustomInstructions(const FString& Prompt);
	void SendMessage(const FString& Prompt);
	void SendStructuredMessage(const FString& Prompt, UScriptStruct* Schema);
	void RetryLastMessage();
	
private:
	/*Callback Functions */
	UFUNCTION()
	void OnMessagedReceivedInternal(FString& Message);
	UFUNCTION()
	void OnStructuredMessageReceivedInternal(FString& Message, UScriptStruct* Struct);
	UFUNCTION()
	void OnErrorReceivedInternal(FString& Message, UScriptStruct* Struct);
	
	// UFUNCTION()
	// void FunctionCallReceived(UObject* Caller, FString& Message, FName& FunctionName, TArray<FString>& ArgNames, TArray<FString>& ArgValues);

};
