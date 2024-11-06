// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/InteractionSystem.h"
#include "Settings/InteractionSettings.h"
#include "UI/DialogueWidget.h"
#include "UI/LockpickMiniGame.h"

bool UInteractionSystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UInteractionSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UInteractionSettings* Settings = GetDefault<UInteractionSettings>())
	{
	//	DialogueWidgetClass = Settings->GetDialogueWidgetClass();
	 //   LockpickMiniGameClass = Settings->GetLockpickMiniGameClass();
	}

#if WITH_EDITOR
	FString Message = FString::Printf(TEXT("InteractionSystem Initialised"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif

}

void UInteractionSystem::Deinitialize()
{
    Super::Deinitialize();
}

void UInteractionSystem::RequestLockpickMiniGame(AActor* Caller)
{
    if (LockpickMiniGameClass)
    {
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            if(!LockpickMiniGameInstance)
            {
                LockpickMiniGameInstance = CreateWidget<ULockpickMiniGame>(PlayerController, LockpickMiniGameClass);
            }
            
            if (LockpickMiniGameInstance)
            {
                LockpickMiniGameInstance->AddToViewport();
            }
        }
    }
    else
    {
#if WITH_EDITOR
        FString Message = FString::Printf(TEXT("MiniGame class has not been set"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif
    }
    
}

void UInteractionSystem::RequestDialogueAction(AActor* Caller)
{
    if (DialogueWidgetClass)
    {
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            if(!DialogueWidgetInstance)
            {
                DialogueWidgetInstance = CreateWidget<UDialogueWidget>(PlayerController, DialogueWidgetClass);
            }
            
            if (DialogueWidgetInstance)
            {
                DialogueWidgetInstance->AddToViewport();
                ActiveActor = Caller;
                OnStartDialogueAction.Broadcast(Caller, DialogueWidgetInstance);
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
}

void UInteractionSystem::AdvanceDialogueAction() const
{
    OnAdvanceDialogueAction.Broadcast(ActiveActor, DialogueWidgetInstance);
}

void UInteractionSystem::FinishDialogueAction(AActor* Caller)
{
    if(Caller == ActiveActor)
    {
        DialogueWidgetInstance->ResetDialogueWidget();
        DialogueWidgetInstance->RemoveFromParent();
        ActiveActor = nullptr;
        OnFinishDialogueAction.Broadcast(Caller, DialogueWidgetInstance);
    }
}
