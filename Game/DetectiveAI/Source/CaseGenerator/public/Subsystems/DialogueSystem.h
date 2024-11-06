// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonCaseTypes.h"
#include "Interfaces/DialogueProvider.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueSystem.generated.h"

class ILLMService;
struct FActorDescription;
struct FMessage;

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UDialogueSystem : public UGameInstanceSubsystem, public IDialogueProvider
{
	GENERATED_BODY()

public:
	virtual void RequestSendMessage(UObject* Caller, FString& Message) override;
	virtual void RequestDialogueOptions(UObject* Caller, FActorDescription& ActorDescription, FDialogueOptionsDelegate& Delegate) override;

protected:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	//virtual void OnWorldBeginPlay(UWorld& InWorld) override;
private:

	UPROPERTY()
	TScriptInterface<ILLMService> LLMService;

	void BindServiceCallbacks();

	UFUNCTION()
	void MessagedReceived(UObject* Caller, FString& Message);
	UFUNCTION()
	void StructuredMessageReceived(UObject* Caller, FString& Message, UScriptStruct* Struct);
	
	FMessageDelegate OnMessageReceived;
	FDialogueOptionsDelegate OnDialogueOptionsReceived;

};
