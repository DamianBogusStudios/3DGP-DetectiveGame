// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Public/ItemDataProvider.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataManager.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UDataManager : public UGameInstanceSubsystem, public IItemDataProvider
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	TSoftObjectPtr<UDataTable> ItemTable;

	/* ItemDataProver */
	bool GetItemFromTable_Implementation(const FName& RowName, FItemDataRow& OutDataRow) override;


	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	void LoadDataTableAsync(FString DataTablePath, TFunction<void(UDataTable*)> Callback);
};
