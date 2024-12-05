// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/UISystem.h"
#include "Blueprint/UserWidget.h"
#include "Settings/WidgetClassSettings.h"
#include "Interfaces/WidgetInterface.h"

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

UUserWidget* UUISystem::RequestStartWidget(UObject* InCaller, EUIElementType WidgetType)
{
	if (UUserWidget* Widget = GetWidget(WidgetType))
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			Widget->AddToViewport();
			ActiveUObject = InCaller;
			//OnStartUIAction.Broadcast(ActiveComponent, Widget);

			if(Widget->Implements<UWidgetInterface>())
			{
				IWidgetInterface::Execute_Setup(Widget, ActiveUObject);
			}

			WidgetQueue.Add(WidgetType);
			CheckInputNeeded();
			return Widget;
		}
	}
	return nullptr;
}

bool UUISystem::RequestFinishWidget(UObject* InCaller, EUIElementType WidgetType)
{
	if(/*InCaller == ActiveActor &&*/WidgetMap.Contains(WidgetType))
	{
		UUserWidget* Widget = WidgetMap[WidgetType].WidgetInstance;

		if (Widget && Widget->IsInViewport())
		{
			if(Widget->Implements<UWidgetInterface>())
			{
				IWidgetInterface::Execute_Cleanup(Widget);
			}
			
			Widget->RemoveFromParent();
			ActiveUObject = nullptr;
			//OnFinishUIAction.Broadcast(ActiveComponent, Widget);

			WidgetQueue.Remove(WidgetType);
			CheckInputNeeded();
			return true;
		}
	}
	return false;
}

void UUISystem::Advance()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(WidgetQueue.Num() > 0)
		{
			if(WidgetMap.Contains(WidgetQueue.Last()))
			{
				auto Widget = WidgetMap[WidgetQueue.Last()].WidgetInstance;

				if(Widget && Widget->Implements<UWidgetInterface>())
				{
					IWidgetInterface::Execute_Advance(Widget);
				}
			}
		}
		
	}
}

void UUISystem::Exit()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(WidgetQueue.Num() > 0)
		{
			RequestFinishWidget(PlayerController,WidgetQueue.Last());
		}
	}
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

void UUISystem::CheckInputNeeded() const
{
	RequestInput.Broadcast(WidgetQueue);
}

void UUISystem::LoadWidgetClasses()
{
	if (auto Settings = GetDefault<UWidgetClassSettings>())
	{
		WidgetMap.Add(EUIElementType::NPCDialogue, {Settings->DialogueBox});
		WidgetMap.Add(EUIElementType::LockpickMiniGame, {Settings->LockpickMiniGame});
		WidgetMap.Add(EUIElementType::Loading, {Settings->LoadingGame});
		WidgetMap.Add(EUIElementType::MicFeedback, {Settings->MicFeedback});
		WidgetMap.Add(EUIElementType::Pickup, {Settings->Pickup});
	}
}
	


