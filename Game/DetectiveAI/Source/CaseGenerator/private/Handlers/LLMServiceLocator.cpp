// Fill out your copyright notice in the Description page of Project Settings.


#include "Handlers/LLMServiceLocator.h"
#include "Handlers/GPTHandler.h"
#include "Interfaces/LLMService.h"
#include "Settings/LLMSettings.h"

TScriptInterface<ILLMService> ULLMServiceLocator::Service;

TScriptInterface<ILLMService> ULLMServiceLocator::GetService()
{
	if(!Service)
		InitializeService();

	return Service;
}


void ULLMServiceLocator::InitializeService()
{

	if(const ULLMSettings* Settings = GetDefault<ULLMSettings>())
	{
		switch (Settings->ActiveLLM)
		{
		case EActiveLLM::ChatGPT:
		default:
			auto Handler = NewObject<UGPTHandler>();
			Service.SetObject(Handler);
			Service.SetInterface(Cast<ILLMService>(Handler));
			break;
		}
	}
}