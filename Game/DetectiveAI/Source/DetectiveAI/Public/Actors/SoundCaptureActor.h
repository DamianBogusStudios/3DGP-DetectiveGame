// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISubmixBufferListener.h"
#include "Components/SynthComponent.h"
#include "GameFramework/Actor.h"
#include "SoundCaptureActor.generated.h"

class ISTTService;
class UAudioCaptureComponent;
class FSubmixBufferListener;

USTRUCT()
struct FWavHeader
{
	GENERATED_BODY()
	
	char ChunkID[4];         // "RIFF"
	uint32 ChunkSize;        // 4 + (8 + Subchunk1Size) + (8 + Subchunk2Size)
	char Format[4];          // "WAVE"

	char Subchunk1ID[4];     // "fmt "
	uint32 Subchunk1Size;    // 16 for PCM
	uint16 AudioFormat;      // PCM = 1
	uint16 NumChannels;
	uint32 SampleRate;
	uint32 ByteRate;
	uint16 BlockAlign;
	uint16 BitsPerSample;

	char Subchunk2ID[4];     // "data"
	uint32 Subchunk2Size;    // data size
};

UCLASS()
class DETECTIVEAI_API ASoundCaptureActor : public AActor, public IAudioBufferListener
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundCaptureActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY()
	TObjectPtr<UAudioCaptureComponent> AudioCaptureComponent;
	UPROPERTY()
	TObjectPtr<USoundSubmix> AudioSubmix;
	UPROPERTY()
	TScriptInterface<ISTTService> STTService;

	TSharedPtr<FSubmixBufferListener> BufferListener;

	UFUNCTION()
	void ToggleVoiceCapture(bool InEnableVoiceCapture);
	
	void OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels,
		const int32 InSampleRate, double AudioClock);

	virtual void OnGeneratedBuffer(const float* AudioBuffer, const int32 NumSamples, const int32 NumChannels) override;

	void OnMicInputReceived(const float* AudioBuffer, const int32 NumSamples, const int32 NumChannels);

	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Voice Detection Settings")
	bool bPushToTalk = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Voice Detection Settings")
	float ActivationThreshold= 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Voice Detection Settings")
	float SilenceThreshold = 0.05f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Voice Detection Settings")
	float SilenceDuration = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voice Detection Settings")
	float MaxAmplitude;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	FTimerHandle SilenceTimerHandle;
	
	TArray<float> AudioBuffer;
	TArray<uint8> FinalAudioData;

	float CurrentSilenceTime;
	bool bIsRecording;
	bool bEnableVoiceCapture = false;

	int32 Channels;
	int32 SampleRate;

	double StartTranscriptionTime;
	
	// void OnSubmixAudio(const USoundWave* OwningSoundWave, const float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock);
	
	void StartRecording();
	void StopRecording();
	
	void ConvertAudioBufferToUInt8();
	void GenerateWavData();

	UFUNCTION()
	void TranscriptReceived(const FString& Transcript);
};
