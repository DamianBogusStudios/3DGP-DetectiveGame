// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()


public:

	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "Item"), Category = "Item", AssetRegistrySearchable)
	FString Name;

	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "Item"), Category = "Item", AssetRegistrySearchable)
	float Weight;

	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "Item"), Category = "Item", AssetRegistrySearchable)
	int32 Value;

	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "UI"), Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "Game"), Category = "Item")
	TSoftClassPtr<AActor> ActorRef;

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Item", GetFName());
	}

protected:
//
//#if WITH_EDITOR
//	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
//#endif
};
