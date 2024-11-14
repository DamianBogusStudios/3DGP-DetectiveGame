// Fill out your copyright notice in the Description page of Project Settings.


#include "Handlers/GPTVoiceHandler.h"
#include "HttpGPTVoiceRequest.h"

UGPTVoiceHandler::UGPTVoiceHandler()
{
}

void UGPTVoiceHandler::SendTextToVoice(UObject* const Caller, const FString& Message, FVoiceDelegate VoiceDelegate) //add voice gender needed
{
	OnVoiceReceived = VoiceDelegate;
	SendTextForVoiceResponse(Caller, Message);
}

void UGPTVoiceHandler::SendTextForVoiceResponse(UObject* const Caller, const FString& Message)
{
	auto Request = UHttpGPTVoiceRequest::StartTextToSpeech(Caller, Message);
	BindCallbacks(Request);
	Request->Activate(); 	
}

void UGPTVoiceHandler::BindCallbacks(UHttpGPTVoiceRequest* Request)
{
	if (Request)
	{
		Request->OnError.AddDynamic(this, &UGPTVoiceHandler::OnErrorReceived);
		Request->OnSpeechSynthesized.AddDynamic(this, &UGPTVoiceHandler::OnSpeechSynthesized);
	}
}

void UGPTVoiceHandler::OnErrorReceived(const FString& ErrorMsg)
{
	UE_LOG(LogTemp, Display, TEXT("OnErrorReceived: %s"), *ErrorMsg);
}

void UGPTVoiceHandler::OnSpeechSynthesized(USoundWaveProcedural* AudioData)
{
	OnVoiceReceived.ExecuteIfBound(AudioData);
	UE_LOG(LogTemp, Display, TEXT("OnSpeechSynthesized"));
}
