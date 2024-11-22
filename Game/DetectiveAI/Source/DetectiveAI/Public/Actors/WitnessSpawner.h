// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonCaseTypes.h"
#include "GameFramework/Actor.h"
#include "WitnessSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorSpawnerFinishedDelegate);


class ABaseNPC;
class USkeletalMesh;

UCLASS()
class DETECTIVEAI_API AWitnessSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWitnessSpawner();

	UPROPERTY(BlueprintAssignable)
	FActorSpawnerFinishedDelegate OnActorsSpawned;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABaseNPC> NPCClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<USkeletalMesh*> MaleMeshes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<USkeletalMesh*> FemaleMeshes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float Spacing = 10.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	TArray<USkeletalMesh*> UsedMeshes;
	
	UFUNCTION()
	void SpawnActors(const TArray<FActorDescription>& Actors);

	USkeletalMesh* GetRandomMesh(EActorGender Gender);
	
};
