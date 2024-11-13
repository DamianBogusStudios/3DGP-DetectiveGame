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

	LoadWidgetClasses();
	
#if WITH_EDITOR
	FString Message = FString::Printf(TEXT("UI System Initialised"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif
}

void UUISystem::Deinitialize()
{
	Super::Deinitialize();
}
#pragma endregion

bool UUISystem::RequestStartWidget(AActor* InCaller, EUIElementType WidgetType)
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

bool UUISystem::RequestFinishWidget(AActor* InCaller, EUIElementType WidgetType)
{
	if(/*InCaller == ActiveActor &&*/WidgetMap.Contains(WidgetType))
	{
		UUserWidget* Widget = WidgetMap[WidgetType].WidgetInstance;

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

void UUISystem::OnFinishedLoading()
{
	RequestFinishWidget(GetWorld()->GetFirstPlayerController(), EUIElementType::Loading);
}

UUserWidget* UUISystem::GetWidget(EUIElementType WidgetType)
{
	if(WidgetMap.Contains(WidgetType))
	{
		if(WidgetMap[WidgetType].WidgetInstance)
		{
			return WidgetMap[WidgetType].WidgetInstance;
		}
		else
		{
			if(GetWorld())
			{
				if(GetWorld()->GetFirstPlayerController())
				{
					WidgetMap[WidgetType].WidgetInstance = CreateWidget(GetWorld()->GetFirstPlayerController(), WidgetMap[WidgetType].WidgetClass);
					return WidgetMap[WidgetType].WidgetInstance;
				}
			}
		}
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

void UUISystem::LoadWidgetClasses()
{
	if (auto Settings = GetDefault<UInteractionSettings>())
	{
		WidgetMap.Add(EUIElementType::NPCDialogue, {Settings->DialogueWidgetClass});
		WidgetMap.Add(EUIElementType::LockpickMiniGame, {Settings->LockpickMiniGameClass});
		WidgetMap.Add(EUIElementType::Loading, {Settings->LoadingWidgetClass});
	}
}
	


