// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpGPTTranscriptRequest.h"

#include "HttpModule.h"
#include "LogHttpGPT.h"
#include "Interfaces/IHttpResponse.h"

UHttpGPTTranscriptRequest* UHttpGPTTranscriptRequest::StartSpeechToText(UObject* const WorldContextObject, const TArray<uint8>& InAudioData, const FString& InFileName)
{
    UHttpGPTTranscriptRequest* NewRequest = NewObject<UHttpGPTTranscriptRequest>();
    NewRequest->AudioData = InAudioData;
    NewRequest->FileName = InFileName;
    NewRequest->RegisterWithGameInstance(WorldContextObject);
    return NewRequest;
}

void UHttpGPTTranscriptRequest::Activate()
{
    Super::Activate();

    if (!CanActivateTask())
    {
        OnError.Broadcast(TEXT("Cannot activate STT task."));
        return;
    }

    FString RequestContent = SetRequestContent();
    if (RequestContent.IsEmpty())
    {
        OnError.Broadcast(TEXT("Failed to set up the STT request content."));
        return;
    }

    // AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this]
    // {
    //     SendRequest();
    // });
    HttpRequest->ProcessRequest();
}

void UHttpGPTTranscriptRequest::InitializeRequest()
{
    FScopeLock Lock(&Mutex);

    UE_LOG(LogHttpGPT_Internal, Display, TEXT("%s (%d): Initializing request object"), *FString(__FUNCTION__), GetUniqueID());

    Boundary = TEXT("----Boundary") + FGuid::NewGuid().ToString();
    FString ContentType = FString::Printf(TEXT("multipart/form-data; boundary=%s"), *Boundary);
    
    
    HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(GetEndpointURL());
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", *ContentType);
    HttpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), {GetCommonOptions().APIKey.ToString()}));

}

bool UHttpGPTTranscriptRequest::CanActivateTask() const
{
    if (AudioData.IsEmpty())
    {
        UE_LOG(LogHttpGPT, Error, TEXT("AudioData is empty."));
        return false;
    }
    return Super::CanActivateTask();
}

FString UHttpGPTTranscriptRequest::GetEndpointURL() const
{
    return TEXT("https://api.openai.com/v1/audio/transcriptions");
}

FString UHttpGPTTranscriptRequest::SetRequestContent()
{
    FScopeLock Lock(&Mutex);

    if (!HttpRequest.IsValid())
    {
        return FString();
    }

    UE_LOG(LogHttpGPT_Internal, Display, TEXT("Setting up STT request content."));

    auto Data = BuildData();   
    HttpRequest->SetContent(Data);

    return "AUDIO DATA SENT FOR TRANSCRIPT";
}

TArray<uint8> UHttpGPTTranscriptRequest::BuildData()
{
    TArray<uint8> RequestBody;
    
    FString LineBreak = TEXT("\r\n");

    AppendStringToRequestBody(FString::Printf(TEXT("--%s%s"), *Boundary, *LineBreak), RequestBody);
AppendStringToRequestBody(FString::Printf(TEXT("Content-Disposition: form-data; name=\"file\"; filename=\"%s\""), *FileName) + LineBreak, RequestBody);
    AppendStringToRequestBody(TEXT("Content-Type: audio/wav") + LineBreak + LineBreak, RequestBody);

    RequestBody.Append(AudioData);

    // Add a line break
    AppendStringToRequestBody(LineBreak, RequestBody);

    // Add the 'model' part
    AppendStringToRequestBody(FString::Printf(TEXT("--%s%s"), *Boundary, *LineBreak), RequestBody);
    AppendStringToRequestBody(TEXT("Content-Disposition: form-data; name=\"model\"") + LineBreak + LineBreak, RequestBody);
    AppendStringToRequestBody(TEXT("whisper-1") + LineBreak, RequestBody);
    AppendStringToRequestBody(FString::Printf(TEXT("--%s--%s"), *Boundary, *LineBreak), RequestBody);

    return RequestBody;
}

void UHttpGPTTranscriptRequest::AppendStringToRequestBody(const FString& InString, TArray<uint8>& RequestBody)
{
    FTCHARToUTF8 Converter(*InString);
    RequestBody.Append((uint8*)Converter.Get(), Converter.Length());    
}

void UHttpGPTTranscriptRequest::OnProgressCompleted(const FString& Content, bool bWasSuccessful)
{
    FScopeLock Lock(&Mutex);

    if (!bWasSuccessful || HttpRequest->GetStatus() != EHttpRequestStatus::Succeeded)
    {
        FString ErrorMessage = TEXT("STT request failed.");
        UE_LOG(LogHttpGPT, Error, TEXT("%s"), *ErrorMessage);
        AsyncTask(ENamedThreads::GameThread, [this, ErrorMessage]()
        {
            OnError.Broadcast(ErrorMessage);
        });
        return;
    }

    FHttpResponsePtr Response = HttpRequest->GetResponse();
    if (!Response.IsValid())
    {
        FString ErrorMessage = TEXT("Invalid HTTP response.");
        UE_LOG(LogHttpGPT, Error, TEXT("%s"), *ErrorMessage);
        AsyncTask(ENamedThreads::GameThread, [this, ErrorMessage]()
        {
            OnError.Broadcast(ErrorMessage);
        });
        return;
    }

    int32 ResponseCode = Response->GetResponseCode();
    if (ResponseCode != 200)
    {
        FString ErrorMessage = FString::Printf(TEXT("STT request failed with response code %d."), ResponseCode);
        UE_LOG(LogHttpGPT, Error, TEXT("%s"), *ErrorMessage);
        AsyncTask(ENamedThreads::GameThread, [this, ErrorMessage]()
        {
            OnError.Broadcast(ErrorMessage);
        });
        return;
    }

    auto ResponseContent = Response->GetContentAsString();

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        FString TranscribedText = JsonObject->GetStringField("text");
        OnSpeechTranscribed.Broadcast(TranscribedText);
    }
    else
    {
        OnError.Broadcast(TEXT("Failed to parse STT response."));
    }
}

