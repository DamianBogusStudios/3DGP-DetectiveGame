// Fill out your copyright notice in the Description page of Project Settings.


#include "Handlers/GPTVoiceHandler.h"
#include "HttpGPTVoiceRequest.h"

UGPTVoiceHandler::UGPTVoiceHandler()
{
}

void UGPTVoiceHandler::SendTextToVoice(UObject* const Caller, const FString& Message, EVoiceType VoiceType,
	FVoiceDelegate Callback, FErrorReceivedDelegate ErrorCallback) //add voice gender needed
{
	VoiceRequest = FVoiceRequest(Caller, Message, VoiceType, Callback, ErrorCallback);

	FHttpGPTVoiceOptions HttpGptVoiceOptions;

	HttpGptVoiceOptions.Voice = VoiceTypeToSynthVoice(VoiceType);
	
	auto Request = UHttpGPTVoiceRequest::StartTextToSpeech(Caller, Message, HttpGptVoiceOptions);
	BindCallbacks(Request);
	Request->Activate(); 	
}

EHttpGPTSynthVoice UGPTVoiceHandler::VoiceTypeToSynthVoice(EVoiceType VoiceType) 
{
	switch (VoiceType)
	{
	case EVoiceType::HighMale: 
		return EHttpGPTSynthVoice::alloy;
	case EVoiceType::StandardMale: 
		return EHttpGPTSynthVoice::echo;
	case  EVoiceType::BritishMale: 
		return EHttpGPTSynthVoice::fable;
	case EVoiceType::DeepMale: 
		return EHttpGPTSynthVoice::onyx;
	case EVoiceType::OlderFemale: 
		return EHttpGPTSynthVoice::nova;
	case EVoiceType::DeepFemale: 
		return EHttpGPTSynthVoice::shimmer;
	}

	return EHttpGPTSynthVoice::alloy;
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
