// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/TTSService.h"
#include "Types/CommonCaseTypes.h"
#include "DialogueComponent.generated.h"


// DECLARE_DYNAMIC_DELEGATE_OneParam(FMessageRelayDelegate, const FString&, MessageRelay);

class ISTTService;
class ITTSService;
class ILLMService;
class UDialogueWidget;
class USoundWaveProcedural;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DETECTIVEAI_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	FActorDescription ActorDescription;

	UPROPERTY()
	TScriptInterface<ILLMService> LLMService;
	UPROPERTY()
	TScriptInterface<ITTSService> TTSService;
	UPROPERTY()
	TScriptInterface<ISTTService> STTService;
	

	UFUNCTION()
	void SetDescription(const FActorDescription& Description);
	UFUNCTION()
	void StartDialogue();
	UFUNCTION()
	void FinishDialogue();
	UFUNCTION()
	void SendMessageToActor(const FString& Message);
	
protected:

	UPROPERTY()
	TObjectPtr<USoundWaveProcedural> ProcessedSoundWave;
	
	bool bActorRegistered = false;
	bool bInDialogue = false;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	
	UFUNCTION()
	void OnTranscriptReceived(const FString& Message);
	
	void GetServices();
	// void GetConfigFiles();
	void BindCallbacks();
	void RegisterWitness();
	void OnMessageReceived(FString& Message);
	void OnVoiceReceived(USoundWaveProcedural* SoundWave);
	};
