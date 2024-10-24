// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/CommonCaseTypes.h"
#include "DialogueComponent.generated.h"


class IDialogueProvider;
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
	TObjectPtr<UBehaviorTree> DialogueTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	FActorDescription ActorDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	FDialogueOptions DialogueOptions;
	
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

	// UFUNCTION()
	// void OnMessageReceived(TObjectPtr<UActorDescription>);

	UFUNCTION()
	void OnDialogueOptionsReceived(FDialogueOptions& InDialogueOptions);
	
	UFUNCTION(BlueprintCallable)
	FString GetGreeting();

	UFUNCTION(BlueprintCallable)
	FDialogueOptions GetDialogueOptions();
	

	FDialogueOptionsDelegate DialogueOptionsDelegate;
	
protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:

	TScriptInterface<IDialogueProvider> DialogueProvider; 
	bool bInDialogue = false;
	
	void BindDialogueDelegates();
	void UnBindDialogueDelegates();

	};
