// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/LLMService.h"
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
	virtual void SendCustomInstructions(UObject* Caller, FString& Message) override;
	virtual void SendMessage(UObject* Caller, FString& Message) override;
	virtual void SendStructuredMessage(UObject* const WorldObject, const FString& Message,  UScriptStruct* Struct) override;
	virtual void AddFunction(const FName& InName, const FString& InDescription) override;
	virtual void AddFunctionParam(const FName& FuncName, const FName& InName, const FString& InDescription, const FString& InType, const UEnum* InEnum) override;
	

	virtual FMessageDelegate& GetMessageDelegate() override;
	virtual FStructuredMessageDelegate& GetStructuredMessageDelegate() override;
	virtual FFunctionCallDelegate& GetFunctionCalledDelegate() override;
	/***********************/
	
	/* delegates */
	FMessageDelegate OnMessageReceived;
	FStructuredMessageDelegate OnStructuredMessageReceived;
	FFunctionCallDelegate OnFunctionCalled;
	
private:

	UPROPERTY()
	TObjectPtr<UDataTable> MetaDataTable;

	UPROPERTY()
	TObjectPtr<UObject> RequestCaller;
	
	UPROPERTY()
	TObjectPtr<UScriptStruct> ProvidedSchemaStruct;
	
	TMap<FName, TUniquePtr<FHttpGPTFunction>> Functions;
	TMap<TObjectPtr<UObject>, TSharedPtr<TArray<FHttpGPTChatMessage>>> ChatMessages;
	
	
	bool bStructuredMessageRequested = false;
	void SendMessageDefault(UObject* const WorldObject, const TArray<FHttpGPTChatMessage>& Messages);
	void SendMessageStructured(UObject* const WorldObject, const TArray<FHttpGPTChatMessage>& Messages, const FHttpGPTStructuredResponse& StructuredResponse);
	void SendMessageStructured(UObject* const WorldObject, const TArray<FHttpGPTChatMessage>& Messages, TSharedPtr<FJsonObject> StructuredResponseSchema);

	/* chat history functions */
	void AddMessageToHistory(UObject* const WorldObject, const EHttpGPTChatRole Role, const FString& Message);
	TArray<FHttpGPTChatMessage>* GetChatHistory(UObject* const WorldObject);

	/* helper functions */
	TArray<FHttpGPTFunction> GetFunctions();
	
	FHttpGPTFunctionProperty FPropertyToHttpFunctionProperty(const FProperty& InProperty, const FString& InDescription);
	FHttpGPTFunctionProperty FunctionParamToHttpFunctionProperty(const FName& InName, const FString& InDescription, const FString& InType, const UEnum* InEnum);
	FHttpGPTStructuredResponse UStructToStructuredResponse(const UScriptStruct* StructType, bool Nested = false);
	TSharedPtr<FJsonObject> FunctionPropertyToJson(const FHttpGPTFunctionProperty& Prop);
	TSharedPtr<FJsonObject> StructuredResponseToJson(const FHttpGPTStructuredResponse& StructuredResponse);
	
	
	

	FString GetDescription(const FProperty& InProperty);
	FString GetDescription(const UScriptStruct* InStruct);
	
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
