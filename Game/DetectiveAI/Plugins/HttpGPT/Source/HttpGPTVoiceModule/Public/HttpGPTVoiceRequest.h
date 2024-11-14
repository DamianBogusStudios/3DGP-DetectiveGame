#pragma once

#include <CoreMinimal.h>
#include <Tasks/HttpGPTBaseTask.h>
#include <Structures/HttpGPTVoiceTypes.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "HttpGPTVoiceRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHttpGPTTextToSpeechResponseDelegate, USoundWaveProcedural*, AudioData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHttpGPTErrorResponseDelegate, const FString&, ErrorMessage);

/**
 * Class to handle Text-to-Speech requests using OpenAI's TTS models
 */
UCLASS(NotPlaceable, Category = "HttpGPT | Voice", meta = (ExposedAsyncProxy = AsyncTask))
class HTTPGPTVOICEMODULE_API UHttpGPTVoiceRequest : public UHttpGPTBaseTask
{
    GENERATED_BODY()

public:
    // Delegate called when speech synthesis is completed successfully
    UPROPERTY(BlueprintAssignable, Category = "HttpGPT | Voice")
    FHttpGPTTextToSpeechResponseDelegate OnSpeechSynthesized;

    // Delegate called when an error occurs
    UPROPERTY(BlueprintAssignable, Category = "HttpGPT | Voice")
    FHttpGPTErrorResponseDelegate OnError;

    static UHttpGPTVoiceRequest* StartTextToSpeech(UObject* const WorldObject, const FString& Text);
    
    UFUNCTION(BlueprintCallable, Category = "HttpGPT | Voice | TextToSpeech",
        meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Start Text-to-Speech"))
    static UHttpGPTVoiceRequest* StartTextToSpeech(
        UObject* const WorldContextObject,
        const FString InText, 
        const FHttpGPTVoiceOptions VoiceOptions
    );
    
    virtual void Activate() override;

protected:
    
    // Override to set up the HTTP request content
    virtual FString SetRequestContent() override;

    // Override to handle the response
    virtual void OnProgressCompleted(const FString& Content, bool bWasSuccessful) override;

    virtual bool CanActivateTask() const override;

    virtual FString GetEndpointURL() const override;

private:

    USoundWaveProcedural* SynthesizedSoundWave;
    FString TextToSynthesize;

    FHttpGPTVoiceOptions VoiceOptions;

    void PlaySynthesizedAudio(const TArray<uint8>& AudioData);
    void SaveAudioToFile(const TArray<uint8>& AudioData, const FString& FilePath);
};
