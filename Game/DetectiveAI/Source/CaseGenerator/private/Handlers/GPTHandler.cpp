// Fill out your copyright notice in the Description page of Project Settings.
#include "Handlers/GPTHandler.h"

#include "AssetTypeCategories.h"
#include "HttpGPTChatModule/Public/Tasks/HttpGPTChatRequest.h"
#include "Types/CommonCaseTypes.h"

#pragma region ILLMService
void UGPTHandler::SendMessage(UObject* Caller, FString& Message)
{
	
}

void UGPTHandler::SendStructuredMessage(UObject* const WorldObject, const FString& Message, UScriptStruct* Struct)
{	
	FHttpGPTStructuredResponse StructuredResponse;
	
	StructuredResponse.Name = Struct->GetFName();  
	StructuredResponse.Description = Struct->GetMetaData("Tooltip");
	
	for(TFieldIterator<FProperty> PropIt(Struct); PropIt; ++PropIt)
	{
		auto Property = *PropIt;
		FHttpGPTFunctionProperty HttpProperty;
			
		HttpProperty.Name = Property->GetFName();
		HttpProperty.Description = Property->GetMetaData("Tooltip");
		HttpProperty.Type = UHttpGPTHelper::CPPToPropertyType(Property->GetCPPType());
			
		StructuredResponse.Properties.Add(HttpProperty);
		StructuredResponse.RequiredProperties.Add(HttpProperty.Name);
	}
	/* forcing valid for now */
	StructuredResponse.bIsValid = true;

	Caller = WorldObject;
	ProvidedSchemaStruct = Struct;
	bStructuredMessageRequested = true;
	SendMessageStructured(WorldObject, Message, StructuredResponse);
}

FMessageDelegate& UGPTHandler::GetMessageDelegate()
{
	return OnMessageReceived;
}

FStructuredMessageDelegate& UGPTHandler::GetStructuredMessageDelegate()
{
	return OnStructuredMessageReceived;
}
#pragma endregion ILLMService


 void UGPTHandler::SendMessageStructured(UObject* const WorldObject, const FString& Message, FHttpGPTStructuredResponse StructuredResponse)
 {
 	auto Request = UHttpGPTChatRequest::SendMessageStructured(WorldObject, Message, {}, StructuredResponse);
 	BindCallbacks(Request);
 	Request->Activate(); 	
 }

 void UGPTHandler::SendMessageDefault(UObject* const WorldObject, const FString& Message)
 {
 	auto Request = UHttpGPTChatRequest::SendMessage_DefaultOptions(WorldObject, Message, {});
 	BindCallbacks(Request);
 	Request->Activate();
 }

 void UGPTHandler::SendMessagesDefault(const TArray<FHttpGPTChatMessage>& Messages)
 {
 }

 void UGPTHandler::SendMessageCustom(const FString& Message, const FHttpGPTCommonOptions& CommonOptions,
 	const FHttpGPTChatOptions& ChatOptions)
 {
 }

 void UGPTHandler::SendMessagesCustom(const TArray<FHttpGPTChatMessage>& Messages,
 	const FHttpGPTCommonOptions& CommonOptions, const FHttpGPTChatOptions& ChatOptions)
 {
 	
 }



#pragma region Callbacks
 void UGPTHandler::OnProcessCompleted(const FHttpGPTChatResponse& Response)
{
	for(auto Choice : Response.Choices)
	{
		if(!Choice.Message.Content.IsEmpty())
		{
			if(bStructuredMessageRequested && ProvidedSchemaStruct)
			{
				FString Message = Choice.Message.Content;
				OnStructuredMessageReceived.Broadcast(Caller, Message, ProvidedSchemaStruct);
				return;
			}
		}
	}
}

 void UGPTHandler::OnProcessUpdated(const FHttpGPTChatResponse& Response)
 {
 	for(auto Choice : Response.Choices)
 	{
 		UE_LOG(LogTemp, Log, TEXT("Request Updated: %s"), *Choice.Message.Content);
 	}
 }

 void UGPTHandler::OnProcessStarted(const FHttpGPTChatResponse& Response)
 {
     //UE_LOG(LogTemp, Log, TEXT("Request Completed: %s"), *Response.Object.ToString());
 }

 void UGPTHandler::OnErrorReceived(const FHttpGPTChatResponse& Response)
 {
     UE_LOG(LogTemp, Error, TEXT("Error Received: %s"), *Response.Object.ToString());
 }

 void UGPTHandler::BindCallbacks(UHttpGPTChatRequest* Request)
 {
 	if (Request)
 	{
 		Request->ProcessCompleted.AddDynamic(this, &UGPTHandler::OnProcessCompleted);
 		Request->ProgressUpdated.AddDynamic(this, &UGPTHandler::OnProcessUpdated);
 		Request->ProgressStarted.AddDynamic(this, &UGPTHandler::OnProcessStarted);
 		Request->ErrorReceived.AddDynamic(this, &UGPTHandler::OnErrorReceived);
 	}
 }
#pragma endregion 