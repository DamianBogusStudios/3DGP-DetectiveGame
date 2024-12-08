// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CluePickup.h"

#include "ActorComponents/BasicInventory.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameClasses/AGPGameInstance.h"
#include "Subsystems/UISystem.h"

ACluePickup::ACluePickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TriggerArea = CreateDefaultSubobject<USphereComponent>(TEXT("ProximitySphere"));
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ACluePickup::OnBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &ACluePickup::OnEndOverlap);

	SetRootComponent(TriggerArea);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(TriggerArea);
}

void ACluePickup::OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bInRange = true;
}

void ACluePickup::OnEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bInRange = false;
}

void ACluePickup::Interact_Implementation(AActor* Caller)
{
	if (bInRange && bIsFocused)
	{
		FocusedActor = Caller;
		if(auto UI = GetGameInstance()->GetSubsystem<UUISystem>())
		{
			UI->RequestStartWidget(this, EUIElementType::LockpickMiniGame);
		}
		
		// if (UBasicInventory* Inventory = Caller->FindComponentByClass<UBasicInventory>())
		// {
		// 	Inventory->AddClue(Clue);
		// 	
		// 	if(bDestroyOnPickup) Destroy();
		// }
	}
}
void ACluePickup::OnFocus_Implementation()
{
	bIsFocused = true;
	/*if(Mesh)
	{
		if(UAGPGameInstance* GameInstance = Cast<UAGPGameInstance>(GetGameInstance()))
		{
			Mesh->SetOverlayMaterial(GameInstance->GetOverlayMaterial(true));
		}
	}*/

}

void ACluePickup::EndFocus_Implementation()
{
	bIsFocused = false;
	/*if (Mesh)
	{
		Mesh->SetOverlayMaterial(nullptr);
	}*/
}

void ACluePickup::OnMiniGameFinished(bool bSuccess)
{
	if(!FocusedActor)
		return;

	
	if (UBasicInventory* Inventory = FocusedActor->FindComponentByClass<UBasicInventory>())
	{
		Inventory->AddClue(Clue);
		if(bDestroyOnPickup) Destroy();
	}
}
