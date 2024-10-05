// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameInstance.h"
#include "Engine/AssetManager.h"
#include "OverlayMaterials.h"

UMaterial* UMGameInstance::GetOverlayMaterial(bool Interactable)
{
    if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
    {
        if (bMaterialsLoaded)
        {
            UOverlayMaterials* Asset = Cast< UOverlayMaterials>(AssetManager->GetPrimaryAssetObject(OverlayMaterials));

            return Interactable ? Asset->InteractableMat.Get() : Asset->NonInteractableMat.Get();
        }
    }
    return nullptr;
}

void UMGameInstance::Init()
{ 
    if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
    {
        TArray<FName> Bundles;

        FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UMGameInstance::OnMaterialsLoaded, OverlayMaterials);

        AssetManager->LoadPrimaryAsset(OverlayMaterials, Bundles, Delegate);
    }
}

void UMGameInstance::OnMaterialsLoaded(FPrimaryAssetId Id)
{
    if(Id == OverlayMaterials)
    {
        bMaterialsLoaded = true;
    }
}