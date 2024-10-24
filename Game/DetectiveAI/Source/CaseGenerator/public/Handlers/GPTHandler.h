// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/LLMService.h"
#include "UObject/Object.h"
#include "Types/CommonCaseTypes.h"
#include "GPTHandler.generated.h"

struct FHttpGPTChatResponse;
struct FHttpGPTStructuredResponse;
class UHttpGPTChatRequest;

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UGPTHandler : public UObject, public ILLMService
{
	GENERATED_BODY()

public:
	
	/* LLMService Interface*/
	virtual void SendMessage(UObject* Caller, FString& Message) override;
	virtual void SendStructuredMessage(UObject* const WorldObject, const FString& Message, UScriptStruct* Struct) override;

	virtual FMessageDelegate& GetMessageDelegate() override;
	virtual FStructuredMessageDelegate& GetStructuredMessageDelegate() override;
	/***********************/
	
	// void SendMessagesDefault(const TArray<FHttpGPTChatMessage>& Messages);
	// void SendMessageCustom(const FString& Message, const FHttpGPTCommonOptions& CommonOptions, const FHttpGPTChatOptions& ChatOptions);
	// void SendMessagesCustom(const TArray<FHttpGPTChatMessage>& Messages, const FHttpGPTCommonOptions& CommonOptions, const FHttpGPTChatOptions& ChatOptions);

	/* delegates */
	FMessageDelegate OnMessageReceived;
	FStructuredMessageDelegate OnStructuredMessageReceived;
	
private:

	TObjectPtr<UObject> RequestCaller;
	TObjectPtr<UScriptStruct> ProvidedSchemaStruct;
	bool bStructuredMessageRequested;
	
	void SendMessageDefault(UObject* const WorldObject, const FString& Message);
	void SendMessageStructured(UObject* const WorldObject, const FString& Message, FHttpGPTStructuredResponse StructuredResponse);
	
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





// /**
// 	* Message with Structured Response Specified.
// 	* 
// 	* @param WorldObject The world object ref
// 	* @param Message content.
// 	* @param Name Name Of Structure.
// 	* @param Description A brief description of the structured response.
// 	* 
// 	* @note ONLY USE WITH USTRUCTS
// */
// template<typename StructType>
// FORCEINLINE void SendMessageStructured(UObject* const WorldObject, const FString& Message, const FName& Name, const FString& Description)
// {
// 	if(!TBaseStructure<StructType>::Get())
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("SendMessageStructured can only be used with USTRUCT types."));
// 		return;
// 	}
//
// 	FHttpGPTStructuredResponse StructuredResponse;
//
// 	StructuredResponse.Name = Name;  
// 	StructuredResponse.Description = Description;
//
// 	for(TFieldIterator<UProperty> PropIt(StructType::StaticStruct()); PropIt; ++PropIt)
// 	{
// 		auto Property = *PropIt;
// 		FHttpGPTFunctionProperty HttpProperty;
// 		
// 		HttpProperty.Name = Property->GetFName();
// 		HttpProperty.Description = Property->GetMetaData("Tooltip");
// 		HttpProperty.Type = UHttpGPTHelper::NameToPropertyType(Property->GetClass()->GetFName());
// 		
// 		StructuredResponse.Properties.Add(HttpProperty);
// 		StructuredResponse.RequiredProperties.Add(HttpProperty.Name);
// 	}
// 	/* forcing valid for now */
// 	StructuredResponse.bIsValid = true;
// 	SendMessageStructured(WorldObject, Message, StructuredResponse);
// }
	