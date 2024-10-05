// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OverlayMaterials.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UOverlayMaterials : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UMaterial> InteractableMat;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UMaterial> NonInteractableMat;
};
