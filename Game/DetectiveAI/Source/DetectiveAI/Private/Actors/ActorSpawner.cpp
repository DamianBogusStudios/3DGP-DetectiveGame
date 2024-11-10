// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ActorSpawner.h"

#include "Characters/BaseNPC.h"
#include "Subsystems/CaseSystem.h"


// Sets default values
AActorSpawner::AActorSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	if(auto CaseSys = GetGameInstance()->GetSubsystem<UCaseSystem>())
	{
		CaseSys->OnActorsGenerated.AddDynamic(this, &AActorSpawner::SpawnActors);
	}
	
}

void AActorSpawner::SpawnActors(const TArray<FActorDescription>& Actors)
{
	if (UWorld* World = GetWorld())
	{
		UsedMeshes.Empty();
		
		for (int i = 0; i < Actors.Num(); i++)
		{
			FVector SpawnLocation = GetActorLocation() + GetActorRightVector() * (i*Spacing);
			FRotator SpawnRotation = FRotator::ZeroRotator;
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			auto Actor = World->SpawnActor<ABaseNPC>(NPCClass, SpawnLocation, SpawnRotation, SpawnParams);

			/* don't have enough character models to be selective with eyecolour,haircolour etc. */
			USkeletalMesh* Mesh = GetRandomMesh(Actors[i].Gender);
			Actor->InitialiseActor(Actors[i], Mesh);
		}

		OnActorsSpawned.Broadcast();
	}
}

USkeletalMesh* AActorSpawner::GetRandomMesh(EActorGender Gender)
{
	TArray<USkeletalMesh*> AvailableMeshes;

	for (auto* Mesh : (Gender == EActorGender::Male) ? MaleMeshes : FemaleMeshes)
	{
		if (!UsedMeshes.Contains(Mesh))
		{
			AvailableMeshes.Add(Mesh);
		}
	}

	if (AvailableMeshes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Meshes have been used. Resetting UsedMeshes."));
		UsedMeshes.Empty();
		AvailableMeshes = (Gender == EActorGender::Male) ? MaleMeshes : FemaleMeshes;
	}

	int32 Index = FMath::RandRange(0, AvailableMeshes.Num() - 1);
	USkeletalMesh* SelectedMesh = AvailableMeshes[Index];
	UsedMeshes.Add(SelectedMesh);
	return SelectedMesh;
}

