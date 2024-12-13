// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CaseGenLogger.generated.h"

struct FCaseFile;
UENUM()
enum EGenContext
{
	Actor,
	Clue,
	Context
};
/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UCaseGenLogger : public UObject
{
	GENERATED_BODY()

public:
	UCaseGenLogger();
	
	void GenStarted(EGenContext Context);
	void GenFinished(EGenContext Context);

	void GenFailed();

	void WriteCaseFile(const FCaseFile& CaseFile);

private:
	FString FilePath = "";
	
	double ActorGenStartTime = 0;
	double ClueGenStartTime = 0;
	double ContextGenStartTime = 0;

	double ActorGenDuration = 0;
	double ClueGenDuration = 0;
	double ContextGenDuration = 0;
	double TotalGenDuration = 0;

	void WriteToCSV() const;
};
