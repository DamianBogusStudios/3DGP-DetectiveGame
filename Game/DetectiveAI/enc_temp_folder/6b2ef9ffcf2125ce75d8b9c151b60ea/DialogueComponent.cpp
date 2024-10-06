// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DialogueComponent.h"
#include "Subsystems/InteractionSystem.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueComponent::StartDialogue() const
{
    if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
    {
        InteractionSubsystem->RequestDialogueAction(Cast<APawn>(GetOwner()), DialogueTree);
    }
	else
    {

#if WITH_EDITOR
        FString Message = FString::Printf(TEXT("InteractionSystem not initialised"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif

    }


   /* if (DialogueWidgetClass)
    {
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            UDialogueWidget* DialogueWidget = CreateWidget<UDialogueWidget>(PlayerController, DialogueWidgetClass);
            if (DialogueWidget)
            {
                
                if(PlayerController->Implements<UDialogueHandler>())
                {

                    IDialogueHandler::Execute_StartDialogue(PlayerController, GetOwner(), DialogueWidgetClass);

                    DialogueWidget->AddToViewport();
                }
                }

            }
        }
    }*/
}

