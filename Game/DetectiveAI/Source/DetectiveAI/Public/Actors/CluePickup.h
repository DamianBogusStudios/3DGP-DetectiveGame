// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Types/CommonCaseTypes.h"
#include "CluePickup.generated.h"


UCLASS()
class DETECTIVEAI_API ACluePickup : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACluePickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clue")
	FClue Clue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	class USphereComponent* TriggerArea;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	bool bDestroyOnPickup;

	virtual void Interact_Implementation(AActor* Caller) override;
	virtual void OnFocus_Implementation() override;
	virtual void EndFocus_Implementation() override;

protected:
	
	UFUNCTION(BlueprintNativeEvent, Category = Colision)
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = Colision)
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY(VisibleAnywhere, Category = Pickup)
	bool bIsFocused;

	UPROPERTY(VisibleAnywhere, Category = Pickup)
	bool bInRange;

	UPROPERTY(VisibleAnywhere, Category = Pickup)
	AActor* FocusedActor;
};
