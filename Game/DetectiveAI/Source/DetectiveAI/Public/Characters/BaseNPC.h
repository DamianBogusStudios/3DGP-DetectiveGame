// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractInterface.h"
#include "BaseNPC.generated.h"

class UDialogComponent;

UCLASS()
class DETECTIVEAI_API ABaseNPC : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	UDialogComponent* DialogComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




	void Interact_Implementation(AActor* Caller) override;
	void OnFocus_Implementation() override;
	void EndFocus_Implementation() override;
};
