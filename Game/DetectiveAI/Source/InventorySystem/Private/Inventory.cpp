// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "ItemDataProvider.h"
#include "ItemDataRow.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UInventory::UInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
	CachedRow = new FItemDataRow();
}


void UInventory::BeginPlay()
{
	Super::BeginPlay();
	InitialiseInventory();
}

void UInventory::InitialiseInventory()
{
	if(UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		auto Subsystems = GameInstance->GetSubsystemArray<UGameInstanceSubsystem>();
	
		for (auto Subsystem : Subsystems)
		{
			if (Subsystem->Implements<UItemDataProvider>())
			{
				DataProvider = TScriptInterface<IItemDataProvider>(Subsystem);
				bInitialised = true;
				OnInventoryInitialised.Broadcast();
				return;
			}
		}
	}
	ThrowMissingDataProviderError();
}

#pragma region InventoryInterface Implementation

bool UInventory::GetAllItems_Implementation(TArray<FName>& OutItemIds, TArray<int32>& OutQuantities)
{
	if (Inventory.Num() > 0)
	{
		Inventory.GenerateKeyArray(OutItemIds);
		Inventory.GenerateValueArray(OutQuantities);
		return true;
	}

	return false;
}

bool UInventory::AddItem_Implementation(const FName& ItemId, int32 Quantity)
{
	if(!DataProvider)
	{
		ThrowMissingDataProviderError();
		return false;
	}


	if (IItemDataProvider::Execute_GetItemFromTable(DataProvider.GetObject(), ItemId, *CachedRow))
	{
		if((Weight + CachedRow->Weight <= Capacity || bAllowOverCapacity))
		{
			Inventory.FindOrAdd(ItemId) += Quantity;
			Weight += CachedRow->Weight;
			return true;
		}
	}

	return false;
}

bool UInventory::RemoveItem_Implementation(const FName& ItemId, int32 Quantity)
{
	if (!DataProvider)
	{
		ThrowMissingDataProviderError();
		return false;
	}

	if (CanRemoveItem(ItemId, Quantity) 
		&& IItemDataProvider::Execute_GetItemFromTable(DataProvider.GetObject(), ItemId, *CachedRow))
	{

		if ((Inventory[ItemId] -= Quantity) <= 0)
			Inventory.Remove(ItemId);

		float RemovedWeight = CachedRow->Weight * Quantity;
		Weight = FMath::Max(Weight - RemovedWeight, 0);

		return true;
	}
	return false;
}


bool UInventory::TransferItem_Implementation(TScriptInterface<IInventoryInterface>& TargetInventory, const FName& ItemId, int32 Quantity)
{
	if (!DataProvider)
	{
		ThrowMissingDataProviderError();
		return false;
	}

	if (CanRemoveItem(ItemId, Quantity) && IInventoryInterface::Execute_AddItem(TargetInventory.GetObject(), ItemId, Quantity))
	{
		RemoveItem(ItemId, Quantity);
		return true;
	}

	return false;
}
#pragma endregion

bool UInventory::CanAddItem(const FName& ItemId, int32 Quantity, float& OutAddedWeight) const
{
	return false;
}

bool UInventory::CanRemoveItem(const FName& ItemId, int32 Quantity) const
{
	return (Inventory.Contains(ItemId) && Inventory[ItemId] >= Quantity);
}

void UInventory::ThrowMissingDataProviderError()
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory: Missing DataProvider!"));
}


/*BACKUP
#pragma region InventoryInterface Implementation

bool UInventory::GetAllItems_Implementation(TArray<FName>& OutItemIds, TArray<int32>& OutQuantities)
{
	if (Inventory.Num() > 0)
	{
		Inventory.GenerateKeyArray(OutItemIds);
		Inventory.GenerateValueArray(OutQuantities);
		return true;
	}

	return false;
}

bool UInventory::AddItem_Implementation(const FName& ItemId, int32 Quantity)
{
	float AddedWeight;

	if (CanAddItem(ItemId, Quantity, AddedWeight))
	{
		Inventory.FindOrAdd(ItemId) += Quantity;
		Weight += AddedWeight;
		return true;
	}

	return false;
}

bool UInventory::RemoveItem_Implementation(const FName& ItemId, int32 Quantity)
{
	Item & enough Quantity is in inventory
	if (CanRemoveItem(ItemId, Quantity))
	{
		UAssetManager& AssetManager = UAssetManager::Get();

		FAssetData AssetData;
		AssetManager.GetPrimaryAssetData(ItemId, AssetData);

		float ItemWeight;

		if (AssetData.GetTagValue<float>("Weight", ItemWeight))
		{
			if ((Inventory[ItemId] -= Quantity) <= 0)
				Inventory.Remove(ItemId);

			float RemovedWeight = ItemWeight * Quantity;
			Weight = FMath::Max(Weight - RemovedWeight, 0);

			return true;
		}
	}

return false;
}

bool UInventory::TransferItem_Implementation(TScriptInterface<IInventoryInterface>& TargetInventory, const FName& ItemId, int32 Quantity)
{
	if (CanRemoveItem(ItemId, Quantity) && TargetInventory->AddItem(ItemId, Quantity))
	{
		RemoveItem(ItemId, Quantity);
		return true;
	}

	return false;
}
#pragma endregion


bool UInventory::CanAddItem(const FName& ItemId, int32 Quantity, float& OutAddedWeight) const
{
	UAssetManager& AssetManager = UAssetManager::Get();

	FAssetData AssetData;
	AssetManager.GetPrimaryAssetData(ItemId, AssetData);

	float ItemWeight;

	if (AssetData.GetTagValue<float>("Weight", ItemWeight))
	{
		OutAddedWeight = ItemWeight * Quantity;
		return (Weight + OutAddedWeight <= Capacity || bAllowOverCapacity);
	}

	return false;
}

bool UInventory::CanRemoveItem(const FName& ItemId, int32 Quantity) const
{
	return (Inventory.Contains(ItemId) && Inventory[ItemId] >= Quantity);
}

*/