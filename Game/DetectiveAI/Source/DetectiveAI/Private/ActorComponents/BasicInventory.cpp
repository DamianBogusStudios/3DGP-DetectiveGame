// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/BasicInventory.h"

#include "Subsystems/UISystem.h"

UBasicInventory::UBasicInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UBasicInventory::AddClue(const FClue& Clue)
{
	Clues.Add(Clue);

	if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
	{
		UI->RequestStartWidget(this, EUIElementType::Pickup);
	}
}

bool UBasicInventory::GetLastClue(FClue& OutClue)
{
	if (Clues.Num() == 0)
		return	false;
	
	OutClue = (Clues[Clues.Num() - 1]);
	return true;
}
