// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/STTService.h"
#include "UObject/Object.h"
#include "GPTTranscriptHandler.generated.h"


USTRUCT()
struct FTranscriptRequest
{
	GENERATED_BODY()
	
	
	TWeakObjectPtr<UObject> Caller;
	TArray<uint8> VoiceInput;
	FTranscriptDelegate OnTranscriptReceived;
	FErrorReceivedDelegate OnErrorReceived;
	
	double StartTime = 0;
	double EndTime = 0;
	
	FTranscriptRequest(UObject* InCaller, const TArray<uint8>& AudioData, const FTranscriptDelegate& Callback, const FErrorReceivedDelegate& OnErrorReceived)
		: Caller(InCaller), VoiceInput(AudioData), OnTranscriptReceived(Callback), OnErrorReceived(OnErrorReceived)
	{
		StartTime = FPlatformTime::Seconds();
	}

	FTranscriptRequest()
	{
		
	}

	void ExecuteCallback(FString& Transcript)
	{
		EndTime = FPlatformTime::Seconds();
		
		OnTranscriptReceived.ExecuteIfBound(Transcript);
	}

	void ExecuteError(FString ErrorMsg) const
	{
		OnErrorReceived.ExecuteIfBound(ErrorMsg, nullptr);
	}
};
/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UGPTTranscriptHandler : public UObject, public ISTTService
{
	GENERATED_BODY()

public:
	
	UGPTTranscriptHandler();
	
	virtual void SendSpeechForTranscript(UObject* const Caller, TArray<uint8>& AudioData, FTranscriptDelegate Callback, FErrorReceivedDelegate ErrorCallback) override;
	
private:

	FTranscriptRequest TranscriptRequest;

	UFUNCTION()
	void BindCallbacks(class UHttpGPTTranscriptRequest* Request);
	UFUNCTION()
	void OnErrorReceivedInternal(const FString& ErrorMsg);
	UFUNCTION()
	void OnTranscriptionReceivedInternal(const FString& TranscribedText);

	
};
