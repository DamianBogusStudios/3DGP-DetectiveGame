// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/TTSService.h"
#include "UObject/Object.h"
#include "GPTVoiceHandler.generated.h"

enum class EHttpGPTSynthVoice : uint8;
class USoundWaveProcedural;
class UHttpGPTVoiceRequest;

USTRUCT()
struct FVoiceRequest
{
	GENERATED_BODY()
	
	
	TWeakObjectPtr<UObject> Caller;
	FString Message;
	EVoiceType VoiceType = EVoiceType::BritishMale;
	FVoiceDelegate OnVoiceReceived;
	FErrorReceivedDelegate OnErrorReceived;
	
	double StartTime = 0;
	double EndTime = 0;
	
	FVoiceRequest(UObject* InCaller, const FString& InMessage, EVoiceType InVoiceType, const FVoiceDelegate& Callback, const FErrorReceivedDelegate& OnErrorReceived)
		: Caller(InCaller), Message(InMessage), VoiceType(InVoiceType), OnVoiceReceived(Callback), OnErrorReceived(OnErrorReceived)
	{
		StartTime = FPlatformTime::Seconds();
	}

	FVoiceRequest()
	{
		
	}

	void ExecuteCallback(USoundWaveProcedural* VoiceAudio)
	{
		EndTime = FPlatformTime::Seconds();
		
		OnVoiceReceived.ExecuteIfBound(VoiceAudio);
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
class CASEGENERATOR_API UGPTVoiceHandler : public UObject, public ITTSService
{
	GENERATED_BODY()

public:

	UGPTVoiceHandler();
	
	virtual void SendTextToVoice(UObject* const Caller, const FString& Message, EVoiceType VoiceType,
		FVoiceDelegate Callback, FErrorReceivedDelegate ErrorCallback) override;

private:
	
	FVoiceRequest VoiceRequest;

	UFUNCTION()
	void BindCallbacks(UHttpGPTVoiceRequest* Request);
	UFUNCTION()
	void OnErrorReceivedInternal(const FString& ErrorMsg) ;
	UFUNCTION()
	void OnSpeechSynthesizedInternal(USoundWaveProcedural* AudioData);


	static EHttpGPTSynthVoice VoiceTypeToSynthVoice(EVoiceType VoiceType);
};
