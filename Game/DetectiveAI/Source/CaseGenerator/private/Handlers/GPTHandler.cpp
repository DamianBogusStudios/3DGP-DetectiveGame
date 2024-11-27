// Fill out your copyright notice in the Description page of Project Settings.
#include "Handlers/GPTHandler.h"
#include "Utils/HttpGPTHelper.h"
#include "Structures/HttpGPTChatTypes.h"
#include "Tasks/HttpGPTChatRequest.h"

/*function forward declarations*/
// FHttpGPTStructuredResponse UStructToStructuredResponse(const UScriptStruct* StructType);
// TSharedPtr<FJsonObject> FunctionPropertyToJson(const FHttpGPTFunctionProperty& Prop);
// TSharedPtr<FJsonObject> StructuredResponseToJson(const FHttpGPTStructuredResponse& StructuredResponse);


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
FString UGPTHandler::GetDescription(const UScriptStruct* InStruct) const
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
FString UGPTHandler::GetDescription(const FProperty& InProperty) const
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
			HttpProperty.InnerType = UHttpGPTHelper::CPPToPropertyType(ArrayProperty->Inner->GetCPPType());

			if (const FStructProperty* StructProperty = CastField<FStructProperty>(ArrayProperty->Inner))
			{
				HttpProperty.InnerTypeSchema = StructProperty->Struct;
			}
			
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
	// else if (const FByteProperty* ByteProperty = CastField<FByteProperty>(&InProperty))
	// {
	// 	if (const UEnum* Enum = ByteProperty->GetIntPropertyEnum())
	// 	{
	// 		for (int32 i = 0; i < Enum->NumEnums() - 1; ++i)
	// 		{
	// 			HttpProperty.Enum.Add(Enum->GetNameByIndex(i));
	// 		}
	// 	}
	// }

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
FHttpGPTStructuredResponse UGPTHandler::UStructToStructuredResponse(const UScriptStruct* StructType, bool Nested)
{
	FHttpGPTStructuredResponse StructuredResponse;
	
	StructuredResponse.Name = StructType->GetFName();  
	StructuredResponse.Description = GetDescription(StructType);
	StructuredResponse.bNestedSchema = Nested;
	
	for(TFieldIterator<FProperty> PropIt(StructType); PropIt; ++PropIt)
	{
		auto Property = *PropIt;
		FHttpGPTFunctionProperty HttpProperty = FPropertyToHttpFunctionProperty(*Property, GetDescription(*Property));
			
		StructuredResponse.Properties.Add(HttpProperty);
		StructuredResponse.RequiredProperties.Add(HttpProperty.Name);
	}
	
	return  StructuredResponse;
}
TSharedPtr<FJsonObject> UGPTHandler::FunctionPropertyToJson(const FHttpGPTFunctionProperty& Prop)
{
	const TSharedPtr<FJsonObject> PropertyJson = MakeShared<FJsonObject>();
		
	PropertyJson->SetStringField("type", UHttpGPTHelper::PropertyTypeToName(Prop.Type).ToString());
	PropertyJson->SetStringField("description", Prop.Description);

	if(Prop.Type == EHttpGPTPropertyType::Array && Prop.InnerType == EHttpGPTPropertyType::Object)
	{
		FHttpGPTStructuredResponse InnerSchema = UStructToStructuredResponse(Prop.InnerTypeSchema, true);
		PropertyJson->SetObjectField("items", StructuredResponseToJson(InnerSchema));
	}
	else if (Prop.Enum.Num() > 0)
	{
		TArray<TSharedPtr<FJsonValue>> EnumValues;
		for (const FName& EnumValue : Prop.Enum)
		{
			EnumValues.Add(MakeShared<FJsonValueString>(EnumValue.ToString()));
		}
		PropertyJson->SetArrayField("enum", EnumValues);
	}
	
	return PropertyJson;
	
}
TSharedPtr<FJsonObject> UGPTHandler::StructuredResponseToJson(const FHttpGPTStructuredResponse& StructuredResponse)
{
	const TSharedPtr<FJsonObject> JsonSchema = MakeShared<FJsonObject>();

		/* json prefix info */
		JsonSchema->SetStringField("name", StructuredResponse.Name.ToString());
		JsonSchema->SetStringField("description", StructuredResponse.Description);
	
	
	/* properties */
	const TSharedPtr<FJsonObject> ParametersJson = MakeShared<FJsonObject>();
	ParametersJson->SetStringField("type", "object");
	
	const TSharedPtr<FJsonObject> PropertiesJson = MakeShared<FJsonObject>();

	for (const FHttpGPTFunctionProperty& Property : StructuredResponse.Properties)
	{
		auto PropertyJson = FunctionPropertyToJson(Property);
		PropertiesJson->SetObjectField(Property.Name.ToString(), PropertyJson);
	}
	
	ParametersJson->SetObjectField("properties", PropertiesJson);

	/* required parameters */
	TArray<TSharedPtr<FJsonValue>> RequiredJsonArray;
	for (const FName& RequiredProperty : StructuredResponse.RequiredProperties)
	{
		RequiredJsonArray.Add(MakeShared<FJsonValueString>(RequiredProperty.ToString()));
	}
	ParametersJson->SetArrayField("required", RequiredJsonArray);


	/* add parameters to final json schema*/
	ParametersJson->SetBoolField("additionalProperties", false);
	ParametersJson->SetBoolField("strict", true);

	if(StructuredResponse.bNestedSchema)
	{
		return ParametersJson;
	}
	
	JsonSchema->SetObjectField("schema", ParametersJson);
	const TSharedPtr<FJsonObject> StructuredResponseJson = MakeShared<FJsonObject>();

	StructuredResponseJson->SetStringField("type", "json_schema");
	StructuredResponseJson->SetObjectField("json_schema", JsonSchema);
	
	return StructuredResponseJson;
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


void UGPTHandler::SendCustomInstructions(UObject* const Caller, const FString& Message)
{
	AddMessageToHistory(Caller, EHttpGPTChatRole::System, Message);
}

void UGPTHandler::SendMessage(UObject* const Caller, const FString& Message, 
				FMessageDelegate Callback, FErrorReceivedDelegate ErrorCallback)
{
	FMessageRequest Request = FMessageRequest(Caller, Message, nullptr);
	Request.OnMessageReceived = Callback;
	AddMessageToHistory(Caller, EHttpGPTChatRole::User, Message);
	AddNewRequest(Request);
}

void UGPTHandler::SendStructuredMessage(UObject* const WorldObject, const FString& Message, UScriptStruct* Struct,
				FStructuredMessageDelegate Callback, FErrorReceivedDelegate ErrorCallback)
{
	FMessageRequest Request = FMessageRequest(WorldObject, Message, Struct);
	Request.OnStructuredMessageReceived = Callback;
	
	AddMessageToHistory(WorldObject, EHttpGPTChatRole::User, Message);
	AddNewRequest(Request);
}

// FMessageDelegate& UGPTHandler::GetMessageDelegate()
// {
// 	return OnMessageReceived;
// }
//
// FStructuredMessageDelegate& UGPTHandler::GetStructuredMessageDelegate()
// {
// 	return OnStructuredMessageReceived;
// }
//
// FFunctionCallDelegate& UGPTHandler::GetFunctionCalledDelegate()
// {
// 	return OnFunctionCalled;
// }

#pragma endregion ILLMService

#pragma region Request Queue Management

void UGPTHandler::AddNewRequest(FMessageRequest& Request)
{
	RequestQueue.Add(Request);
	if(RequestQueue.Num() == 1)
	{
		ProcessNextRequest();
	}
}


void UGPTHandler::ProcessNextRequest()
{
	auto& Request = RequestQueue.Top();

	UObject* Caller = Request.Caller.Get();

	if(Caller == nullptr) //calling object is no longer valid.
	{
		RequestQueue.Pop();
		return;
	}
	
	if(Request.StructSchema == nullptr)
	{
		SendMessageDefault(Caller, *GetChatHistory(Caller));
	}
	else
	{
		auto StructuredResponse = UStructToStructuredResponse(Request.StructSchema);
		auto ResponseSchema = StructuredResponseToJson(StructuredResponse);
		SendMessageStructured(Caller, *GetChatHistory(Caller), ResponseSchema);
	}
	
	Request.StartTime = FPlatformTime::Seconds();
}

#pragma endregion


void UGPTHandler::SendMessageStructured(UObject* const WorldObject, const TArray<FHttpGPTChatMessage>& Messages,
	TSharedPtr<FJsonObject> ResponseSchema)
{
	
	bStructuredMessageRequested = true;
	auto Request = UHttpGPTChatRequest::SendMessagesStructured(WorldObject, Messages, GetFunctions(), ResponseSchema);
	BindCallbacks(Request);
	Request->Activate(); 	
}

/* obsolete */
 // void UGPTHandler::SendMessageStructured(UObject* const WorldObject, const TArray<FHttpGPTChatMessage>& Messages,
 // 	const FHttpGPTStructuredResponse& StructuredResponse)
 // {
	// bStructuredMessageRequested = true;
 // 	auto Request = UHttpGPTChatRequest::SendMessagesStructured(WorldObject, Messages, GetFunctions(), StructuredResponse);
 // 	BindCallbacks(Request);
 // 	Request->Activate(); 	
 // }

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

void UGPTHandler::AddMessageToHistory(TWeakObjectPtr<UObject> WorldObject, const EHttpGPTChatRole Role, const FString& Message)
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

TArray<FHttpGPTChatMessage>* UGPTHandler::GetChatHistory(TWeakObjectPtr<UObject> WorldObject)
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
			auto Request = RequestQueue.Pop();
			
			FString Message = Choice.Message.Content;
			AddMessageToHistory(Request.Caller, EHttpGPTChatRole::Assistant, Message);

			Request.ExecuteCallbacks(Message);
		}

		// if(Choice.FinishReason.IsEqual("function_call", ENameCase::IgnoreCase))
		// {
		// 	FString Message = Choice.Message.Content;
		// 	AddMessageToHistory(RequestCaller, EHttpGPTChatRole::Function, Message);
		// 	
		// 	FHttpGPTFunctionCall FuncCall = Choice.Message.FunctionCall;
		// 	TArray<FString> ArgNames;
		// 	TArray<FString> ArgValues;
		//
		// 	TSharedPtr<FJsonObject> JsonObject;
		// 	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FuncCall.Arguments);
		//
		// 	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		// 	{
		// 		for (auto& Element : JsonObject->Values)
		// 		{
		// 			ArgNames.Add(Element.Key);
		// 			ArgValues.Add(Element.Value->AsString());
		// 		}
		// 	}
		//
		// 	OnFunctionCalled.Broadcast(RequestCaller, Message, FuncCall.Name,ArgNames, ArgValues);
		// }//
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
     UE_LOG(LogTemp, Error, TEXT("Error Received: %s"), *Response.Error.Code.ToString());
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