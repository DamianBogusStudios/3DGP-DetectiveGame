// Author: Damian Boguslawski
#pragma once

#include <CoreMinimal.h>
#include "HttpGPTVoiceTypes.generated.h"

UENUM(BlueprintType, Category = "HttpGPT | Voice", Meta = (DisplayName = "HttpGPT Voice Model"))
enum class EHttpGPTVoiceModel : uint8
{
	tts1 UMETA(DisplayName = "tts-1"),
	tts1_hd UMETA(DisplayName = "tts-1-hd")
};

UENUM(BlueprintType, Category = "HttpGPT | Voice", Meta = (DisplayName = "HttpGPT Voice Model"))
enum class EHttpGPTSynthVoice : uint8
{
	alloy UMETA(DisplayName = "alloy"),
	echo UMETA(DisplayName = "echo"),
	fable UMETA(DisplayName = "fable"),
	onyx UMETA(DisplayName = "onyx"),
	nova UMETA(DisplayName = "nova"),
	shimmer UMETA(DisplayName = "shimmer")
};

UENUM(BlueprintType)
enum class EHttpGPTVoiceOutputFormat : uint8
{
	Mp3     UMETA(DisplayName = "mp3"),
	Pcm     UMETA(DisplayName = "pcm"),
	Opus    UMETA(DisplayName = "opus"),
	Aac     UMETA(DisplayName = "aac"),
	Flac    UMETA(DisplayName = "flac"),
	Wav     UMETA(DisplayName = "wav"),
};

USTRUCT(BlueprintType, Category = "HttpGPT | Chat", Meta = (DisplayName = "HttpGPT Chat Options"))
struct HTTPGPTCOMMONMODULE_API FHttpGPTVoiceOptions
{
	GENERATED_BODY()

	FHttpGPTVoiceOptions();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HttpGPT | Voice", Meta = (DisplayName = "Model"))
	EHttpGPTVoiceModel Model;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HttpGPT | Voice", Meta = (DisplayName = "Voice Type"))
	EHttpGPTSynthVoice Voice;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HttpGPT | Voice", Meta = (DisplayName = "Output Format"))
	EHttpGPTVoiceOutputFormat OutputFormat;


private:
	void SetDefaults();
};

