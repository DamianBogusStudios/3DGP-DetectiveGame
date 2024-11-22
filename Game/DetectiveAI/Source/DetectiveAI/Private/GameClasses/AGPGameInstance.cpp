// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClasses/AGPGameInstance.h"

#include "Actors/WitnessSpawner.h"
#include "Engine/AssetManager.h"
#include "Data/OverlayMaterials.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/CaseSystem.h"
#include "Subsystems/UISystem.h"

//TODO need to move material code to separate class.

void UAGPGameInstance::Init()
{
    Super::Init();
    
    if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
    {
        TArray<FName> Bundles;

        FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UAGPGameInstance::OnMaterialsLoaded, OverlayMaterials);

        AssetManager->LoadPrimaryAsset(OverlayMaterials, Bundles, Delegate);
    }

    if (auto CaseSys = GetSubsystem<UCaseSystem>())
    {
        CaseSys->OnCaseGenerationFinished.AddDynamic(this, &UAGPGameInstance::CaseGenerationFinishedCallback);
        CaseSys->OnProgressUpdated.AddDynamic(this, &UAGPGameInstance::CaseGenerationProgressCallback);
    }

}

void UAGPGameInstance::BeginInit()
{
    if (AWitnessSpawner* ActorSpawner = Cast<AWitnessSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AWitnessSpawner::StaticClass())))
    {
        ActorSpawner->OnActorsSpawned.AddDynamic(this, &UAGPGameInstance::OnActorsSpawned);
    }

    if(auto UI = GetSubsystem<UUISystem>())
    {
        UI->RequestStartWidget(GetWorld()->GetFirstPlayerController(), EUIElementType::Loading);
    }
}


void UAGPGameInstance::StartGame()
{
    if (auto CaseSys = GetSubsystem<UCaseSystem>())
    {
        CaseSys->StartCaseGeneration();
    }
}


UMaterial* UAGPGameInstance::GetOverlayMaterial(bool Interactable)
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

void UAGPGameInstance::OnMaterialsLoaded(FPrimaryAssetId Id)
{
    if(Id == OverlayMaterials)
    {
        bMaterialsLoaded = true;
    }
}

void UAGPGameInstance::CaseGenerationProgressCallback(const FString& Msg, float Progress)
{
    OnGenProgressUpdated.Broadcast(Msg, Progress);
}

void UAGPGameInstance::CaseGenerationFinishedCallback(double ElapsedTime)
{
    bCaseSystemFinished = true;
    CheckAllLoaded();
}

void UAGPGameInstance::OnActorsSpawned()
{
    bActorSpawnerFinished = true;
    CheckAllLoaded();
}

void UAGPGameInstance::CheckAllLoaded() const
{
    if (bCaseSystemFinished /*&& bActorSpawnerFinished*/)
    {
        OnGameFinishedLoading.Broadcast();
    }
}