// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClasses/MGameInstance.h"

#include "Actors/ActorSpawner.h"
#include "Engine/AssetManager.h"
#include "Data/OverlayMaterials.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/CaseSystem.h"
#include "Subsystems/UISystem.h"

//TODO need to move material code to separate class.

void UMGameInstance::Init()
{
    Super::Init();
    
    if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
    {
        TArray<FName> Bundles;

        FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UMGameInstance::OnMaterialsLoaded, OverlayMaterials);

        AssetManager->LoadPrimaryAsset(OverlayMaterials, Bundles, Delegate);
    }

    if (auto CaseSys = GetSubsystem<UCaseSystem>())
    {
        CaseSys->OnCaseGenerationFinished.AddDynamic(this, &UMGameInstance::CaseGenerationFinishedCallback);
        CaseSys->OnProgressUpdated.AddDynamic(this, &UMGameInstance::CaseGenerationProgressCallback);
    }
}

void UMGameInstance::BeginInit()
{
    if (AActorSpawner* ActorSpawner = Cast<AActorSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AActorSpawner::StaticClass())))
    {
        ActorSpawner->OnActorsSpawned.AddDynamic(this, &UMGameInstance::OnActorsSpawned);
    }

    if(auto UI = GetSubsystem<UUISystem>())
    {
        UI->RequestStartWidget(GetWorld()->GetFirstPlayerController(), EUIElementType::Loading);
    }
}


void UMGameInstance::StartGame()
{
    if (auto CaseSys = GetSubsystem<UCaseSystem>())
    {
        CaseSys->StartCaseGeneration();
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

void UMGameInstance::CaseGenerationProgressCallback(const FString& Msg, float Progress)
{
    OnGenProgressUpdated.Broadcast(Msg, Progress);
}

void UMGameInstance::CaseGenerationFinishedCallback(double ElapsedTime)
{
    bCaseSystemFinished = true;
    CheckAllLoaded();
}

void UMGameInstance::OnActorsSpawned()
{
    bActorSpawnerFinished = true;
    CheckAllLoaded();
}

void UMGameInstance::CheckAllLoaded() const
{
    if (bCaseSystemFinished /*&& bActorSpawnerFinished*/)
    {
        OnGameFinishedLoading.Broadcast();
    }
}