// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InteractionSystem.generated.h"


class UDialogueWidget;
class UBehaviorTree;
class APawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRequestDialogueActionDelegate, APawn*, Caller, UBehaviorTree*, BT, 
	UDialogueWidget*, Widget);

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UInteractionSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY()
	FRequestDialogueActionDelegate OnRequestDialogueAction;

	UFUNCTION(BlueprintCallable)
	void RequestDialogueAction(APawn* Caller, UBehaviorTree* BT) const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
};
