// Fill out your copyright notice in the Description page of Project Settings.


#include "Handlers/GPTVoiceHandler.h"
#include "HttpGPTVoiceRequest.h"

UGPTVoiceHandler::UGPTVoiceHandler()
{
}

void UGPTVoiceHandler::SendTextToVoice(UObject* const Caller, const FString& Message,
	FVoiceDelegate Callback, FErrorReceivedDelegate ErrorCallback) //add voice gender needed
{
	VoiceRequest = FVoiceRequest(Caller, Message, Callback, ErrorCallback);
	
	auto Request = UHttpGPTVoiceRequest::StartTextToSpeech(Caller, Message);
	BindCallbacks(Request);
	Request->Activate(); 	
}

void UGPTVoiceHandler::BindCallbacks(UHttpGPTVoiceRequest* Request)
{
	if (Request)
	{
		Request->OnError.AddDynamic(this, &UGPTVoiceHandler::OnErrorReceivedInternal);
		Request->OnSpeechSynthesized.AddDynamic(this, &UGPTVoiceHandler::OnSpeechSynthesizedInternal);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UGPTVoiceHandler::OnErrorReceivedInternal(const FString& ErrorMsg)
{
	VoiceRequest.ExecuteError(ErrorMsg);
	UE_LOG(LogTemp, Display, TEXT("OnErrorReceived: %s"), *ErrorMsg);
}

void UGPTVoiceHandler::OnSpeechSynthesizedInternal(USoundWaveProcedural* AudioData)
{
	VoiceRequest.ExecuteCallback(AudioData);
	UE_LOG(LogTemp, Display, TEXT("OnSpeechSynthesized"));
}
