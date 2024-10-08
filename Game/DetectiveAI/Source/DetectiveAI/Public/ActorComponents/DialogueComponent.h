// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"

class UDialogueWidget;
class UBehaviorTree;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DETECTIVEAI_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI)
	UBehaviorTree* DialogueTree;

	UFUNCTION()
	void StartDialogue() const;

	UFUNCTION()
	void FinishDialogue() const;
	
	UFUNCTION()
	void OnDialogueStarted(AActor* Caller, UDialogueWidget* Widget);

	UFUNCTION()
	void OnAdvanceDialogue(AActor* Caller, UDialogueWidget* Widget);

	UFUNCTION()
	void OnFinishDialogue(AActor* Caller, UDialogueWidget* Widget);

	UFUNCTION(BlueprintCallable)
	FString GetGreeting();
	
protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:

	void BindDialogueDelegates();
	void UnBindDialogueDelegates();

	bool bInDialogue = false;
};
