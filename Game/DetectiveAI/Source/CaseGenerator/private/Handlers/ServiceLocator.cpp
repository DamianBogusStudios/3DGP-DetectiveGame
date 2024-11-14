// Fill out your copyright notice in the Description page of Project Settings.


#include "Handlers/ServiceLocator.h"
#include "Handlers/GPTHandler.h"
#include "Handlers/GPTVoiceHandler.h"
#include "Handlers/NullLLMService.h"
#include "Handlers/NullVoiceHandler.h"
#include "Interfaces/LLMService.h"
#include "Settings/LLMSettings.h"
#include "Interfaces/TTSService.h"

TScriptInterface<ILLMService> UServiceLocator::LLMService;
TScriptInterface<ITTSService> UServiceLocator::TTSService;

void UServiceLocator::BeginDestroy()
{
    Super::BeginDestroy();
    Cleanup();
}

void UServiceLocator::Cleanup()
{
	if(LLMService)
	{
		LLMService.GetObject()->RemoveFromRoot();
		LLMService.SetObject(nullptr);
		LLMService.SetInterface(nullptr);
	}
	if(TTSService)
	{
	    TTSService.GetObject()->RemoveFromRoot();
	    TTSService.SetObject(nullptr);
	    TTSService.SetInterface(nullptr);
	}
}

TScriptInterface<ILLMService> UServiceLocator::GetService_LLM()
{
    if(!LLMService)
    {
        InitializeLLMService();
    }

    return LLMService;
}
TScriptInterface<ITTSService> UServiceLocator::GetService_TTS()
{
	if(!TTSService)
	{
		InitializeTTSService();
	}

	return TTSService;
}

void UServiceLocator::InitializeLLMService()
{
    UObject* Handler = nullptr;

    if (const ULLMSettings* Settings = GetDefault<ULLMSettings>())
    {
        switch (Settings->GetActiveLLM())
        {
        case EActiveLLM::ChatGPT:
            Handler = NewObject<UGPTHandler>();
        	break;
        default:
            Handler = NewObject<UNullLLMService>();
        	break;
        }
    }
    
    if (Handler)
    {
        Handler->AddToRoot();
        LLMService.SetObject(Handler);
        LLMService.SetInterface(Cast<ILLMService>(Handler));
    }
}


void UServiceLocator::InitializeTTSService()
{
	UObject* Handler = nullptr;

	if (const ULLMSettings* Settings = GetDefault<ULLMSettings>())
	{
		switch (Settings->GetActiveTTS())
		{
		case EActiveTTS::OpenAI:
			Handler = NewObject<UGPTVoiceHandler>();
			break;
		default:
			Handler = NewObject<UNullVoiceHandler>();
			break;
		}
	}
    
	if (Handler)
	{
		Handler->AddToRoot();
		TTSService.SetObject(Handler);
		TTSService.SetInterface(Cast<ITTSService>(Handler));
	}
}
