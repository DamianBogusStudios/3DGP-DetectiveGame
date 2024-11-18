// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/STTService.h"
#include "UObject/Object.h"
#include "GPTSpeechHandler.generated.h"

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UGPTSpeechHandler : public UObject, public ISTTService
{
	GENERATED_BODY()

public:
	
	UGPTSpeechHandler();

	UPROPERTY(BlueprintAssignable)
	FTranscriptDelegate OnTranscriptionReceived;
	
	virtual void SendSpeechForTranscript(UObject* const Caller, TArray<uint8>& AudioData) override;

	virtual FTranscriptDelegate& GetDelegate() override
	{
		return OnTranscriptionReceived;
	}
	
private:
	
	void BindCallbacks(class UHttpGPTTranscriptRequest* Request);

	UFUNCTION()
	void OnErrorReceived(const FString& ErrorMsg);

	UFUNCTION()
	void OnTranscriptionReceivedInternal(const FString& TranscribedText);

	
};
