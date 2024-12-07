// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"

#include "ActorComponents/BasicInventory.h"
#include "Components/SphereComponent.h"
#include "Interfaces/Interactable.h"

// Sets default values
AMainCharacter::AMainCharacter(const FObjectInitializer & ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BasicInventory = CreateDefaultSubobject<UBasicInventory>(TEXT("Inventory"));
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Sphere")); 
	InteractionSphere->SetupAttachment(RootComponent);
	
}
 
// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnBeginInteractionOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnEndInteractionOverlap);
	GetWorldTimerManager().SetTimer(UpdateTimerHandle, this, &AMainCharacter::SortFocusedActors, FocusUpdateFrequency, true);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::OnBeginInteractionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UInteractable>())
	{
		InteractableActors.Add(OtherActor);
	}//
}

void AMainCharacter::OnEndInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (OtherActor && OtherActor->Implements<UInteractable>() && InteractableActors.Contains(OtherActor))
	{
		if (OtherActor == FocusedActor)
		{
			SelectFocusedActor(nullptr);
		}
		InteractableActors.Remove(OtherActor);
	}
}

void AMainCharacter::SortFocusedActors()
{
	float BestScore = -1; 
	TSoftObjectPtr<AActor> CachedActor;

	for (const auto& Actor : InteractableActors)
	{
		float DistRatio = 1 - (FVector::DistSquared(GetActorLocation(), Actor->GetActorLocation()) / FMath::Square(MaxInterationDist));
		float DotProduct = FVector::DotProduct(GetActorForwardVector(), Actor->GetActorForwardVector()) + 1;

		float Score = DotProduct + DistRatio;

		if (Score >= BestScore)
		{
			BestScore = Score;
			CachedActor = Actor;
		}
	}

	if(CachedActor)
	{
		SelectFocusedActor(CachedActor);
	}
}

void AMainCharacter::SelectFocusedActor(TSoftObjectPtr<AActor> NewFocusedActor)
{
	if(NewFocusedActor == FocusedActor)
	{
		return;
	}

	
	if(FocusedActor)
	{
		IInteractable::Execute_EndFocus(FocusedActor.Get());
	}
	if (NewFocusedActor && NewFocusedActor->Implements<UInteractable>())
	{
		IInteractable::Execute_OnFocus(NewFocusedActor.Get());
	}

	FocusedActor = NewFocusedActor;
}


void AMainCharacter::Interact()
{
	//final check
	if (FocusedActor && FocusedActor->Implements<UInteractable>())
	{
		IInteractable::Execute_Interact(FocusedActor.Get(), this);
	}
}

