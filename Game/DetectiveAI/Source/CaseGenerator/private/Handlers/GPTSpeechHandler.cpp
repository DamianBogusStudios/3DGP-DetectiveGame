// Fill out your copyright notice in the Description page of Project Settings.


#include "Handlers/GPTSpeechHandler.h"
#include "HttpGPTTranscriptRequest.h"

UGPTSpeechHandler::UGPTSpeechHandler()
{
}


void UGPTSpeechHandler::SendSpeechForTranscript(UObject* const Caller, TArray<uint8>& AudioData)
{
	FString FileName = "audio.wav";
	UHttpGPTTranscriptRequest* Request = UHttpGPTTranscriptRequest::StartSpeechToText(Caller, AudioData, FileName);
	BindCallbacks(Request);
	Request->Activate();
}


void UGPTSpeechHandler::BindCallbacks(UHttpGPTTranscriptRequest* Request)
{
	if (Request)
	{
		Request->OnError.AddDynamic(this, &UGPTSpeechHandler::OnErrorReceived);
		Request->OnSpeechTranscribed.AddDynamic(this, &UGPTSpeechHandler::OnTranscriptionReceivedInternal);
	}
}

void UGPTSpeechHandler::OnErrorReceived(const FString& ErrorMsg)
{
	UE_LOG(LogTemp, Error, TEXT("STT Error: %s"), *ErrorMsg);
}

void UGPTSpeechHandler::OnTranscriptionReceivedInternal(const FString& TranscribedText)
{
	OnTranscriptionReceived.Broadcast(TranscribedText);
	UE_LOG(LogTemp, Log, TEXT("Transcribed Text: %s"), *TranscribedText);
}
