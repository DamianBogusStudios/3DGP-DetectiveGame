// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonCaseTypes.h"
#include "UObject/Interface.h"
#include "STTService.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTranscriptDelegate, const FString&, TranscribedText);

// This class does not need to be modified.
UINTERFACE()
class USTTService : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CASEGENERATOR_API ISTTService
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SendSpeechForTranscript(UObject* const Caller, TArray<uint8>& AudioData,
		FTranscriptDelegate Callback,
		FErrorReceivedDelegate ErrorCallback) = 0;
};
