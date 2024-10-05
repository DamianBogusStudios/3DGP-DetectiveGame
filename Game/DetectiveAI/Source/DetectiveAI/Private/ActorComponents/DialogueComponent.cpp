// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DialogueComponent.h"
#include "Interfaces/DialogueHandler.h"
#include "UI/DialogueWidget.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UDialogueComponent::StartDialogue()
{
    if (DialogueWidgetClass)
    {
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            UDialogueWidget* DialogueWidget = CreateWidget<UDialogueWidget>(PlayerController, DialogueWidgetClass);
            if (DialogueWidget)
            {
                DialogueWidget->AddToViewport();
                
                if(PlayerController->Implements<UDialogueHandler>())
                {
                    IDialogueHandler::Execute_StartDialogue(PlayerController, GetOwner(), DialogueWidgetClass);
                }

            }
        }
    }
}

