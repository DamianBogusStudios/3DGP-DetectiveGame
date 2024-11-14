// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TTSService.generated.h"

class USoundWaveProcedural;

DECLARE_DELEGATE_OneParam(FVoiceDelegate,
                          USoundWaveProcedural* /* Message */);

// This class does not need to be modified.
UINTERFACE()
class UTTSService : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CASEGENERATOR_API ITTSService
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SendTextToVoice(UObject* const Caller, const FString& Message, FVoiceDelegate Delegate) = 0;
	
};
