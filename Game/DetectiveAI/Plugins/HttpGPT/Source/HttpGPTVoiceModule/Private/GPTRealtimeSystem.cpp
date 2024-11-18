// Fill out your copyright notice in the Description page of Project Settings.


#include "GPTRealtimeSystem.h"

#include "IWebSocket.h"
#include "WebSocketsModule.h"

void UGPTRealtimeSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	UE_LOG(LogTemp, Warning, TEXT("UGPTRealtimeSystem Module Loaded"));
	
	TMap<FString, FString> Headers;
	Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *GetAPIKey()));
	Headers.Add(TEXT("OpenAI-Beta"), TEXT("realtime=v1"));

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(GetEndpointURL(), TEXT(""), Headers);

	WebSocket->OnConnected().AddUObject(this, &UGPTRealtimeSystem::OnConnected);
	WebSocket->OnConnectionError().AddUObject(this, &UGPTRealtimeSystem::OnConnectionError);
	WebSocket->OnClosed().AddUObject(this, &UGPTRealtimeSystem::OnClosed);
	WebSocket->OnMessage().AddUObject(this, &UGPTRealtimeSystem::OnMessageReceived);

	WebSocket->Connect();
}

void UGPTRealtimeSystem::Deinitialize()
{
	Super::Deinitialize();

	if(WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
}

void UGPTRealtimeSystem::OnConnected()
{
	UE_LOG(LogTemp, Display, TEXT("Realtime API: Websocket Connected"));
}

void UGPTRealtimeSystem::OnConnectionError(const FString& ErrorMessage)
{
	UE_LOG(LogTemp, Display, TEXT("Realtime API: Websocket Connection Error: %s"), *ErrorMessage);
}
void UGPTRealtimeSystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	FString WasClean = (bWasClean ? "true" : "false");
	UE_LOG(LogTemp, Display, TEXT("Realtime API: Closed, StatusCode: %d, Reason: %s, WasClean: %s"), StatusCode, *Reason, *WasClean);
}

void UGPTRealtimeSystem::OnMessageReceived(const FString& Message)
{
	UE_LOG(LogTemp, Display, TEXT("Realtime API: Message Received: %s"), *Message);
}


// UHttpGPTVoiceRequest* UHttpGPTVoiceRequest::StartTextToSpeech(UObject* const WorldObject, const FString& Text)
// {
//     return StartTextToSpeech(WorldObject, Text, FHttpGPTVoiceOptions());
// }
//
// UHttpGPTVoiceRequest* UHttpGPTVoiceRequest::StartTextToSpeech(
// UObject* const WorldContextObject, const FString InText, const FHttpGPTVoiceOptions VoiceOptions)
// {
//     UHttpGPTVoiceRequest* NewRequest = NewObject<UHttpGPTVoiceRequest>();
//     NewRequest->TextToSynthesize = InText;
//     NewRequest->VoiceOptions = VoiceOptions;
//     NewRequest->RegisterWithGameInstance(WorldContextObject);
//     return NewRequest;
// }
//
// void UHttpGPTVoiceRequest::Activate()
// {
//     Super::Activate();
//
//     if (!CanActivateTask())
//     {
//         OnError.Broadcast(TEXT("Cannot activate TTS task."));
//         return;
//     }
//
//     FString RequestContent = SetRequestContent();
//     if (RequestContent.IsEmpty())
//     {
//         OnError.Broadcast(TEXT("Failed to set up the TTS request content."));
//         return;
//     }
//
//     HttpRequest->ProcessRequest();
// }
//
// bool UHttpGPTVoiceRequest::CanActivateTask() const
// {
//     if (TextToSynthesize.IsEmpty())
//     {
//         UE_LOG(LogHttpGPT, Error, TEXT("TextToSynthesize is empty."));
//         return false;
//     }
//     return Super::CanActivateTask();
// }
//
// FString UHttpGPTVoiceRequest::GetEndpointURL() const
// {
//     return TEXT("https://api.openai.com/v1/audio/speech");
// }
//
// FString UHttpGPTVoiceRequest::SetRequestContent()
// {
//     FScopeLock Lock(&Mutex);
//
//     if (!HttpRequest.IsValid())
//     {
//         return FString();
//     }
//
//     UE_LOG(LogHttpGPT_Internal, Display, TEXT("Setting up TTS request content."));
//     
//     TSharedPtr<FJsonObject> JsonRequest = MakeShared<FJsonObject>();
//     JsonRequest->SetStringField(TEXT("model"), UHttpGPTHelper::ModelToString(VoiceOptions.Model));
//     JsonRequest->SetStringField(TEXT("voice"), UHttpGPTHelper::VoiceToString(VoiceOptions.Voice));
//     JsonRequest->SetStringField(TEXT("input"), TextToSynthesize);
//     JsonRequest->SetStringField(TEXT("response_format"), UHttpGPTHelper::OutputFormatToString(VoiceOptions.OutputFormat));
//     
//     FString RequestContentString;
//     TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestContentString);
//     FJsonSerializer::Serialize(JsonRequest.ToSharedRef(), Writer);
//
//     HttpRequest->SetContentAsString(RequestContentString);
//
//     return RequestContentString;
// }
//
// void UHttpGPTVoiceRequest::OnProgressCompleted(const FString& Content, bool bWasSuccessful)
// {
//     FScopeLock Lock(&Mutex);
//
//     if (!bWasSuccessful || HttpRequest->GetStatus() != EHttpRequestStatus::Succeeded)
//     {
//         FString ErrorMessage = TEXT("TTS request failed.");
//         UE_LOG(LogHttpGPT, Error, TEXT("%s"), *ErrorMessage);
//         AsyncTask(ENamedThreads::GameThread, [this, ErrorMessage]()
//         {
//             OnError.Broadcast(ErrorMessage);
//         });
//         return;
//     }
//
//     FHttpResponsePtr Response = HttpRequest->GetResponse();
//     if (!Response.IsValid())
//     {
//         FString ErrorMessage = TEXT("Invalid HTTP response.");
//         UE_LOG(LogHttpGPT, Error, TEXT("%s"), *ErrorMessage);
//         AsyncTask(ENamedThreads::GameThread, [this, ErrorMessage]()
//         {
//             OnError.Broadcast(ErrorMessage);
//         });
//         return;
//     }
//
//     int32 ResponseCode = Response->GetResponseCode();
//     if (ResponseCode != 200)
//     {
//         FString ErrorMessage = FString::Printf(TEXT("TTS request failed with response code %d."), ResponseCode);
//         UE_LOG(LogHttpGPT, Error, TEXT("%s"), *ErrorMessage);
//         AsyncTask(ENamedThreads::GameThread, [this, ErrorMessage]()
//         {
//             OnError.Broadcast(ErrorMessage);
//         });
//         return;
//     }
//
//     const TArray<uint8>& AudioData = Response->GetContent();
//     if (AudioData.Num() == 0)
//     {
//         FString ErrorMessage = TEXT("Received empty audio data from TTS API.");
//         UE_LOG(LogHttpGPT, Error, TEXT("%s"), *ErrorMessage);
//         AsyncTask(ENamedThreads::GameThread, [this, ErrorMessage]()
//         {
//             OnError.Broadcast(ErrorMessage);
//         });
//         return;
//     }
//
//     // Broadcast the audio data
//     AsyncTask(ENamedThreads::GameThread, [this, AudioData]()
//     {
//         PlaySynthesizedAudio(AudioData);
//     });
// }
//
// void UHttpGPTVoiceRequest::PlaySynthesizedAudio(const TArray<uint8>& AudioData)
// {
//     if(SynthesizedSoundWave)
//     {
//         SynthesizedSoundWave = nullptr;
//     }
//     
//     SynthesizedSoundWave = NewObject<USoundWaveProcedural>(USoundWaveProcedural::StaticClass());
//
//     if (!SynthesizedSoundWave)
//     {
//         UE_LOG(LogHttpGPT, Error, TEXT("Failed to create sound wave."));
//         return;
//     }
//
//     if (AudioData.Num() <= 44)
//     {
//         UE_LOG(LogTemp, Warning, TEXT("Audio data is too short to contain both header and PCM data"));
//         return;
//     }
//     
//     // For PCM format, we can use the raw data directly
//     const FString OutputFormat = UHttpGPTHelper::OutputFormatToString(VoiceOptions.OutputFormat);
//     if (OutputFormat == TEXT("wav"))
//     {
//         SynthesizedSoundWave->SoundGroup = SOUNDGROUP_Default;
//         SynthesizedSoundWave->bProcedural = true;
//         SynthesizedSoundWave->bLooping = false;
//         SynthesizedSoundWave->SetSampleRate(24000);
//         SynthesizedSoundWave->NumChannels = 1;
//
//
//         float SampleRate = 24000.0f;
//         SynthesizedSoundWave->Duration = static_cast<float>(AudioData.Num()) / (SampleRate * SynthesizedSoundWave->NumChannels * sizeof(int16));
//         
//         // Queue audio data for procedural playback
//         SynthesizedSoundWave->QueueAudio(AudioData.GetData(), AudioData.Num());
//     }
//     else
//     {
//         UE_LOG(LogHttpGPT, Warning, TEXT("Output format is not PCM. Decoding is required but not implemented."));
//         OnError.Broadcast(TEXT("Decoding of non-PCM formats is not implemented."));
//         return;
//     }
//     
//     SaveAudioToFile(AudioData, FPaths::ProjectSavedDir() + TEXT("SynthesizedAudio.wav"));
//
//     
//     OnSpeechSynthesized.Broadcast(SynthesizedSoundWave);
// }
//
//
//
// void UHttpGPTVoiceRequest::SaveAudioToFile(const TArray<uint8>& AudioData, const FString& FilePath)
// {
//     FFileHelper::SaveArrayToFile(AudioData, *FilePath);
// }