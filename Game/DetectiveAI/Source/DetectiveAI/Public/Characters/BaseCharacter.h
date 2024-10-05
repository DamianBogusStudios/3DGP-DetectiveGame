// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/Array.h"
#include "BaseCharacter.generated.h"


class USphereComponent;
class InteractInterface;

UCLASS()
class DETECTIVEAI_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	USphereComponent* InteractionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	float FocusUpdateFrequency = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	float MaxInterationDist = 200.f;

	UFUNCTION()
	void Interact();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	TArray<TSoftObjectPtr<AActor>> InteractableActors;

	TSoftObjectPtr<AActor> FocusedActor;

	FTimerHandle UpdateTimerHandle;

	UFUNCTION()
	void OnBeginInteractionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndInteractionOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void SortFocusedActors();
	void SelectFocusedActor(TSoftObjectPtr<AActor> NewFocusedActor);
};
