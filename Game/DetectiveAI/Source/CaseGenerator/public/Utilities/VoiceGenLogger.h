// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VoiceGenLogger.generated.h"

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UVoiceGenLogger : public UObject
{
	GENERATED_BODY()
public:

	UVoiceGenLogger();
	
	void ResponseRequested();
	void ResponseReceived(int stringLength);

	void VoiceRequested();
	void VoiceReceived();
private:
	FString FilePath = "";

	int ResponseLength = 0;
	double ResponseStart = 0;
	double VoiceStart = 0;
	
	double ResponseDuration = 0;
	double VoiceDuration = 0;

	void WriteToCSV() const;
};
