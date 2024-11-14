// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TempSoundActor.h"
#include "Handlers/ServiceLocator.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWaveProcedural.h"


// Sets default values
ATempSoundActor::ATempSoundActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATempSoundActor::BeginPlay()
{
	Super::BeginPlay();

	/*temp*/
	FString VoiceMsg = "This is a test msg. Hi my name is Damian.";

	
	TTSService = UServiceLocator::GetService_TTS();

	if(TTSService)
	{
		VoiceDelegate.BindUObject(this, &ATempSoundActor::VoiceReceived);
		TTSService->SendTextToVoice(GetWorld()->GetFirstPlayerController(), VoiceMsg, VoiceDelegate);
	}
	/********/
}

void ATempSoundActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); 
	
	// if(ProcessedSoundWave && bProcessing)
	// {
	// 	int32 RemainingBufferSize = ProcessedSoundWave->GetAvailableAudioByteCount();
	//
	// 	if (RemainingBufferSize == 0)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Sound Played"));
	// 		UGameplayStatics::PlaySound2D(this, ProcessedSoundWave);
	// 		bProcessing =false;
	// 	}
	// }
}

void ATempSoundActor::VoiceReceived(USoundWaveProcedural* SoundWave)
{
	bProcessing = true;
	ProcessedSoundWave = SoundWave;
	UGameplayStatics::PlaySound2D(this, ProcessedSoundWave);
}
