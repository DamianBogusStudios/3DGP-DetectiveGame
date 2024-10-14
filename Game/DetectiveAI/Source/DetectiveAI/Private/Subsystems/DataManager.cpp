// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DataManager.h"
#include "Engine/AssetManager.h"
#include "Settings/DataManagerSettings.h"

bool UDataManager::GetItemFromTable_Implementation(const FName& RowName, FItemDataRow& OutDataRow)
{
	if (ItemTable != nullptr)
	{
		FItemDataRow* Row = ItemTable->FindRow<FItemDataRow>(RowName, TEXT("GetItemFromTable"));

		if (Row)
		{
			OutDataRow = *Row;
			return true;
		}
	}

	return false;
}

void UDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(const UDataManagerSettings* Settings = GetDefault<UDataManagerSettings>())
	{
		ItemTable = Settings->ItemTable.LoadSynchronous();
	}

	FString Message = FString::Printf(TEXT("DataManager Initialised"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
}

void UDataManager::Deinitialize()
{
	FString Message = FString::Printf(TEXT("DataManager Deinitialised"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
}

void UDataManager::LoadDataTableAsync(FString DataTablePath, TFunction<void(UDataTable*)> Callback)
{
	FSoftObjectPath AssetPath(DataTablePath);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetPath,
		FStreamableDelegate::CreateLambda([=]()
			{
				UDataTable* LoadedTable = Cast<UDataTable>(AssetPath.ResolveObject());
				Callback(LoadedTable);
			}));
}