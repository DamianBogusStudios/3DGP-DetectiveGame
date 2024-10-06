// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCController.generated.h"

class AActor;
class UBehaviorTree;
class UDialogueWidget;

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API ANPCController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		

	UFUNCTION()
	void OnDialogueRequested(UBehaviorTree* BT, UDialogueWidget* Widget);

private:

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;



};
