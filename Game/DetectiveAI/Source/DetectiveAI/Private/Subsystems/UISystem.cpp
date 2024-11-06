// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/UISystem.h"
#include "Blueprint/UserWidget.h"
#include "Settings/InteractionSettings.h"
#include "Interfaces/WidgetCleanupInterface.h"

#pragma region  Init
bool UUISystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UUISystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
#if WITH_EDITOR
	FString Message = FString::Printf(TEXT("UI Sysytem Initialised"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif

}

void UUISystem::Deinitialize()
{
	Super::Deinitialize();
}
#pragma endregion

bool UUISystem::RequestStartWidget(AActor* InCaller, EWidgetType WidgetType)
{
	if (UUserWidget* Widget = GetWidget(WidgetType))
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			Widget->AddToViewport();
			ActiveActor = InCaller;
			OnStartUIAction.Broadcast(ActiveActor, Widget);
			return true;
		}
	}
	return false;
}

bool UUISystem::RequestFinishWidget(AActor* InCaller, EWidgetType WidgetType)
{
	if(InCaller == ActiveActor && WidgetMap.Contains(WidgetType))
	{
		UUserWidget* Widget = WidgetMap[WidgetType];

		if (Widget && Widget->IsInViewport())
		{
			if(Widget->Implements<UWidgetCleanupInterface>())
			{
				IWidgetCleanupInterface::Execute_Cleanup(Widget);
			}
			
			Widget->RemoveFromParent();
			ActiveActor = nullptr;
			OnFinishUIAction.Broadcast(ActiveActor, Widget);
			return true;
		}
	}
	return false;
}

UUserWidget* UUISystem::GetWidget(EWidgetType WidgetType)
{
	if(WidgetMap.Contains(WidgetType))
	{
		return WidgetMap[WidgetType];
	}
	else if (auto WidgetClass = GetDefault<UInteractionSettings>()->GetWidgetClass(WidgetType))
	{
		auto WidgetInstance = CreateWidget(GetWorld()->GetFirstPlayerController(), WidgetClass);
		WidgetMap.Add(WidgetType, WidgetInstance);
		return WidgetInstance;
	}
	else
	{
#if WITH_EDITOR
		FString Message = FString::Printf(TEXT("DialogueWidget class has not been set"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif
	}

	return nullptr;
}


