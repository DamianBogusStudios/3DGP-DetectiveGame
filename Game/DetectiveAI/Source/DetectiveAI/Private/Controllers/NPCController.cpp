// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/NPCController.h"


void ANPCController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ANPCController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ANPCController::BeginPlay()
{
	Super::BeginPlay();
}
void ANPCController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}