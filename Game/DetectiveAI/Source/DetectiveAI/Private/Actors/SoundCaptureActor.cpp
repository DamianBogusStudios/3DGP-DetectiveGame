// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SoundCaptureActor.h"
#include "AudioCaptureComponent.h"
#include "AudioDevice.h"
#include "AudioMixerDevice.h"
#include "Sound/SoundSubmix.h"
#include "Handlers/ServiceLocator.h"
#include "Interfaces/STTService.h"
#include "RawObjects/FSubmixBufferListener.h"
#include "Subsystems/UISystem.h"
#include "UI/DialogueWidget.h"

// Sets default values
ASoundCaptureActor::ASoundCaptureActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioCaptureComponent = CreateDefaultSubobject<UAudioCaptureComponent>(TEXT("AudioCaptureComponent"));
}

// Called when the game starts or when spawned
void ASoundCaptureActor::BeginPlay()
{
	Super::BeginPlay();

	if(auto Widget = CreateWidget(GetWorld()->GetFirstPlayerController(), WidgetClass))
	{
		Widget->AddToViewport();
	}

	AudioSubmix = NewObject<USoundSubmix>(USoundSubmix::StaticClass());
	STTService = UServiceLocator::GetService_STT();
	STTService->GetDelegate().AddDynamic(this, &ASoundCaptureActor::TranscriptReceived);
	
	if (AudioCaptureComponent)
	{
		// FAudioDevice* AudioDevice = GetWorld()->GetAudioDeviceRaw();
		//
		// BufferListener = MakeShared<FSubmixBufferListener>(TWeakObjectPtr<ASoundCaptureActor>(this));
		// // ISubmixBufferListener* BufferListenerPtr = BufferListener.Get();
		//
		// AudioCaptureComponent->SoundSubmix = AudioSubmix;
		// AudioCaptureComponent->SetOutputToBusOnly(true);
		// AudioCaptureComponent->Start();
		
		// if (AudioDevice && AudioSubmix)
		// {
		// 	AudioDevice->RegisterSubmixBufferListener(BufferListener.ToSharedRef(), *AudioSubmix);
		// }
	}
}

void ASoundCaptureActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AudioCaptureComponent)
	{
		AudioCaptureComponent->Stop();
	}

	FAudioDevice* AudioDevice = GetWorld()->GetAudioDeviceRaw();
	if (AudioDevice && AudioSubmix)
	{
		AudioDevice->UnregisterSubmixBufferListener(BufferListener.ToSharedRef(), AudioDevice->GetMainSubmixObject());
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASoundCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoundCaptureActor::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 InSampleRate, double AudioClock)
{
	OnMicInputReceived(AudioData, NumSamples, NumChannels);
}

void ASoundCaptureActor::OnGeneratedBuffer(const float* AudioData, const int32 NumSamples, const int32 NumChannels)
{
	OnMicInputReceived(AudioData, NumSamples, NumChannels);
}

void ASoundCaptureActor::OnMicInputReceived(const float* AudioData, const int32 NumSamples, const int32 NumChannels)
{
	MaxAmplitude = 0.0f;
	Channels = NumChannels;
	SampleRate = 16000;

	for (int32 i = 0; i < NumSamples; i++)
	{
		float AbsSample = FMath::Abs(AudioData[i]);
		if (AbsSample > MaxAmplitude)
		{
			MaxAmplitude = AbsSample;
		}
	}

	if (MaxAmplitude > ActivationThreshold)
	{
		if (!bIsRecording)
		{
			StartRecording();
		}

		CurrentSilenceTime = 0.0f;
		AudioBuffer.Append(AudioData, NumSamples);
	}
	else if (bIsRecording)
	{
		float BufferDuration = static_cast<float>(NumSamples) / (SampleRate * NumChannels);
		CurrentSilenceTime += BufferDuration;

		AudioBuffer.Append(AudioData, NumSamples);

		if (CurrentSilenceTime >= SilenceDuration)
		{
			StopRecording();
		}
	}
}

void ASoundCaptureActor::StartRecording()
{
	bIsRecording = true;
	CurrentSilenceTime = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("Recording started."));
}

void ASoundCaptureActor::StopRecording()
{
	bIsRecording = false;
	//ConvertAudioBufferToUInt8();
	GenerateWavData();
	
	AudioBuffer.Reset();
	CurrentSilenceTime = 0.0f;

	if(STTService)
	{
		StartTranscriptionTime = GetWorld()->GetTimeSeconds();
		STTService->SendSpeechForTranscript(this, FinalAudioData);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Recording stopped."));
}

void ASoundCaptureActor::ConvertAudioBufferToUInt8()
{
	int32 NumSamples = AudioBuffer.Num();
	FinalAudioData.Reset();
	FinalAudioData.AddUninitialized(NumSamples * sizeof(int16));

	int16* Int16Data = reinterpret_cast<int16*>(FinalAudioData.GetData());

	for (int32 i = 0; i < NumSamples; i++)
	{
		// Convert float [-1.0f, 1.0f] to int16 [-32768, 32767]
		float ClampedSample = FMath::Clamp(AudioBuffer[i], -1.0f, 1.0f);
		Int16Data[i] = static_cast<int16>(ClampedSample * 32767);
	}

	UE_LOG(LogTemp, Log, TEXT("Audio data converted to uint8. Size: %d bytes"), FinalAudioData.Num());
}

void ASoundCaptureActor::GenerateWavData()
{
    if (AudioBuffer.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("AudioBuffer is empty, cannot generate WAV data."));
        return;
    }


    FWavHeader Header;

    // Fill in the header
    FMemory::Memcpy(Header.ChunkID, "RIFF", 4);
    FMemory::Memcpy(Header.Format, "WAVE", 4);
    FMemory::Memcpy(Header.Subchunk1ID, "fmt ", 4);
    FMemory::Memcpy(Header.Subchunk2ID, "data", 4);

    Header.Subchunk1Size = 16;  // PCM
    Header.AudioFormat = 1;     // PCM format
    Header.NumChannels = Channels;  // Number of channels
    Header.SampleRate = SampleRate;    // Samples per second
    Header.BitsPerSample = 16;         // Bits per sample

    Header.ByteRate = Header.SampleRate * Header.NumChannels * Header.BitsPerSample / 8;
    Header.BlockAlign = Header.NumChannels * Header.BitsPerSample / 8;

    int32 NumSamples = AudioBuffer.Num();
    uint32 AudioDataSize = NumSamples * sizeof(int16);

    Header.Subchunk2Size = AudioDataSize;
    Header.ChunkSize = 4 + (8 + Header.Subchunk1Size) + (8 + Header.Subchunk2Size);

    // Prepare the final audio data array
    FinalAudioData.Reset();
    FinalAudioData.AddUninitialized(sizeof(FWavHeader) + AudioDataSize);

    // Copy the header
    FMemory::Memcpy(FinalAudioData.GetData(), &Header, sizeof(FWavHeader));

    // Convert float samples to int16 and copy them
    int16* Int16Data = reinterpret_cast<int16*>(FinalAudioData.GetData() + sizeof(FWavHeader));
    for (int32 i = 0; i < NumSamples; i++)
    {
        float ClampedSample = FMath::Clamp(AudioBuffer[i], -1.0f, 1.0f);
        Int16Data[i] = static_cast<int16>(ClampedSample * 32767);
    }

    UE_LOG(LogTemp, Log, TEXT("Audio data converted to WAV format. Size: %d bytes"), FinalAudioData.Num());
}

void ASoundCaptureActor::TranscriptReceived(const FString& Transcript)
{
	if(auto UI = GetGameInstance()->GetSubsystem<UUISystem>())
	{
		auto Widget = Cast<UDialogueWidget>(UI->GetWidget(EUIElementType::NPCDialogue));

		if(Widget)
		{
			double TimeElapsed = GetWorld()->GetTimeSeconds() - StartTranscriptionTime;

			UE_LOG(LogTemp, Log, TEXT("Time To Transcribe: %f"), TimeElapsed);
			Widget->TranscriptReceived(Transcript);
			
			FString Message = FString::Printf(TEXT("%f: %s"), TimeElapsed, *Transcript);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

		}
		
	}
	
}

void ASoundCaptureActor::ToggleVoiceCapture(bool InEnableVoiceCapture)
{
	if(AudioSubmix)
	{
		bEnableVoiceCapture = InEnableVoiceCapture;
		float SubmixVol = bEnableVoiceCapture ? 1.0f : 0.0f;

		AudioSubmix->SetSubmixOutputVolume(GetWorld(), SubmixVol);
	}
}
