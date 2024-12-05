// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ClueSpawner.h"

#include "Actors/CluePickup.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Subsystems/CaseSystem.h"
#include "Types/CommonCaseTypes.h"


AClueSpawner::AClueSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	for (auto i = 0; i < static_cast<int>(ERoom::MAX); i++)
	{
		SpawnVolumes.Add(static_cast<ERoom>(i), nullptr);
	}
}

void AClueSpawner::BeginPlay()
{
	Super::BeginPlay();

	if(auto CaseSys = GetGameInstance()->GetSubsystem<UCaseSystem>())
	{
		CaseSys->OnCluesGenerated.AddDynamic(this, &AClueSpawner::SpawnClues);
	}
}


void AClueSpawner::SpawnClues(const TArray<FClue>& Clues)
{
	for (auto& Clue : Clues)
	{
		if (auto& SpawnZone = SpawnVolumes[Clue.Room])
		{
			FVector SpawnLocation = GetRandomPointInVolume(SpawnZone->GetComponentByClass<UBoxComponent>());
			SpawnLocation.Z = 80;
			
			if (ACluePickup* Pickup = GetWorld()->SpawnActor<ACluePickup>(ActorPrefab, SpawnLocation, FRotator::ZeroRotator))
			{
				Pickup->Clue = Clue;
			}
		}
	}
}

FVector AClueSpawner::GetRandomPointInVolume(UBoxComponent* RoomVolume) const
{
	if (RoomVolume)
	{
		const auto& Bounds = RoomVolume->Bounds;
		return UKismetMathLibrary::RandomPointInBoundingBox(Bounds.Origin, Bounds.BoxExtent);
	}
	return FVector::ZeroVector;
}
