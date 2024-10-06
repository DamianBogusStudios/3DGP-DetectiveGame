// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/NPCController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Subsystems/InteractionSystem.h"
#include "UI/DialogueWidget.h"



void ANPCController::OnPossess(APawn* InPawn)
{

#if WITH_EDITOR
	FString Message = FString::Printf(TEXT("AI Controller Possessed NPC"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif
}

void ANPCController::OnUnPossess()
{
}

void ANPCController::BeginPlay()
{
	Super::BeginPlay();

	if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
	{
		InteractionSubsystem->OnRequestDialogueAction.AddDynamic(this, &ANPCController::OnDialogueRequested);
	}

}
void ANPCController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);


	if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
	{
		InteractionSubsystem->OnRequestDialogueAction.RemoveDynamic(this, &ANPCController::OnDialogueRequested);
	}
}


void ANPCController::OnDialogueRequested(APawn* Caller, UBehaviorTree* BT, UDialogueWidget* Widget)
{
	if (Caller == GetPawn())
	{
		RunBehaviorTree(BT);
		UseBlackboard(BT->GetBlackboardAsset(), BlackboardComponent);

		if(BlackboardComponent)
		{
			BlackboardComponent->SetValueAsObject("DialogueWidget", Widget);
		}
	}
}
