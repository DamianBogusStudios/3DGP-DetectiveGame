// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/GenAIUtilities.h"
#include "JsonObjectConverter.h"

bool UGenAIUtilities::JsonToUStruct(const FString& Content, const UScriptStruct* Schema, void* OutStruct)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		if(FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), Schema, OutStruct, 0, 0))
		{
			return true;
		}
	}

	return false;
}

bool UGenAIUtilities::UStructToJsonObjectString(const UScriptStruct* InStructType, const void* InStruct, FString& OutJsonString)
{
	return FJsonObjectConverter::UStructToJsonObjectString(InStructType, InStruct, OutJsonString);
}
