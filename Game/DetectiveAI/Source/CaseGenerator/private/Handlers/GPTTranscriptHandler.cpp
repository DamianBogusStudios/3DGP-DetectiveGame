// Fill out your copyright notice in the Description page of Project Settings.


#include "Handlers/GPTTranscriptHandler.h"
#include "HttpGPTTranscriptRequest.h"

UGPTTranscriptHandler::UGPTTranscriptHandler()
{
}


void UGPTTranscriptHandler::SendSpeechForTranscript(UObject* const Caller, TArray<uint8>& AudioData,
	FTranscriptDelegate Callback, FErrorReceivedDelegate ErrorCallback)
{
	FString FileName = "audio.wav";
	UHttpGPTTranscriptRequest* Request = UHttpGPTTranscriptRequest::StartSpeechToText(Caller, AudioData, FileName);
	BindCallbacks(Request);
	Request->Activate();
}


void UGPTTranscriptHandler::BindCallbacks(UHttpGPTTranscriptRequest* Request)
{
	if (Request)
	{
		Request->OnError.AddDynamic(this, &UGPTTranscriptHandler::OnErrorReceivedInternal);
		Request->OnSpeechTranscribed.AddDynamic(this, &UGPTTranscriptHandler::OnTranscriptionReceivedInternal);
	}
}

void UGPTTranscriptHandler::OnErrorReceivedInternal(const FString& ErrorMsg)
{
	UE_LOG(LogTemp, Error, TEXT("STT Error: %s"), *ErrorMsg);
}

void UGPTTranscriptHandler::OnTranscriptionReceivedInternal(const FString& TranscribedText)
{
	// OnTranscriptionReceived.Broadcast(TranscribedText);
	UE_LOG(LogTemp, Log, TEXT("Transcribed Text: %s"), *TranscribedText);
}
