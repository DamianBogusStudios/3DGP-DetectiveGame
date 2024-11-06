// Fill out your copyright notice in the Description page of Project Settings.
#include "Handlers/GPTHandler.h"

#include "AssetTypeCategories.h"
#include "Utils/HttpGPTHelper.h"
#include "Structures/HttpGPTChatTypes.h"
#include "Tasks/HttpGPTChatRequest.h"



UGPTHandler::UGPTHandler()
{
	MetaDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/DT_MetaData.DT_MetaData'"));
	
	if (!MetaDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("GPTHandler: Data Table not found."));
		return;
	}
}

#pragma region Helper Functions
FString UGPTHandler::GetDescription(const UScriptStruct* InStruct)
{
	if(!MetaDataTable)
		return "";

	FString RowName =  InStruct->GetName();

	const FString ContextString(TEXT("LookupContext"));
	
	if (auto Row = MetaDataTable->FindRow<FDescriptionTableRow>(*RowName, ContextString, true))
	{
		UE_LOG(LogTemp, Log, TEXT("Row '%s' exists in the DataTable!"), *RowName);
		return Row->Description;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Row '%s' does not exist in the DataTable."), *RowName);
	return "";
}
FString UGPTHandler::GetDescription(const FProperty& InProperty)
{
	if(!MetaDataTable)
		return "";
	
	FName PropertyName = InProperty.GetFName();
	UStruct* OwnerStruct = InProperty.GetOwnerStruct();
	FName StructName = OwnerStruct ? OwnerStruct->GetFName() : FName(TEXT("Unknown"));

	FString RowName =  StructName.ToString() + "_" + PropertyName.ToString();

	const FString ContextString(TEXT("LookupContext"));
	
	if (auto Row = MetaDataTable->FindRow<FDescriptionTableRow>(*RowName, ContextString, true))
	{
		UE_LOG(LogTemp, Log, TEXT("Row '%s' exists in the DataTable!"), *RowName);
		return Row->Description;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Row '%s' does not exist in the DataTable."), *RowName);
	return "";
}

FHttpGPTFunctionProperty UGPTHandler::FPropertyToHttpFunctionProperty(const FProperty& InProperty, const FString& InDescription)
{
	FHttpGPTFunctionProperty HttpProperty;
	HttpProperty.Name = InProperty.GetFName();
	HttpProperty.Description = InDescription;
	HttpProperty.Type = UHttpGPTHelper::CPPToPropertyType(InProperty.GetCPPType());
	
	const FEnumProperty* EnumProperty = nullptr;
	
	if(HttpProperty.Type == EHttpGPTPropertyType::Array)
	{
		if (const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(&InProperty))
		{
			EnumProperty = CastField<FEnumProperty>(ArrayProperty->Inner);
		}
	}
	else
	{
		EnumProperty = CastField<FEnumProperty>(&InProperty);
	}
	
	if (EnumProperty)
	{
		if (const UEnum* Enum = EnumProperty->GetEnum())
		{
			for (int32 i = 0; i < Enum->NumEnums() - 1; ++i)
			{
				HttpProperty.Enum.Add(Enum->GetNameByIndex(i));
			}
		}
	}
	else if (const FByteProperty* ByteProperty = CastField<FByteProperty>(&InProperty))
	{
		if (const UEnum* Enum = ByteProperty->GetIntPropertyEnum())
		{
			for (int32 i = 0; i < Enum->NumEnums() - 1; ++i)
			{
				HttpProperty.Enum.Add(Enum->GetNameByIndex(i));
			}
		}
	}

	return HttpProperty;
}
FHttpGPTFunctionProperty  UGPTHandler::FunctionParamToHttpFunctionProperty(const FName& InName, const FString& InDescription, const FString& InType, const UEnum* InEnum)
{
	FHttpGPTFunctionProperty HttpProperty;
	HttpProperty.Name = InName;
	HttpProperty.Description = InDescription;
	HttpProperty.Type = UHttpGPTHelper::CPPToPropertyType(InType);

	if(InEnum)
	{
		for (int32 i = 0; i < InEnum->NumEnums() - 1; ++i)
		{
			HttpProperty.Enum.Add(FName(InEnum->GetNameStringByIndex(i)));
		}
	}
	return HttpProperty;
}
	
#pragma endregion

#pragma region ILLMService
void UGPTHandler::AddFunction(const FName& InName, const FString& InDescription)
{
	if(!Functions.Contains(InName))
	{
		FHttpGPTFunction HttpFunction;
		HttpFunction.Name = InName;
		HttpFunction.Description = InDescription;
		Functions.Add(InName, MakeUnique<FHttpGPTFunction>(HttpFunction));
	}
}
void UGPTHandler::AddFunctionParam(const FName& FuncName, const FName& InName, const FString& InDescription, const FString& InType, const UEnum* InEnum)
{
	if(Functions.Contains(FuncName))
	{
		FHttpGPTFunctionProperty HttpProperty = FunctionParamToHttpFunctionProperty(InName, InDescription, InType, InEnum);
		Functions[FuncName]->Properties.Add(HttpProperty);
		Functions[FuncName]->RequiredProperties.Add(InName);
	}
}


void UGPTHandler::SendCustomInstructions(UObject* Caller, FString& Message)
{
	AddMessageToHistory(Caller, EHttpGPTChatRole::System, Message);
}

void UGPTHandler::SendMessage(UObject* Caller, FString& Message)
{
	RequestCaller = Caller;
	AddMessageToHistory(Caller, EHttpGPTChatRole::User, Message);
	SendMessageDefault(Caller, *GetChatHistory(Caller));
	Functions.Empty();
}

void UGPTHandler::SendStructuredMessage(UObject* const WorldObject, const FString& Message, UScriptStruct* Struct)
{	
	FHttpGPTStructuredResponse StructuredResponse;
	
	StructuredResponse.Name = Struct->GetFName();  
	StructuredResponse.Description = GetDescription(Struct);

	
	for(TFieldIterator<FProperty> PropIt(Struct); PropIt; ++PropIt)
	{
		auto Property = *PropIt;
		FHttpGPTFunctionProperty HttpProperty = FPropertyToHttpFunctionProperty(*Property, GetDescription(*Property));
			
		StructuredResponse.Properties.Add(HttpProperty);
		StructuredResponse.RequiredProperties.Add(HttpProperty.Name);
	}
	/* forcing valid for now */
	StructuredResponse.bIsValid = true;

	RequestCaller = WorldObject;
	ProvidedSchemaStruct = Struct;
	AddMessageToHistory(WorldObject, EHttpGPTChatRole::User, Message);
	SendMessageStructured(WorldObject, *GetChatHistory(WorldObject), StructuredResponse);
	Functions.Empty();
}



FMessageDelegate& UGPTHandler::GetMessageDelegate()
{
	return OnMessageReceived;
}

FStructuredMessageDelegate& UGPTHandler::GetStructuredMessageDelegate()
{
	return OnStructuredMessageReceived;
}

FFunctionCallDelegate& UGPTHandler::GetFunctionCalledDelegate()
{
	return OnFunctionCalled;
}

#pragma endregion ILLMService


 void UGPTHandler::SendMessageStructured(UObject* const WorldObject, const TArray<FHttpGPTChatMessage>& Messages,
 	const FHttpGPTStructuredResponse& StructuredResponse)
 {
	bStructuredMessageRequested = true;
 	auto Request = UHttpGPTChatRequest::SendMessagesStructured(WorldObject, Messages, GetFunctions(), StructuredResponse);
 	BindCallbacks(Request);
 	Request->Activate(); 	
 }

void UGPTHandler::SendMessageDefault(UObject* const WorldObject, const TArray<FHttpGPTChatMessage>& Messages)
 {
	bStructuredMessageRequested = false;
 	auto Request = UHttpGPTChatRequest::SendMessages_DefaultOptions(WorldObject, Messages, GetFunctions());
 	BindCallbacks(Request);
 	Request->Activate();
 }

TArray<FHttpGPTFunction> UGPTHandler::GetFunctions()
{
	if(Functions.IsEmpty())
		return {};

	TArray<FHttpGPTFunction> FunctionArray;

	for(auto& Function : Functions)
	{
		FunctionArray.Add(*Function.Value);
	}
	
	return FunctionArray;
}

#pragma region Chat History

void UGPTHandler::AddMessageToHistory(UObject* const WorldObject, const EHttpGPTChatRole Role, const FString& Message)
{
	if(ChatMessages.Contains(WorldObject))
	{
		ChatMessages[WorldObject]->Add({Role, Message});
	}
	else
	{
		TArray<FHttpGPTChatMessage> Messages;
		Messages.Add({Role, Message});
		ChatMessages.Add(WorldObject, MakeShared<TArray<FHttpGPTChatMessage>>(Messages));	
	}
}

TArray<FHttpGPTChatMessage>* UGPTHandler::GetChatHistory(UObject* const WorldObject)
{
	if (!ChatMessages.Contains(WorldObject))
	{
		ChatMessages.Add(WorldObject, MakeShared<TArray<FHttpGPTChatMessage>>());
	}
	
	return ChatMessages[WorldObject].Get();
}

#pragma endregion


#pragma region Callbacks
 void UGPTHandler::OnProcessCompleted(const FHttpGPTChatResponse& Response)
{
	for(auto Choice : Response.Choices)
	{		
		if(Choice.FinishReason.IsEqual("stop", ENameCase::IgnoreCase))
		{
			FString Message = Choice.Message.Content;
			AddMessageToHistory(RequestCaller, EHttpGPTChatRole::Assistant, Message);
			
			if(bStructuredMessageRequested && ProvidedSchemaStruct)
			{
				OnStructuredMessageReceived.Broadcast(RequestCaller, Message, ProvidedSchemaStruct);
			}
			else
			{
				OnMessageReceived.Broadcast(RequestCaller, Message);
			}

		}

		if(Choice.FinishReason.IsEqual("function_call", ENameCase::IgnoreCase))
		{
			FString Message = Choice.Message.Content;
			AddMessageToHistory(RequestCaller, EHttpGPTChatRole::Function, Message);
			
			FHttpGPTFunctionCall FuncCall = Choice.Message.FunctionCall;
			TArray<FString> ArgNames;
			TArray<FString> ArgValues;

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FuncCall.Arguments);

			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				for (auto& Element : JsonObject->Values)
				{
					ArgNames.Add(Element.Key);
					ArgValues.Add(Element.Value->AsString());
				}
			}

			OnFunctionCalled.Broadcast(RequestCaller, Message, FuncCall.Name,ArgNames, ArgValues);
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