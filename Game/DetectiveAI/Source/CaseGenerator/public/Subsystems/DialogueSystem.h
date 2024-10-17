// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueSystem.generated.h"

class UChatGPT;
struct FMessage;
/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UDialogueSystem : public UWorldSubsystem
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "OpenAI")
	int32 MaxTokens{2000};

protected:	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	TObjectPtr<UChatGPT> ChatGPT;

	// UPROPERTY()
	// TObjectPtr<USaveSettings> SaveSettings;

private:
	
	UFUNCTION()
	void SendMessage(const FText& Text);

	void RequestDialogueOptions(const FString& ActorDescription);
	
	void InitChatGPT();

	void OnRequestCompleted();
	void OnRequestUpdated(const FMessage& Message, bool WasError);

};
