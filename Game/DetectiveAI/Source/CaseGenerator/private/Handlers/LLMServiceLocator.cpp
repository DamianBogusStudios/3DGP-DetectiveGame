// Fill out your copyright notice in the Description page of Project Settings.


#include "Handlers/LLMServiceLocator.h"
#include "Handlers/GPTHandler.h"
#include "Handlers/NullLLMService.h"
#include "Interfaces/LLMService.h"
#include "Settings/LLMSettings.h"

TScriptInterface<ILLMService> ULLMServiceLocator::Service;

void ULLMServiceLocator::BeginDestroy()
{
	UObject::BeginDestroy();
	Cleanup();
}


TScriptInterface<ILLMService> ULLMServiceLocator::GetService()
{
	if(!Service)
		InitializeService();

	return Service;
}

void ULLMServiceLocator::Cleanup()
{
	if(Service)
	{
		Service.GetObject()->RemoveFromRoot();
		Service.SetObject(nullptr);
		Service.SetInterface(nullptr);
	}
}


void ULLMServiceLocator::InitializeService()
{
	if(const ULLMSettings* Settings = GetDefault<ULLMSettings>())
	{
		UObject* Handler;
		
		switch (Settings->GetActiveLLM())
		{
		case EActiveLLM::ChatGPT:
			Handler = NewObject<UGPTHandler>();
			break;
		default:
			Handler = NewObject<UNullLLMService>();
			break;
		}

		if (Handler)
		{
			Handler->AddToRoot();
			Service.SetObject(Handler);
			Service.SetInterface(Cast<ILLMService>(Handler));
		}
	}
}