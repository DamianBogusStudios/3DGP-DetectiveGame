// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TTSService.h"
#include "TempSoundActor.generated.h"

class ITTSService;
class USoundWaveProcedural;

UCLASS()
class DETECTIVEAI_API ATempSoundActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATempSoundActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	TScriptInterface<ITTSService> TTSService;

	TObjectPtr<USoundWaveProcedural> ProcessedSoundWave;

	FVoiceDelegate VoiceDelegate;

	void VoiceReceived(USoundWaveProcedural* SoundWave);

	bool bProcessing = false;
};
