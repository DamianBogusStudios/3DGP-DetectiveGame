// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/TTSService.h"
#include "UObject/Object.h"
#include "NullVoiceHandler.generated.h"

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UNullVoiceHandler : public UObject, public ITTSService
{
	GENERATED_BODY()
public:

	virtual void SendTextToVoice(UObject* const Caller, const FString& Message, FVoiceDelegate VoiceDelegate) override;
};
