// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLMBaseSubSystem.h"
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
class CASEGENERATOR_API UDialogueSystem : public ULLMBaseSubSystem, public IDialogueProvider
{
	GENERATED_BODY()

public:
	
	virtual void RequestSendMessage(UObject* Caller, FString& Message) override;
	virtual void RequestDialogueOptions(UObject* Caller, FActorDescription& ActorDescription, FDialogueOptionsDelegate& Delegate) override;

protected:
	
	virtual void PostInit() override;
	virtual void MessageReceived(FString& Message) override;
	virtual void StructuredMessageReceived(FString& Message, UScriptStruct* Struct) override;

private:
	
	FDialogueOptionsDelegate OnDialogueOptionsReceived;
};
