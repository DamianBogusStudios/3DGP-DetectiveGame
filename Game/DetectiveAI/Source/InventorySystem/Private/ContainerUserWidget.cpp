// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerUserWidget.h"

UContainerUserWidget::UContainerUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

//UContainerUserWidget* UContainerUserWidget::CreateContainerWidget(const UObject* WorldContextObject, TSubclassOf<UContainerUserWidget> WidgetClass, TScriptInterface<class InventoryInterface> InInventory)
//{
//    if (!WorldContextObject || !WidgetClass || !InInventory)
//    {
//        return nullptr;
//    }
//
//    UContainerUserWidget* Widget = CreateWidget<UContainerUserWidget>(WorldContextObject->GetWorld(), WidgetClass);
//
//    if (Widget)
//    {
//        Widget->Inventory = InInventory;
//    }
//    return Widget;
//}
