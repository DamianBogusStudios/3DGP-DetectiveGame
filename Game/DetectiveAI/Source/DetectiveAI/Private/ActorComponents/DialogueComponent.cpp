// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DialogueComponent.h"
#include "Subsystems/InteractionSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UI/DialogueWidget.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UDialogueComponent::BeginPlay()
{
    Super::BeginPlay();
    BindDialogueDelegates();
}

void UDialogueComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnBindDialogueDelegates();
}


void UDialogueComponent::BindDialogueDelegates()
{
    if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
    {
        InteractionSubsystem->OnStartDialogueAction.AddDynamic(this, &UDialogueComponent::OnDialogueStarted);
        InteractionSubsystem->OnAdvanceDialogueAction.AddDynamic(this, &UDialogueComponent::OnAdvanceDialogue);
        InteractionSubsystem->OnFinishDialogueAction.AddDynamic(this, &UDialogueComponent::OnFinishDialogue);
    }
}

void UDialogueComponent::UnBindDialogueDelegates()
{
    if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
    {
        InteractionSubsystem->OnStartDialogueAction.RemoveDynamic(this, &UDialogueComponent::OnDialogueStarted);
        InteractionSubsystem->OnAdvanceDialogueAction.RemoveDynamic(this, &UDialogueComponent::OnAdvanceDialogue);
        InteractionSubsystem->OnFinishDialogueAction.RemoveDynamic(this, &UDialogueComponent::OnFinishDialogue);
    }
}


void UDialogueComponent::StartDialogue() const
{
    if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
    {
        InteractionSubsystem->RequestDialogueAction(GetOwner());
    }
	else
    {
#if WITH_EDITOR
        FString Message = FString::Printf(TEXT("InteractionSystem not initialised"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif

    }
}

void UDialogueComponent::FinishDialogue() const
{
    if(bInDialogue)
    {
        if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
        {
            InteractionSubsystem->FinishDialogueAction(GetOwner());
        }
    }
}


void UDialogueComponent::OnDialogueStarted(AActor* Caller, UDialogueWidget* Widget)
{
    if(Caller == GetOwner())
    {
        bInDialogue = true;
        
        auto Pawn = Cast<APawn>(Caller);
        auto Controller = Pawn->GetController<AAIController>();

        auto BlackboardComponent = Controller->GetBlackboardComponent();
        Controller->RunBehaviorTree(DialogueTree);
        Controller->UseBlackboard(DialogueTree->GetBlackboardAsset(), BlackboardComponent);
       
        if (BlackboardComponent)
        {
            BlackboardComponent->SetValueAsObject("DialogueWidget", Widget);
        }
    }
}
void UDialogueComponent::OnAdvanceDialogue(AActor* Caller, UDialogueWidget* Widget)
{
    if(Caller == GetOwner())
    {
       
    }
}


void UDialogueComponent::OnFinishDialogue(AActor* Caller, UDialogueWidget* Widget)
{
    if(Caller == GetOwner())
    {
        bInDialogue = false;
        
    }
}




