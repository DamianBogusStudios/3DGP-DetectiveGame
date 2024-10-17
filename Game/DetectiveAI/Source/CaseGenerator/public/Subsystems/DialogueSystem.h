// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonCaseTypes.h"
#include "Interfaces/DialogueProvider.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueSystem.generated.h"

class UChatGPT;
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
	
	virtual void RequestDialogueOptions(UObject* Caller, FActorDescription& ActorDescription) override;
	virtual FMessageDelegate& GetResponseDelegate() override;
	virtual FDialogueOptionsDelegate& GetDialogueOptionsDelegate() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "OpenAI")
	int32 MaxTokens{2000};

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	TObjectPtr<UChatGPT> ChatGPT;

	FMessageDelegate OnMessageReceived;
	FDialogueOptionsDelegate OnDialogueOptionsReceived;

	UObject* DialogueRequestCaller;
	
	UFUNCTION()
	void SendMessage(const FText& Text);

	void RequestDialogueOptions(const FString& ActorDescription);
	
	void InitChatGPT();

	void OnRequestCompleted();
	void OnRequestUpdated(const FMessage& Message, bool WasError);

};
