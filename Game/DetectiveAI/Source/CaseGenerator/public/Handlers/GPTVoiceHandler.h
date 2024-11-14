// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/TTSService.h"
#include "UObject/Object.h"
#include "GPTVoiceHandler.generated.h"

class USoundWaveProcedural;
class UHttpGPTVoiceRequest;
/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UGPTVoiceHandler : public UObject, public ITTSService
{
	GENERATED_BODY()

public:

	UGPTVoiceHandler();

	FVoiceDelegate OnVoiceReceived;

	/* text -> voice*/
	virtual void SendTextToVoice(UObject* const Caller, const FString& Message, FVoiceDelegate Delegate) override;

private:

	void SendTextForVoiceResponse(UObject* const Caller, const FString& Message);

	void BindCallbacks(UHttpGPTVoiceRequest* Request);
	
	UFUNCTION()
	void OnErrorReceived(const FString& ErrorMsg);
	UFUNCTION()
	void OnSpeechSynthesized(USoundWaveProcedural* AudioData);

};
