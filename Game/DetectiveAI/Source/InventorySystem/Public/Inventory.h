// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryInterface.h"
#include "Delegates/Delegate.h"
#include "Inventory.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryInitialisedDel);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventory : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	bool GetAllItems_Implementation(TArray<FName>& OutItemIds, TArray<int32>& OutQuantities) override;
	bool AddItem_Implementation(const FName& ItemId, int32 Quantity = 1) override;
	bool RemoveItem_Implementation(const FName& ItemId, int32 Quantity = 1) override;
	bool TransferItem_Implementation(TScriptInterface<IInventoryInterface>& TargetInventory, const FName& ItemId, int32 Quantity) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	float Weight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Capacity = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bAllowOverCapacity = false;

	UPROPERTY(BlueprintAssignable)
	FInventoryInitialisedDel OnInventoryInitialised;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TScriptInterface<class IItemDataProvider> DataProvider;
	
	TMap<FName, int32> Inventory;

	struct FItemDataRow* CachedRow;
	
	bool CanAddItem(const FName& ItemId, int32 Quantity, float& OutAddedWeight) const;
	bool CanRemoveItem(const FName& ItemId, int32 Quantity = 1) const;

private:

	bool bInitialised = false;

	void InitialiseInventory();

	void ThrowMissingDataProviderError();

};
