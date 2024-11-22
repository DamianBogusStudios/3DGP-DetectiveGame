// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LockpickMiniGame.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"

#pragma region BP Native Events
void ULockpickMiniGame::ReceivePreInitialisation_Implementation()
{
	
}
void ULockpickMiniGame::ReceiveReset_Implementation()
{
	
}
#pragma endregion

void ULockpickMiniGame::NativeConstruct()
{
	Super::NativeConstruct();

	InitialisePins();
}

void ULockpickMiniGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	MovePins();
}

void ULockpickMiniGame::InitialisePins()
{
	ReceivePreInitialisation();
}

void ULockpickMiniGame::TensionLock()
{
	SetLockState(ELockState::Tensioned);
}

void ULockpickMiniGame::TensionReleased()
{
	SetLockState(ELockState::Locked);
	for(auto& Pin : Pins)
	{
		Pin.TargetY = 190.0f;
	}
}

void ULockpickMiniGame::PickPin()
{
	if(CurrentPin < Pins.Num())
	{
		Pins[CurrentPin].TargetY = 0;
		Pins[CurrentPin].bMovingUp = true;
	}
}

void ULockpickMiniGame::RaisePin(float Percentage)
{
	if(CurrentPin < Pins.Num())
	{
		Pins[CurrentPin].TargetY = 0;
		Pins[CurrentPin].bMovingUp = true;
	}
}

void ULockpickMiniGame::SecurePin()
{
	if(CurrentPin < Pins.Num())
	{
		auto& LockPin = Pins[CurrentPin];
		FVector2D CurrentPosition = LockPin.Pin->GetRenderTransform().Translation;
		if(CurrentPosition.Y < LockPin.TargetZoneSize)
		{
			LockPin.Pin->SetColorAndOpacity(FLinearColor::Green);
			LockPin.bSet = true;
			LockPin.Pin->SetRenderTranslation(FVector2D(CurrentPosition.X, 0));
		}
	}
}

void ULockpickMiniGame::CheckLockStatus()
{
	int SetPinCount = 0;

	for(auto& Pin : Pins)
	{
		if(Pin.bSet)
			SetPinCount++;
	}

	if(SetPinCount == Pins.Num())
	{
		SetLockState(ELockState::Unlocked);
		OnMiniGameFinished.Broadcast(true);
	}
}

void ULockpickMiniGame::SetLockState(ELockState InState)
{
	if(LockState == InState || LockState == ELockState::Unlocked)
		return;

	LockState = InState;
}

void ULockpickMiniGame::MovePick(bool bLeft)
{
	CurrentPin += bLeft ? -1 : 1;
	CurrentPin = FMath::Clamp(CurrentPin, 0, Pins.Num() - 1);

	FVector2D CurrentPosition = Pick->GetRenderTransform().Translation;
	FVector2D PinPosition = Pins[CurrentPin].Pin->GetParent()->GetRenderTransform().Translation;
	
	Pick->SetRenderTranslation(FVector2D(PinPosition.X, CurrentPosition.Y));
}


void ULockpickMiniGame::MovePins()
{
	for (auto& Pin : Pins)
	{
		if(LockState != ELockState::Tensioned || Pin.bMovingUp)
			MovePinToTarget(Pin);
	}
}

void ULockpickMiniGame::MovePinToTarget(FLockPin& Pin)
{
	if(Pin.bSet)
		return;
	
	FVector2D CurrentPosition = Pin.Pin->GetRenderTransform().Translation;
	Pin.bMovingUp = CurrentPosition.Y > Pin.TargetY;

	float InterpSpeed =(Pin.TargetY < CurrentPosition.Y) ? PinRiseSpeed : PinFallSpeed;
	if(FMath::Abs(Pin.TargetY - CurrentPosition.Y) < 10)
		InterpSpeed *= 10;
	
	float NewY = FMath::FInterpTo(CurrentPosition.Y, Pin.TargetY, GetWorld()->GetDeltaSeconds(), InterpSpeed); 
	Pin.Pin->SetRenderTranslation(FVector2D(CurrentPosition.X, NewY));

	if (FMath::IsNearlyEqual(NewY, Pin.TargetY, 1.0f))
	{
		Pin.TargetY = Pin.MaxY;
	}
}

void ULockpickMiniGame::Cleanup_Implementation()
{
	ReceiveReset();
}

