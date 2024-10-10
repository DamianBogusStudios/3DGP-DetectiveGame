// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
    bool GetAllItems(TArray<FName>& OutItemIds, TArray<int32>& OutQuantities);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
    bool AddItem(const FName& ItemId, int32 Quantity);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
    bool RemoveItem(const FName& ItemId, int32 Quantity);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
    bool TransferItem(TScriptInterface<IInventoryInterface>& TargetInventory, const FName& ItemId, int32 Quantity);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
    bool GetItemData(const FName& ItemId, FString& OutName, float& OutWeight, int32& OutValue) const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
    bool GetItemObject(const FName& ItemId, UPrimaryDataAsset* OutDataAsset) const;

};
