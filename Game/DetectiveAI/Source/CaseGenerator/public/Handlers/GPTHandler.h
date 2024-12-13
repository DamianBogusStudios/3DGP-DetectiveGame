// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/LLMService.h"
#include "Structures/HttpGPTChatTypes.h"
#include "UObject/Object.h"
#include "Types/CommonCaseTypes.h"
#include "GPTHandler.generated.h"

enum class EHttpGPTChatRole : uint8;
struct FHttpGPTFunctionProperty;
struct FHttpGPTFunction;
struct FHttpGPTChatResponse;
struct FHttpGPTStructuredResponse;
struct FHttpGPTChatMessage;
class UHttpGPTChatRequest;
class UDataTable;

USTRUCT()
struct FMessageRequest
{
	GENERATED_BODY()
	
	TWeakObjectPtr<UObject> Caller;
	FString Message;
	UScriptStruct* StructSchema;
	TArray<FHttpGPTFunction> Functions;
	
	FMessageDelegate OnMessageReceived;
	FStructuredMessageDelegate OnStructuredMessageReceived;
	FFunctionCallDelegate OnFunctionCalled;
	FErrorReceivedDelegate OnErrorReceived;

	double StartTime = 0;
	double EndTime = 0;
	
	FMessageRequest(UObject* InCaller, const FString& InMessage, UScriptStruct* InSchemaStruct, TArray<FHttpGPTFunction> InFunctions)
		: Caller(InCaller), Message(InMessage), StructSchema(InSchemaStruct), Functions(InFunctions)
	{
	}

	FMessageRequest()
	{
		Caller = nullptr;
		Message = "";
		StructSchema = nullptr;
	}

	void ExecuteCallbacks(FString& Response)
	{
		EndTime = FPlatformTime::Seconds();
		
		if(StructSchema)
			OnStructuredMessageReceived.ExecuteIfBound(Response, StructSchema);
		else
			OnMessageReceived.ExecuteIfBound(Response);
	}

	void ExecuteFunctionCall(FName& FunctionToCall, TArray<FString>& ArgValues) 
	{
		OnFunctionCalled.ExecuteIfBound(FunctionToCall, ArgValues);
	}

	void ExecuteError()
	{
		OnErrorReceived.ExecuteIfBound(Message, StructSchema);
	}
};

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UGPTHandler : public UObject, public ILLMService
{
	GENERATED_BODY()

public:

	UGPTHandler();

	
	/* LLMService Interface*/
	virtual void SendCustomInstructions(UObject* const Caller, const FString& Message) override;

	virtual void SendMessage( UObject* const Caller, const FString& Message,
				FMessageDelegate Callback, FErrorReceivedDelegate ErrorCallback, FFunctionCallDelegate FunctionCall) override;

	virtual void SendStructuredMessage(UObject* const Caller, const FString& Message, UScriptStruct* StructSchema,
				FStructuredMessageDelegate Callback, FErrorReceivedDelegate ErrorCallback) override;

	virtual void AddFunction(const FName& InName, const FString& InDescription) override;
	virtual void AddFunctionParam(const FName& FuncName, const FName& InName, const FString& InDescription, const FString& InType, const UEnum* InEnum) override;
	
	/***********************/
	
	/* delegates */
	FMessageDelegate OnMessageReceived;
	FStructuredMessageDelegate OnStructuredMessageReceived;
	FFunctionCallDelegate OnFunctionCalled;
	
private:

	UPROPERTY()
	TObjectPtr<UDataTable> MetaDataTable;

	UPROPERTY()
	TArray<FMessageRequest> RequestQueue;

	/*chat log*/
	TMap<TWeakObjectPtr<UObject>, TSharedPtr<TArray<FHttpGPTChatMessage>>> ChatMessages;
	/*function calling*/
	TMap<FName, TUniquePtr<FHttpGPTFunction>> Functions;

	
	void AddNewRequest(FMessageRequest& Request);
	void ProcessNextRequest();
	
	void SendMessageDefault(UObject* const WorldObject, const TArray<FHttpGPTChatMessage>& Messages, TArray<FHttpGPTFunction> Functions);
	void SendMessageStructured(UObject* const WorldObject, const TArray<FHttpGPTChatMessage>& Messages,
		const TSharedPtr<FJsonObject>& StructuredResponseSchema, TArray<FHttpGPTFunction>& Functions);

	/* chat history functions */
	void AddMessageToHistory(TWeakObjectPtr<UObject> WorldObject, const EHttpGPTChatRole Role, const FString& Message);
	TArray<FHttpGPTChatMessage>* GetChatHistory(TWeakObjectPtr<UObject> WorldObject);

	/* helper functions */
	TArray<FHttpGPTFunction> GetFunctions();
	
	static FHttpGPTFunctionProperty FPropertyToHttpFunctionProperty(const FProperty& InProperty, const FString& InDescription);
	static FHttpGPTFunctionProperty FunctionParamToHttpFunctionProperty(const FName& InName, const FString& InDescription, const FString& InType, const UEnum* InEnum);
	FHttpGPTStructuredResponse UStructToStructuredResponse(const UScriptStruct* StructType, bool Nested = false);
	TSharedPtr<FJsonObject> FunctionPropertyToJson(const FHttpGPTFunctionProperty& Prop);
	TSharedPtr<FJsonObject> StructuredResponseToJson(const FHttpGPTStructuredResponse& StructuredResponse);
	
	
	

	FString GetDescription(const FProperty& InProperty) const;
	 FString GetDescription(const UScriptStruct* InStruct) const;
	
	/* callbacks */
	UFUNCTION()
	void OnProcessCompleted(const FHttpGPTChatResponse& Response);
	UFUNCTION()
	void OnProcessUpdated(const FHttpGPTChatResponse& Response);
	UFUNCTION()
	void OnProcessStarted(const FHttpGPTChatResponse& Response);
	UFUNCTION()
	void OnErrorReceived(const FHttpGPTChatResponse& Response);

	void BindCallbacks(UHttpGPTChatRequest* Request);
	
};
