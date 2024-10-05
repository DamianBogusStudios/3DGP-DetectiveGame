// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DialogComponent.h"
#include "Interfaces/DialogueHandler.h"
#include "UI/DialogWidget.h"

// Sets default values for this component's properties
UDialogComponent::UDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UDialogComponent::StartDialog()
{
    if (DialogWidgetClass)
    {
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            UDialogWidget* DialogWidget = CreateWidget<UDialogWidget>(PlayerController, DialogWidgetClass);
            if (DialogWidget)
            {
                DialogWidget->AddToViewport();
                
                if(PlayerController->Implements<UDialogueHandler>())
                {
                    IDialogueHandler::Execute_StartDialogue(Cast<UDialogueHandler>(PlayerController), GetOwner(), DialogWidgetClass);
                }

            }
        }
    }
}

