// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Interfaces/InteractInterface.h"

// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer & ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Sphere")); 
	
	InteractionSphere->SetupAttachment(RootComponent);
}
 
// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();


	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnBeginInteractionOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnEndInteractionOverlap);
	GetWorldTimerManager().SetTimer(UpdateTimerHandle, this, &ABaseCharacter::SortFocusedActors, FocusUpdateFrequency, true);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::OnBeginInteractionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UInteractInterface>())
	{
		InteractableActors.Add(OtherActor);
	}
}

void ABaseCharacter::OnEndInteractionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (OtherActor && OtherActor->Implements<UInteractInterface>() && InteractableActors.Contains(OtherActor))
	{
		if (OtherActor == FocusedActor)
		{
			SelectFocusedActor(nullptr);
		}
		InteractableActors.Remove(OtherActor);
	}
}

void ABaseCharacter::SortFocusedActors()
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

void ABaseCharacter::SelectFocusedActor(TSoftObjectPtr<AActor> NewFocusedActor)
{
	if(NewFocusedActor == FocusedActor)
	{
		return;
	}
	else
	{
		if(FocusedActor)
		{
			IInteractInterface::Execute_EndFocus(FocusedActor.Get());
		}
	
		if (NewFocusedActor && NewFocusedActor->Implements<UInteractInterface>()) //Check
		{
			IInteractInterface::Execute_OnFocus(NewFocusedActor.Get());
		}

		FocusedActor = NewFocusedActor;
	}
}


void ABaseCharacter::Interact()
{
	//final check
	if (FocusedActor && FocusedActor->Implements<UInteractInterface>())
	{
		IInteractInterface::Execute_Interact(FocusedActor.Get(), this);
	}
}

