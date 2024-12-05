// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Interactable.h"
#include "BaseNPC.generated.h"

struct FActorDescription;
class UDialogueComponent;

UCLASS()
class DETECTIVEAI_API ABaseNPC : public ACharacter, public IInteractable
{
	GENERATED_BODY()

public:

	ABaseNPC(const FObjectInitializer& ObjectInitializer, FActorDescription ActorDescription, USkeletalMesh* InitialMesh = nullptr);

	
	// Sets default values for this character's properties
	ABaseNPC();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	UDialogueComponent* DialogueComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
	void InitialiseActor(FActorDescription ActorDescription, USkeletalMesh* SkeletalMesh);

	void Interact_Implementation(AActor* Caller) override;
	void OnFocus_Implementation() override;
	void EndFocus_Implementation() override;

private:

	USkeletalMesh* PendingMesh;
	FActorDescription* PendingActorDescription;
};
