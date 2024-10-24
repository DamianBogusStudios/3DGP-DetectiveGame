// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonCaseTypes.h"
#include "Interfaces/DialogueProvider.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueSystem.generated.h"

class ULLMService;
struct FActorDescription;
struct FMessage;

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UDialogueSystem : public UWorldSubsystem, public IDialogueProvider
{
	GENERATED_BODY()

public:
	//virtual void RequestSendMessage(UObject* Caller, FString& Message) override;
	virtual void RequestDialogueOptions(UObject* Caller, FActorDescription& ActorDescription) override;
	virtual FMessageDelegate& GetResponseDelegate() override;
	virtual FDialogueOptionsDelegate& GetDialogueOptionsDelegate() override;
	
protected:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	UPROPERTY()
	TScriptInterface<ILLMService> LLMService;

	FMessageDelegate OnMessageReceived;
	FDialogueOptionsDelegate OnDialogueOptionsReceived;

	UObject* DialogueRequestCaller;
	

	bool ParseFromJsonToStruct(const FString& Content, UScriptStruct* Schema, TSharedPtr<void> StructInstance);


	

};
