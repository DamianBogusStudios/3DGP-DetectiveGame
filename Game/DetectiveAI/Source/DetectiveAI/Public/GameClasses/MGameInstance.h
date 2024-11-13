// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameFinishedLoadingDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProgressUpdatedDelegate,
	const FString&, NextTask,
	float, Progress);

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

	UPROPERTY(BlueprintAssignable)
	FProgressUpdatedDelegate OnGenProgressUpdated;
	
	UPROPERTY(BlueprintAssignable)
	FGameFinishedLoadingDelegate OnGameFinishedLoading;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId OverlayMaterials;

	UFUNCTION(BlueprintCallable)
	UMaterial* GetOverlayMaterial(bool Interactable);
	
	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void BeginInit();
	
protected:
	// Called when the game starts or when spawned
	virtual void Init() override;

	UFUNCTION()
	void OnMaterialsLoaded(FPrimaryAssetId Id);

	UFUNCTION()
	void CaseGenerationProgressCallback(const FString& Msg, float Progress);
	
	UFUNCTION()
	void CaseGenerationFinishedCallback(double ElapsedTime);

	UFUNCTION()
	void OnActorsSpawned();

	UFUNCTION()
	void CheckAllLoaded() const;

	
private:

	bool bMaterialsLoaded, bCaseSystemFinished, bActorSpawnerFinished;
	FDelegateHandle OnPostWorldInitializationHandle;	
};
