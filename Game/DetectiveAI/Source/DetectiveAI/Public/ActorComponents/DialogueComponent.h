// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/TTSService.h"
#include "Types/CommonCaseTypes.h"
#include "DialogueComponent.generated.h"


class ITTSService;
class ILLMService;
class UPromptConfigData;
class IDialogueProvider;
class UDialogueWidget;
class UBehaviorTree;
class USoundWaveProcedural;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DETECTIVEAI_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI)
	TObjectPtr<UBehaviorTree> DialogueTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	FActorDescription ActorDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	FDialogueOptions DialogueOptions;

	FMessageDelegate MessageDelegate;
	FVoiceDelegate VoiceDelegate;

	UFUNCTION()
	void SetDescription(const FActorDescription& Description);
	UFUNCTION()
	void StartDialogue() const;
	UFUNCTION()
	void FinishDialogue() const;
	
protected:

	UPROPERTY()
	TObjectPtr<UPromptConfigData> PromptConfig;
	UPROPERTY()
	TScriptInterface<ILLMService> LLMService;
	UPROPERTY()
	TScriptInterface<ITTSService> TTSService;
	UPROPERTY()
	TObjectPtr<USoundWaveProcedural> ProcessedSoundWave;
	
	bool bActorRegistered = false;
	bool bInDialogue = false;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void GetServices();
	void GetConfigFiles();
	void BindCallbacks();
	void RegisterWitness();

	void SendMessageToActor(const FString& Message);
	void OnMessageReceived(FString& Message);
	void OnVoiceReceived(USoundWaveProcedural* SoundWave);
	
	//
	// UFUNCTION()
	// void OnDialogueStarted(AActor* Caller, UUserWidget* Widget);
	//
	// UFUNCTION()
	// void OnAdvanceDialogue(AActor* Caller, UUserWidget* Widget);
	//
	// UFUNCTION()
	// void OnFinishDialogue(AActor* Caller, UUserWidget* Widget);
	//
	// // UFUNCTION()
	// // void OnMessageReceived(TObjectPtr<UActorDescription>);
	//
	// UFUNCTION()
	// void OnDialogueOptionsReceived(FDialogueOptions& InDialogueOptions);
	//
	// UFUNCTION(BlueprintCallable)
	// FString GetGreeting();
	//
	// UFUNCTION(BlueprintCallable)
	// FDialogueOptions GetDialogueOptions();
	

	// FDialogueOptionsDelegate DialogueOptionsDelegate;
	
// protected:
//
// 	// virtual void BeginPlay() override;
// 	// virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
// private:

	// TScriptInterface<IDialogueProvider> DialogueProvider; 
	// bool bInDialogue = false;
	//
	// void BindDialogueDelegates();
	// void UnBindDialogueDelegates();

	};
