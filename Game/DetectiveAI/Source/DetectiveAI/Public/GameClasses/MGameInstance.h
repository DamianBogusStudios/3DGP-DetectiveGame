// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

class ULLMServiceLocator;
class UMaterial;
class UOverlayMaterials;

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId OverlayMaterials;

	UFUNCTION(BlueprintCallable)
	UMaterial* GetOverlayMaterial(bool Interactable);
	
	UFUNCTION()
	void OnMaterialsLoaded(FPrimaryAssetId Id);

protected:
	// Called when the game starts or when spawned
	virtual void Init() override;

private:

	bool bMaterialsLoaded;
};
