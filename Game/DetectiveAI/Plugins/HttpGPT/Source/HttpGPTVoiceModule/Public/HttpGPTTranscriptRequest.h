// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Tasks/HttpGPTBaseTask.h>
#include "UObject/Object.h"
#include "HttpGPTTranscriptRequest.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHttpGPTSpeechResponseDelegate, const FString&, TranscribedText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHttpGPTErrorSpeechDelegate, const FString&, ErrorMessage);

/**
 * Class to handle Speech-to-Text requests using OpenAI's TTS models
 */
UCLASS(NotPlaceable, Category = "HttpGPT | Voice", meta = (ExposedAsyncProxy = AsyncTask))
class HTTPGPTVOICEMODULE_API UHttpGPTTranscriptRequest : public UHttpGPTBaseTask
{
   GENERATED_BODY()
 public:
 UPROPERTY(BlueprintAssignable, Category = "HttpGPT | Voice")
 FHttpGPTSpeechResponseDelegate OnSpeechTranscribed;

 UPROPERTY(BlueprintAssignable, Category = "HttpGPT | Voice")
 FHttpGPTErrorSpeechDelegate OnError;

 static UHttpGPTTranscriptRequest* StartSpeechToText(UObject* const WorldContextObject, const TArray<uint8>& AudioData, const FString& FileName = TEXT("audio.wav"));

 // UFUNCTION(BlueprintCallable, Category = "HttpGPT | Voice | SpeechToText",
 //       meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Start Speech-to-Text"))
 // static UHttpGPTVoiceRequest* StartSpeechToText(
 //     UObject* const WorldContextObject,
 //     const TArray<uint8>& AudioData, 
 //     const FString FileName = TEXT("audio.wav")
 // );
 virtual void Activate() override;

protected:
	
	virtual void InitializeRequest() override;
 virtual FString SetRequestContent() override;
    virtual void OnProgressCompleted(const FString& Content, bool bWasSuccessful) override;
 virtual bool CanActivateTask() const override;
    virtual FString GetEndpointURL() const override;

private:

	TArray<uint8> BuildData();
	void AppendStringToRequestBody(const FString& InString, TArray<uint8>& RequestBody);

 TArray<uint8> AudioData;
 FString FileName;
 FString Boundary;

};