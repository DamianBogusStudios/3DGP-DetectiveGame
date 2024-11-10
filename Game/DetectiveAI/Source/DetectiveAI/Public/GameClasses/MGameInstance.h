// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameFinishedLoadingDelegate);

struct FClue;
struct FCaseFile;
struct FActorDescription;
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

	FGameFinishedLoadingDelegate OnGameFinishedLoading;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId OverlayMaterials;

	UFUNCTION(BlueprintCallable)
	UMaterial* GetOverlayMaterial(bool Interactable);
	
	UFUNCTION()
	void OnMaterialsLoaded(FPrimaryAssetId Id);

	UFUNCTION()
	void OnCaseGenerationFinished(double ElapsedTime);

	UFUNCTION()
	void OnActorsSpawned();

	UFUNCTION()
	void CheckAllLoaded();

	
protected:
	// Called when the game starts or when spawned
	virtual void Init() override;

private:

	bool bMaterialsLoaded, bCaseSystemFinished, bActorSpawnerFinished;
	FDelegateHandle OnPostWorldInitializationHandle;

	void OnPostWorldInit(UWorld* World, const UWorld::InitializationValues IVS);


	
};
