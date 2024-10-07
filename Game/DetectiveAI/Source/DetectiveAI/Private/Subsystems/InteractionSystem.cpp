// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/InteractionSystem.h"
#include "Settings/InteractionSettings.h"
#include "UI/DialogueWidget.h"

bool UInteractionSystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UInteractionSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UInteractionSettings* Settings = GetDefault<UInteractionSettings>())
	{
		DialogueWidgetClass = Settings->GetDialogueWidgetClass();
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

void UInteractionSystem::RequestDialogueAction(AActor* Caller) const
{
    if (DialogueWidgetClass)
    {
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            if (UDialogueWidget* DialogueWidget = CreateWidget<UDialogueWidget>(PlayerController, DialogueWidgetClass))
            {
                DialogueWidget->AddToViewport();

                ActiveActor = Caller;
                DialogueWidgetInstance = DialogueWidget;
                OnStartDialogueAction.Broadcast(Caller, DialogueWidget);
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
//mindreading.
// combine sixaxis motors and triggers.
}

void UInteractionSystem::AdvanceDialogueAction() const
{
    OnAdvanceDialogueAction.Broadcast(ActiveActor, DialogueWidgetInstance);
}

void UInteractionSystem::FinishDialogueAction(AActor* Caller) const
{
    if(Caller == ActiveActor)
    {
        DialogueWidgetInstance->RemoveFromParent();
        OnFinishDialogueAction.Broadcast(ActiveActor, DialogueWidgetInstance);
    }
}
