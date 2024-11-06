// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "MetaDataGenerator.generated.h"

/**
 * 
 */
UCLASS()
class CASEGENERATOREDITOR_API UMetaDataGenerator : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetTableData();
	
};
