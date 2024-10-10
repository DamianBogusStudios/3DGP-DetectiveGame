// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContainerUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UContainerUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UContainerUserWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<class IInventoryInterface> ViewedInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<class IItemDataProvider> DataProvider;

};
