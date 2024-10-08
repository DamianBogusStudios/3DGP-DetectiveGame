// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InteractionSystem.generated.h"


class UDialogueWidget;
class UBehaviorTree;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueActionDelegate, AActor*, Caller, UDialogueWidget*, DialogueWidget);

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

	// UPROPERTY()
	// FRequestDialogueActionDelegate OnRequestDialogueAction;

	UPROPERTY(BlueprintAssignable)
	FDialogueActionDelegate OnStartDialogueAction;
	
	UPROPERTY(BlueprintAssignable)
	FDialogueActionDelegate OnAdvanceDialogueAction;

	UPROPERTY(BlueprintAssignable)
	FDialogueActionDelegate OnFinishDialogueAction;

	UFUNCTION(BlueprintCallable)
	void RequestDialogueAction(AActor* Caller);

	UFUNCTION(BlueprintCallable)
	void AdvanceDialogueAction() const;

	UFUNCTION(BlueprintCallable)
	void FinishDialogueAction(AActor* Caller);
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dialogue)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<AActor> ActiveActor;

	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidgetInstance;	
};
