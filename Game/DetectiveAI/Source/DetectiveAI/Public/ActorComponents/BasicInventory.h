// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/CommonCaseTypes.h"
#include "BasicInventory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DETECTIVEAI_API UBasicInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBasicInventory();
	
	void AddClue(const FClue& Clue);
	bool GetLastClue(FClue& OutClue);
	
protected:

	UPROPERTY()
	TArray<FClue> Clues;
};
