// Fill out your copyright notice in the Description page of Project Settings.


#include "Structures/HttpGPTVoiceTypes.h"
#include "Management/HttpGPTSettings.h"


FHttpGPTVoiceOptions::FHttpGPTVoiceOptions()
{
	SetDefaults();
}

void FHttpGPTVoiceOptions::SetDefaults()
{
	if (const UHttpGPTSettings* const Settings = GetDefault<UHttpGPTSettings>())
	{
		Model = Settings->VoiceOptions.Model;
		Voice = Settings->VoiceOptions.Voice;
		OutputFormat = Settings->VoiceOptions.OutputFormat;
	}
}