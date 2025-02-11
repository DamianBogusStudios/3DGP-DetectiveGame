// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEHttpGPT

#include "Tasks/HttpGPTChatRequest.h"
#include <Utils/HttpGPTHelper.h>
#include <Structures/HttpGPTChatTypes.h>
#include <Management/HttpGPTSettings.h>
#include <HttpGPTInternalFuncs.h>
#include <LogHttpGPT.h>
#include <Interfaces/IHttpRequest.h>
#include <Dom/JsonObject.h>
#include <Serialization/JsonWriter.h>
#include <Serialization/JsonReader.h>
#include <Serialization/JsonSerializer.h>
#include <Misc/ScopeTryLock.h>
#include <Async/Async.h>

#if WITH_EDITOR
#include <Editor.h>
#endif

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(HttpGPTChatRequest)
#endif

#if WITH_EDITOR
UHttpGPTChatRequest* UHttpGPTChatRequest::EditorTask(const TArray<FHttpGPTChatMessage>& Messages, const FHttpGPTChatOptions& Options)
{
	UHttpGPTChatRequest* const NewAsyncTask = SendMessages_CustomOptions(GEditor->GetEditorWorldContext().World(), Messages,
	                                                                     TArray<FHttpGPTFunction>(),
	                                                                     FHttpGPTStructuredResponse(),
	                                                                     FHttpGPTCommonOptions(), Options);
	NewAsyncTask->bIsEditorTask = true;

	return NewAsyncTask;
}
#endif

/* need to handle this on my end and just send through a completed json.*/
/* refactored this section to include structured responses */
UHttpGPTChatRequest* UHttpGPTChatRequest::SendMessagesStructured(UObject* const WorldContextObject, const TArray<FHttpGPTChatMessage>& Messages, const TArray<FHttpGPTFunction>& Functions
																	,TSharedPtr<FJsonObject> ResponseSchema)
{
	return SendMessages_CustomOptions(WorldContextObject, Messages, Functions, ResponseSchema, FHttpGPTCommonOptions(), FHttpGPTChatOptions());
}

UHttpGPTChatRequest* UHttpGPTChatRequest::SendMessagesStructured(UObject* const WorldContextObject, const TArray<FHttpGPTChatMessage>& Messages, const TArray<FHttpGPTFunction>& Functions
																	,const FHttpGPTStructuredResponse StructuredResponse )
{
	return SendMessages_CustomOptions(WorldContextObject, Messages, Functions, StructuredResponse, FHttpGPTCommonOptions(), FHttpGPTChatOptions());
}

// UHttpGPTChatRequest* UHttpGPTChatRequest::SendMessage_DefaultOptions(UObject* const WorldContextObject, const FString& Message,
//                                                                      const TArray<FHttpGPTFunction>& Functions)
// {
// 	return SendMessage_CustomOptions(WorldContextObject, Message, Functions, FHttpGPTStructuredResponse(), FHttpGPTCommonOptions(), FHttpGPTChatOptions());
// }

UHttpGPTChatRequest* UHttpGPTChatRequest::SendMessages_DefaultOptions(UObject* const WorldContextObject, const TArray<FHttpGPTChatMessage>& Messages,
                                                                      const TArray<FHttpGPTFunction>& Functions)
{
	return SendMessages_CustomOptions(WorldContextObject, Messages, Functions, nullptr, FHttpGPTCommonOptions(), FHttpGPTChatOptions());
}

UHttpGPTChatRequest* UHttpGPTChatRequest::SendMessage_CustomOptions(UObject* const WorldContextObject, const FString& Message,
                                                                    const TArray<FHttpGPTFunction>& Functions,
                                                                    const FHttpGPTStructuredResponse StructuredResponse,
                                                                    const FHttpGPTCommonOptions CommonOptions,
                                                                    const FHttpGPTChatOptions ChatOptions)
{
	return SendMessages_CustomOptions(WorldContextObject, {FHttpGPTChatMessage(EHttpGPTChatRole::User, Message)}, Functions, StructuredResponse, CommonOptions,
	                                  ChatOptions);
}

// UHttpGPTChatRequest* UHttpGPTChatRequest::SendMessages_CustomOptions(UObject* const WorldContextObject, const TArray<FHttpGPTChatMessage>& Messages,
//                                                                      const TArray<FHttpGPTFunction>& Functions,
//                                                                      const FHttpGPTCommonOptions CommonOptions, const FHttpGPTChatOptions ChatOptions)
// {
// 	UHttpGPTChatRequest* const NewAsyncTask = NewObject<UHttpGPTChatRequest>();
// 	NewAsyncTask->Messages = Messages;
// 	NewAsyncTask->CommonOptions = CommonOptions;
// 	NewAsyncTask->ChatOptions = ChatOptions;
// 	NewAsyncTask->Functions = Functions;
//
// 	NewAsyncTask->RegisterWithGameInstance(WorldContextObject);
//
// 	return NewAsyncTask;
// }

UHttpGPTChatRequest* UHttpGPTChatRequest::SendMessages_CustomOptions(UObject* const WorldContextObject, const TArray<FHttpGPTChatMessage>& Messages,
																	 const TArray<FHttpGPTFunction>& Functions, const FHttpGPTStructuredResponse StructuredResponse,
																	 const FHttpGPTCommonOptions CommonOptions, const FHttpGPTChatOptions ChatOptions)
{
	UHttpGPTChatRequest* const NewAsyncTask = NewObject<UHttpGPTChatRequest>();
	NewAsyncTask->Messages = Messages;
	NewAsyncTask->CommonOptions = CommonOptions;
	NewAsyncTask->ChatOptions = ChatOptions;
	NewAsyncTask->Functions = Functions;
	NewAsyncTask->StructuredResponse = StructuredResponse;

	NewAsyncTask->RegisterWithGameInstance(WorldContextObject);

	return NewAsyncTask;
}


UHttpGPTChatRequest* UHttpGPTChatRequest::SendMessages_CustomOptions(UObject* const WorldContextObject, const TArray<FHttpGPTChatMessage>& Messages,
																	 const TArray<FHttpGPTFunction>& Functions, TSharedPtr<FJsonObject> ResponseSchema,
																	 const FHttpGPTCommonOptions CommonOptions, const FHttpGPTChatOptions ChatOptions)
{
	UHttpGPTChatRequest* const NewAsyncTask = NewObject<UHttpGPTChatRequest>();
	NewAsyncTask->Messages = Messages;
	NewAsyncTask->CommonOptions = CommonOptions;
	NewAsyncTask->ChatOptions = ChatOptions;
	NewAsyncTask->Functions = Functions;
	NewAsyncTask->ResponseSchema = ResponseSchema;

	NewAsyncTask->RegisterWithGameInstance(WorldContextObject);

	return NewAsyncTask;
}


/***********************************************************/


bool UHttpGPTChatRequest::CanActivateTask() const
{
	if (!Super::CanActivateTask())
	{
		return false;
	}

	if (HttpGPT::Internal::HasEmptyParam(Messages))
	{
		UE_LOG(LogHttpGPT, Error, TEXT("%s (%d): Can't activate task: Invalid Messages."), *FString(__FUNCTION__), GetUniqueID());
		return false;
	}

	return true;
}

bool UHttpGPTChatRequest::CanBindProgress() const
{
	return GetChatOptions().bStream;
}

FString UHttpGPTChatRequest::GetEndpointURL() const
{
	return FString::Format(TEXT("{0}/{1}"), {
		                       GetCommonOptions().Endpoint,
		                       UHttpGPTHelper::GetEndpointForModel(GetChatOptions().Model, GetCommonOptions().bIsAzureOpenAI,
		                                                           GetCommonOptions().AzureOpenAIAPIVersion)
	                       });
}

const FHttpGPTChatOptions UHttpGPTChatRequest::GetChatOptions() const
{
	return ChatOptions;
}

FString UHttpGPTChatRequest::SetRequestContent()
{
	FScopeLock Lock(&Mutex);

	if (!HttpRequest.IsValid())
	{
		return FString();
	}

	UE_LOG(LogHttpGPT_Internal, Display, TEXT("%s (%d): Mounting content"), *FString(__FUNCTION__), GetUniqueID());

	const TSharedPtr<FJsonObject> JsonRequest = MakeShared<FJsonObject>();
	JsonRequest->SetStringField("model", UHttpGPTHelper::ModelToName(GetChatOptions().Model).ToString().ToLower());
	JsonRequest->SetNumberField("max_tokens", GetChatOptions().MaxTokens);
	JsonRequest->SetNumberField("temperature", GetChatOptions().Temperature);
	JsonRequest->SetNumberField("top_p", GetChatOptions().TopP);
	JsonRequest->SetNumberField("n", GetChatOptions().Choices);
	JsonRequest->SetNumberField("presence_penalty", GetChatOptions().PresencePenalty);
	JsonRequest->SetNumberField("frequency_penalty", GetChatOptions().FrequencyPenalty);
	JsonRequest->SetBoolField("stream", GetChatOptions().bStream);

	if (!HttpGPT::Internal::HasEmptyParam(GetCommonOptions().User))
	{
		JsonRequest->SetStringField("user", GetCommonOptions().User.ToString());
	}

	if (!HttpGPT::Internal::HasEmptyParam(GetChatOptions().Stop))
	{
		TArray<TSharedPtr<FJsonValue>> StopJson;
		for (const FName& Iterator : GetChatOptions().Stop)
		{
			StopJson.Add(MakeShared<FJsonValueString>(Iterator.ToString()));
		}

		JsonRequest->SetArrayField("stop", StopJson);
	}

	if (!HttpGPT::Internal::HasEmptyParam(GetChatOptions().LogitBias))
	{
		const TSharedPtr<FJsonObject> LogitBiasJson = MakeShared<FJsonObject>();
		for (const TPair<int32, float>& Iterator : GetChatOptions().LogitBias)
		{
			LogitBiasJson->SetNumberField(FString::FromInt(Iterator.Key), Iterator.Value);
		}

		JsonRequest->SetObjectField("logit_bias", LogitBiasJson);
	}

	if (UHttpGPTHelper::ModelSupportsChat(GetChatOptions().Model))
	{
		UE_LOG(LogHttpGPT_Internal, Display, TEXT("%s (%d): Selected model supports Chat API. Mounting section history."), *FString(__FUNCTION__),
		       GetUniqueID());

		TArray<TSharedPtr<FJsonValue>> MessagesJson;
		for (const FHttpGPTChatMessage& Iterator : Messages)
		{
			MessagesJson.Add(Iterator.GetMessage());
		}

		JsonRequest->SetArrayField("messages", MessagesJson);

		if (!Functions.IsEmpty())
		{
			TArray<TSharedPtr<FJsonValue>> FunctionsJson;
			for (const FHttpGPTFunction& Iterator : Functions)
			{
				FunctionsJson.Add(Iterator.GetFunction());
			}

			JsonRequest->SetArrayField("functions", FunctionsJson);
		}
		
		if(UHttpGPTHelper::ModelSupportsStructuredResponse(GetChatOptions().Model) && ResponseSchema.IsValid() /* check if response schema is valid by checking if its empty*/)
		{
			JsonRequest->SetObjectField("response_format", ResponseSchema);
		}
	}
	else
	{
		UE_LOG(LogHttpGPT_Internal, Display, TEXT("%s (%d): Selected model does not supports Chat API. Using last message as prompt content."),
		       *FString(__FUNCTION__), GetUniqueID());
		JsonRequest->SetStringField("prompt", Messages.Top().Content);
	}
	

	FString RequestContentString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestContentString);
	FJsonSerializer::Serialize(JsonRequest.ToSharedRef(), Writer);

	HttpRequest->SetContentAsString(RequestContentString);

	return RequestContentString;
}



TSharedPtr<FJsonObject> UHttpGPTChatRequest::SerializeStructuredResponse() const
{
	return nullptr;
	//const TSharedPtr<FJsonObject> JsonSchema = MakeShared<FJsonObject>();

	// /* json prefix info */
	// JsonSchema->SetStringField("name", StructuredResponse.Name.ToString());
	// JsonSchema->SetStringField("description", StructuredResponse.Description);
	//
	// /* properties */
	// const TSharedPtr<FJsonObject> ParametersJson = MakeShared<FJsonObject>();
	// ParametersJson->SetStringField("type", "object");
	//
	// const TSharedPtr<FJsonObject> PropertiesJson = MakeShared<FJsonObject>();
	//
	// for (const FHttpGPTFunctionProperty& Property : StructuredResponse.Properties)
	// {
	// 	auto PropertyJson = Property.ToJson();
	// 	PropertiesJson->SetObjectField(Property.Name.ToString(), PropertyJson);
	// }
	//
	// ParametersJson->SetObjectField("properties", PropertiesJson);
	//
	//
	// /* required parameters */
	//
	// TArray<TSharedPtr<FJsonValue>> RequiredJsonArray;
	// for (const FName& RequiredProperty : StructuredResponse.RequiredProperties)
	// {
	// 	RequiredJsonArray.Add(MakeShared<FJsonValueString>(RequiredProperty.ToString()));
	// }
	// ParametersJson->SetArrayField("required", RequiredJsonArray);
	//
	//
	// /* add parameters to final json schema*/
	// JsonSchema->SetObjectField("schema", ParametersJson);
	// ParametersJson->SetBoolField("additionalProperties", false);
	// ParametersJson->SetBoolField("strict", true);
	//
	// const TSharedPtr<FJsonObject> StructuredResponseJson = MakeShared<FJsonObject>();
	//
	// StructuredResponseJson->SetStringField("type", "json_schema");
	// StructuredResponseJson->SetObjectField("json_schema", JsonSchema);
	
	//return StructuredResponseJson;
}


void UHttpGPTChatRequest::OnProgressUpdated(const FString& Content, int32 BytesSent, int32 BytesReceived)
{
	FScopeLock Lock(&Mutex);

	if (HttpGPT::Internal::HasEmptyParam(Content))
	{
		return;
	}

	TArray<FString> Deltas = GetDeltasFromContent(Content);

	UE_LOG(LogHttpGPT_Internal, Display, TEXT("%s (%d): Progress Updated"), *FString(__FUNCTION__), GetUniqueID());
	UE_LOG(LogHttpGPT_Internal, Display, TEXT("%s (%d): Content: %s; Bytes Sent: %d; Bytes Received: %d"), *FString(__FUNCTION__), GetUniqueID(),
	       *Deltas.Top(), BytesSent, BytesReceived);

	DeserializeStreamedResponse(Deltas);

	if (!Response.bSuccess)
	{
		return;
	}

	if (!bInitialized)
	{
		bInitialized = true;

		AsyncTask(ENamedThreads::GameThread, [this]
		{
			FScopeLock Lock(&Mutex);
			ProgressStarted.Broadcast(Response);
		});
	}

	AsyncTask(ENamedThreads::GameThread, [this]
	{
		const FScopeTryLock Lock(&Mutex);

		if (Lock.IsLocked())
		{
			ProgressUpdated.Broadcast(Response);
		}
	});
}

void UHttpGPTChatRequest::OnProgressCompleted(const FString& Content, const bool bWasSuccessful)
{
	FScopeLock Lock(&Mutex);

	if (!bWasSuccessful || HttpGPT::Internal::HasEmptyParam(Content))
	{
		UE_LOG(LogHttpGPT, Error, TEXT("%s (%d): Request failed"), *FString(__FUNCTION__), GetUniqueID());
		AsyncTask(ENamedThreads::GameThread, [this]
		{
			RequestFailed.Broadcast();
		});

		return;
	}

	UE_LOG(LogHttpGPT_Internal, Display, TEXT("%s (%d): Process Completed"), *FString(__FUNCTION__), GetUniqueID());
	UE_LOG(LogHttpGPT_Internal, Display, TEXT("%s (%d): Content: %s"), *FString(__FUNCTION__), GetUniqueID(), *Content);

	if (!GetChatOptions().bStream)
	{
		DeserializeSingleResponse(Content);
	}
	else
	{
		const TArray<FString> Deltas = GetDeltasFromContent(Content);
		DeserializeStreamedResponse(Deltas);
	}

	if (Response.bSuccess)
	{
		AsyncTask(ENamedThreads::GameThread, [this]
		{
			FScopeLock Lock(&Mutex);

			if (!GetChatOptions().bStream)
			{
				ProgressStarted.Broadcast(Response);
			}

			ProcessCompleted.Broadcast(Response);
		});
	}
	else
	{
		UE_LOG(LogHttpGPT, Error, TEXT("%s (%d): Request failed"), *FString(__FUNCTION__), GetUniqueID());
		AsyncTask(ENamedThreads::GameThread, [this]
		{
			FScopeLock Lock(&Mutex);
			ErrorReceived.Broadcast(Response);
		});
	}
}

TArray<FString> UHttpGPTChatRequest::GetDeltasFromContent(const FString& Content) const
{
	TArray<FString> Deltas;
	Content.ParseIntoArray(Deltas, TEXT("data: "));

	if (Deltas.Top().Contains("[done]", ESearchCase::IgnoreCase))
	{
		Deltas.Pop();
	}

	if (HttpGPT::Internal::HasEmptyParam(Deltas))
	{
		Deltas.Add(Content);
	}

	return Deltas;
}

void UHttpGPTChatRequest::DeserializeStreamedResponse(const TArray<FString>& Deltas)
{
	FScopeLock Lock(&Mutex);

	Response.Choices.Empty(Deltas.Num());
	for (const FString& Delta : Deltas)
	{
		DeserializeSingleResponse(Delta);
	}
}

void UHttpGPTChatRequest::DeserializeSingleResponse(const FString& Content)
{
	FScopeLock Lock(&Mutex);

	if (HttpGPT::Internal::HasEmptyParam(Content))
	{
		return;
	}

	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
	TSharedPtr<FJsonObject> JsonResponse = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(Reader, JsonResponse);

	if (CheckError(JsonResponse, Response.Error))
	{
		Response.bSuccess = false;
		return;
	}

	Response.bSuccess = true;

	Response.ID = *JsonResponse->GetStringField(TEXT("id"));
	Response.Object = *JsonResponse->GetStringField(TEXT("object"));
	Response.Created = JsonResponse->GetNumberField(TEXT("created"));

	const TArray<TSharedPtr<FJsonValue>> ChoicesArr = JsonResponse->GetArrayField(TEXT("choices"));

	for (auto Iterator = ChoicesArr.CreateConstIterator(); Iterator; ++Iterator)
	{
		const TSharedPtr<FJsonObject> ChoiceObj = (*Iterator)->AsObject();
		const int32 ChoiceIndex = ChoiceObj->GetIntegerField(TEXT("index"));

		FHttpGPTChatChoice* Choice = Response.Choices.FindByPredicate([this, ChoiceIndex](const FHttpGPTChatChoice& Element)
		{
			return Element.Index == ChoiceIndex;
		});

		if (!Choice)
		{
			FHttpGPTChatChoice NewChoice;
			NewChoice.Index = ChoiceIndex;
			Choice = &Response.Choices.Add_GetRef(NewChoice);
		}

		if (const TSharedPtr<FJsonObject>* MessageObj; ChoiceObj->TryGetObjectField(TEXT("message"), MessageObj))
		{
			if (FString RoleStr; (*MessageObj)->TryGetStringField(TEXT("role"), RoleStr))
			{
				Choice->Message.Role = RoleStr == "user" ? EHttpGPTChatRole::User : EHttpGPTChatRole::Assistant;
			}

			if (FString ContentStr; (*MessageObj)->TryGetStringField(TEXT("content"), ContentStr))
			{
				Choice->Message.Content = ContentStr;
			}

			if (const TSharedPtr<FJsonObject>* FunctionObj; (*MessageObj)->TryGetObjectField(TEXT("function_call"), FunctionObj))
			{
				if (FString FunctionNameStr; (*FunctionObj)->TryGetStringField(TEXT("name"), FunctionNameStr))
				{
					Choice->Message.FunctionCall.Name = *FunctionNameStr;
				}
				if (FString FunctionArgumentsStr; (*FunctionObj)->TryGetStringField(TEXT("arguments"), FunctionArgumentsStr))
				{
					Choice->Message.FunctionCall.Arguments = FunctionArgumentsStr;
				}
			}
		}
		else if (const TSharedPtr<FJsonObject>* DeltaObj; ChoiceObj->TryGetObjectField(TEXT("delta"), DeltaObj))
		{
			if (FString RoleStr; (*DeltaObj)->TryGetStringField(TEXT("role"), RoleStr))
			{
				Choice->Message.Role = UHttpGPTHelper::NameToRole(*RoleStr);
			}
			else if (FString ContentStr; (*DeltaObj)->TryGetStringField(TEXT("content"), ContentStr))
			{
				Choice->Message.Content += ContentStr;
			}

			if (const TSharedPtr<FJsonObject>* FunctionObj; (*DeltaObj)->TryGetObjectField(TEXT("function_call"), FunctionObj))
			{
				if (FString FunctionNameStr; (*FunctionObj)->TryGetStringField(TEXT("name"), FunctionNameStr))
				{
					Choice->Message.FunctionCall.Name = *FunctionNameStr;
				}
				if (FString FunctionArgumentsStr; (*FunctionObj)->TryGetStringField(TEXT("arguments"), FunctionArgumentsStr))
				{
					Choice->Message.FunctionCall.Arguments += FunctionArgumentsStr;
				}
			}
		}
		else if (FString MessageText; ChoiceObj->TryGetStringField(TEXT("text"), MessageText))
		{
			Choice->Message.Role = EHttpGPTChatRole::Assistant;
			Choice->Message.Content += MessageText;
		}

		while (Choice->Message.Content.StartsWith("\n"))
		{
			Choice->Message.Content.RemoveAt(0);
		}

		if (FString FinishReasonStr; ChoiceObj->TryGetStringField(TEXT("finish_reason"), FinishReasonStr))
		{
			Choice->FinishReason = *FinishReasonStr;
		}
	}

	if (const TSharedPtr<FJsonObject>* UsageObj; JsonResponse->TryGetObjectField(TEXT("usage"), UsageObj))
	{
		Response.Usage = FHttpGPTChatUsage((*UsageObj)->GetNumberField(TEXT("prompt_tokens")), (*UsageObj)->GetNumberField(TEXT("completion_tokens")),
		                                   (*UsageObj)->GetNumberField(TEXT("total_tokens")));
	}
}

UHttpGPTChatRequest* UHttpGPTChatHelper::CastToHttpGPTChatRequest(UObject* const Object)
{
	return Cast<UHttpGPTChatRequest>(Object);
}
