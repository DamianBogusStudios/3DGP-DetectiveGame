// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClasses/MGameInstance.h"

#include "Actors/ActorSpawner.h"
#include "Engine/AssetManager.h"
#include "Data/OverlayMaterials.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/CaseSystem.h"

void UMGameInstance::Init()
{
    Super::Init();
    
    OnPostWorldInitializationHandle = FWorldDelegates::OnPostWorldInitialization.AddUObject(this,
        &UMGameInstance::OnPostWorldInit);

    if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
    {
        TArray<FName> Bundles;

        FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UMGameInstance::OnMaterialsLoaded, OverlayMaterials);

        AssetManager->LoadPrimaryAsset(OverlayMaterials, Bundles, Delegate);
    }

    if (auto CaseSys = GetSubsystem<UCaseSystem>())
    {
        CaseSys->OnCaseGenerationFinished.AddDynamic(this, &UMGameInstance::OnCaseGenerationFinished);
    }
    
}

void UMGameInstance::OnPostWorldInit(UWorld* World, const UWorld::InitializationValues IVS)
{
    if (AActorSpawner* ActorSpawner = Cast<AActorSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AActorSpawner::StaticClass())))
    {
        ActorSpawner->OnActorsSpawned.AddDynamic(this, &UMGameInstance::OnActorsSpawned);
    }
}

UMaterial* UMGameInstance::GetOverlayMaterial(bool Interactable)
{
    if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
    {
        if (bMaterialsLoaded)
        {
            UOverlayMaterials* Asset = Cast< UOverlayMaterials>(AssetManager->GetPrimaryAssetObject(OverlayMaterials));

            return Interactable ? Asset->InteractableMat.Get() : Asset->NonInteractableMat.Get();
        }
    }
    return nullptr;
}


void UMGameInstance::OnMaterialsLoaded(FPrimaryAssetId Id)
{
    if(Id == OverlayMaterials)
    {
        bMaterialsLoaded = true;
    }
}

void UMGameInstance::OnCaseGenerationFinished(double ElapsedTime)
{
    bCaseSystemFinished = true;
    
}

void UMGameInstance::OnActorsSpawned()
{
    bActorSpawnerFinished = true;
}

void UMGameInstance::CheckAllLoaded()
{
    if (bCaseSystemFinished && bActorSpawnerFinished)
    {
        OnGameFinishedLoading.Broadcast();
    }
}