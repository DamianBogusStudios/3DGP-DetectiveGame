// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLMBaseSubSystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Types/CommonCaseTypes.h"
#include "CaseSystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCaseActorsDelegate, const TArray<FActorDescription>&, Actors);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCaseCluesDelegate, const TArray<FClue>&, Clues);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCaseDetailsDelegate,
	const EMotive&, Motive,
	const EMurderWeapon&, MurderWeapon,
	const FString&, Context);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCaseGenerationFinishedDelegate,
	double, ElapsedTime);
/**
* CaseSystem
*
* process of generation
* 1. Random initial values are set in code.
* 2. Custom instructions along with initial values sent to chat completions API
* 3. Actors are generated sequentially one at a time with next generation triggered by receiving previous callback
* 4. Clues are generated with nested schema
* 5. Actor context properties updated to include connections.
* 6. Delegates are triggered to initiate game start and spawning of actors/clues.
*/
UCLASS()
class CASEGENERATOR_API UCaseSystem : public ULLMBaseSubSystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCaseGenerationFinishedDelegate OnCaseGenerationFinished;
	
	UPROPERTY(BlueprintAssignable)
	FCaseActorsDelegate OnActorsGenerated;
	UPROPERTY(BlueprintAssignable)
	FCaseCluesDelegate OnCluesGenerated;
	UPROPERTY(BlueprintAssignable)
	FCaseDetailsDelegate OnCaseDetailsGenerated;

	UFUNCTION()
	double GetStartTime() const { return GenStartTime; }
	
protected:

	virtual void PostInit() override;
	virtual void MessageReceived(FString& Message) override;
	virtual void StructuredMessageReceived(FString& Message, UScriptStruct* Struct) override;
	
private:

	FCaseFile CaseFile;
	TArray<FString> GeneratedActorSchemas;
	int NumOfActors;
	int NumOfClues;

	double GenStartTime;

	/* generative functions */
	void RandomiseInitialParams();
	void GenerateCase();
	void GenerateActor();
	void GenerateClues();
	void GenerateConnections();
	void FinishGeneration();
};