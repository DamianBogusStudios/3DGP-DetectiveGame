// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClueSpawner.generated.h"

struct FClue;
enum class ERoom : uint8;
class UBoxComponent;

UCLASS()
class DETECTIVEAI_API AClueSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClueSpawner();

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TMap<TEnumAsByte<ERoom>, AActor*> SpawnVolumes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Templates")
	TSubclassOf<AActor> ActorPrefab;
	
	UFUNCTION()
	void SpawnClues(const TArray<FClue>& Clues);
	
	FVector GetRandomPointInVolume(UBoxComponent* BoxComponent) const;
	
};
