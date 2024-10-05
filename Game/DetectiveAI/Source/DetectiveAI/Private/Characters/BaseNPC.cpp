// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseNPC.h"
#include "MGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "ActorComponents/DialogComponent.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DialogComponent = CreateDefaultSubobject<UDialogComponent>(TEXT("Dialog Component"));
}

// Called when the game starts or when spawned
void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseNPC::Interact_Implementation(AActor* Caller)
{
	if(DialogComponent)
	{
		DialogComponent->StartDialog();
	}
}

void ABaseNPC::OnFocus_Implementation()
{
	if(GetMesh())
	{
		UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance());

		if(GameInstance)
		{
			GetMesh()->SetOverlayMaterial(GameInstance->GetOverlayMaterial(true));
		}
	}

}

void ABaseNPC::EndFocus_Implementation()
{
	if (GetMesh())
	{
		GetMesh()->SetOverlayMaterial(nullptr);
	}
}

