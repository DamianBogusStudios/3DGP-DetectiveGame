// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEHttpGPT

#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "Structures/HttpGPTChatTypes.h"
#include "Structures/HttpGPTVoiceTypes.h"
#include "Structures/HttpGPTImageTypes.h"
#include "HttpGPTHelper.generated.h"

/**
 *
 */
UCLASS(NotPlaceable, Category = "HttpGPT")
class HTTPGPTCOMMONMODULE_API UHttpGPTHelper final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Convert HttpGPT Model to Name"))
	static const FName ModelToName(const EHttpGPTChatModel Model);
	
	UFUNCTION(BlueprintPure, Category = "HttpGPT | Voice", meta = (DisplayName = "Convert HttpGPT Model to Name"))
	static const FString ModelToString(const EHttpGPTVoiceModel Model);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Voice", meta = (DisplayName = "Convert HttpGPT Model to Name"))
	static const FString VoiceToString(const EHttpGPTSynthVoice Voice);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Voice", meta = (DisplayName = "Convert HttpGPT Model to Name"))
	static const FString OutputFormatToString(const EHttpGPTVoiceOutputFormat OutputFormat);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Convert Name to HttpGPT Model"))
	static const EHttpGPTChatModel NameToModel(const FName Model);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Convert HttpGPT Role to Name"))
	static const FName RoleToName(const EHttpGPTChatRole Role);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Convert Name to HttpGPT Role"))
	static const EHttpGPTChatRole NameToRole(const FName Role);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Convert HttpGPT Param Type to Name"))
	static const FName PropertyTypeToName(const EHttpGPTPropertyType Type);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Convert Name to HttpGPT Param Type"))
	static const EHttpGPTPropertyType NameToPropertyType(const FName Type);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Convert Type to HttpGPT Param Type"))
	static const EHttpGPTPropertyType CPPToPropertyType(const FString Type);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Get Available GPT Models"))
	static const TArray<FName> GetAvailableGPTModels();

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Get Endpoint for Model"))
	static const FString GetEndpointForModel(const EHttpGPTChatModel Model, const bool bIsAzureOpenAI = false,
	                                         const FString& AzureOpenAIAPIVersion = TEXT("None"));

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Model Supports Chat"))
	static const bool ModelSupportsChat(const EHttpGPTChatModel Model);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Chat", meta = (DisplayName = "Model Supports Structured Response"))
	static const bool ModelSupportsStructuredResponse(const EHttpGPTChatModel Model);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Image", meta = (DisplayName = "Convert HttpGPT Size to Name"))
	static const FName SizeToName(const EHttpGPTImageSize Size);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Image", meta = (DisplayName = "Convert Name to HttpGPT Size"))
	static const EHttpGPTImageSize NameToSize(const FName Size);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Image", meta = (DisplayName = "Convert HttpGPT Format to Name"))
	static const FName FormatToName(const EHttpGPTResponseFormat Format);

	UFUNCTION(BlueprintPure, Category = "HttpGPT | Image", meta = (DisplayName = "Convert Name to HttpGPT Format"))
	static const EHttpGPTResponseFormat NameToFormat(const FName Format);


};
