// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GenAIUtilities.generated.h"

/**
 * 
 */
UCLASS(NotPlaceable)
class CASEGENERATOR_API UGenAIUtilities final : public UObject
{
	GENERATED_BODY()
	
public:
	
	static bool JsonToUStruct(const FString& Content, const UScriptStruct* Schema, void* OutStruct);

	static bool UStructToJsonObjectString(const UScriptStruct* InStructType, const void* InStruct, FString& OutJsonString);

	
	
};
