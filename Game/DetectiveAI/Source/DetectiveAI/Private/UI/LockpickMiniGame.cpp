// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LockpickMiniGame.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Controllers/MainPlayerController.h"

#pragma region IWidgetInterface

void ULockpickMiniGame::Setup_Implementation(UObject* Caller)
{
	InitialisePins();
	PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PlayerController)
	{
		PlayerController->MiniGameInputInterface.SetObject(this); 
		PlayerController->MiniGameInputInterface.SetInterface(Cast<IMiniGameInputInterface>(this)); 

		PlayerController->RequestInputContext(EInputContext::MiniGame, false);
	}

	BindRandomPin();

	FVector2D CurrentPosition = Pick->GetRenderTransform().Translation;
	Pick->SetRenderTranslation(FVector2D(50, CurrentPosition.Y));
	UpdateTriggerEffect();
}

void ULockpickMiniGame::Advance_Implementation()
{

}


void ULockpickMiniGame::Cleanup_Implementation()
{
	if (PlayerController)
	{
		PlayerController->MiniGameInputInterface.SetObject(nullptr); 
		PlayerController->MiniGameInputInterface.SetInterface(nullptr); 
	}
}

#pragma endregion

#pragma region Input Callbacks

void ULockpickMiniGame::HandleApplyTensionCompleted_MiniGame()
{
	
	FVector2D CurrentPosition = TensionWrench->GetRenderTransform().Translation;	
	TensionWrench->SetRenderTranslation(FVector2D(0, CurrentPosition.Y));
}
void ULockpickMiniGame::HandleRaisePinCompleted_MiniGame()
{
	if(CurrentPin < Pins.Num())
	{
		Pins[CurrentPin].TargetPosition = 230;
	}
}
void ULockpickMiniGame::HandleApplyTension_MiniGame(float Value)
{
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("ApplyTension: %f"), Value);
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, Msg);
	}
	
	FVector2D CurrentPosition = TensionWrench->GetRenderTransform().Translation;	
	TensionWrench->SetRenderTranslation(FVector2D(250 * Value, CurrentPosition.Y));
}
void ULockpickMiniGame::HandleRaisePin_MiniGame(float Value)
{
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("RaisePin: %f"), Value);
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, Msg);
	}
	
	if(CurrentPin < Pins.Num())
	{
		Pins[CurrentPin].TargetPosition = 230 - (230 * Value);
	}
}
void ULockpickMiniGame::HandleMovePin_MiniGame(bool Right)
{
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("MovePin: %s"), Right ? TEXT("Right") : TEXT("Left"));
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, Msg);
	}

	UpdateCurrentPin(Right ? 1 : -1);
	UpdateTriggerEffect();

	FVector2D CurrentPosition = Pick->GetRenderTransform().Translation;	
	Pick->SetRenderTranslation(FVector2D(50 + (100 * (CurrentPin)), CurrentPosition.Y));
}
#pragma endregion


void ULockpickMiniGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	MovePins();
}


void ULockpickMiniGame::CheckLockStatus()
{
	if(GetNotSetPins().Num() == 0)
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


void ULockpickMiniGame::MovePins()
{
	for (auto& Pin : Pins)
	{
		MovePinToTarget(Pin);
	}
}

void ULockpickMiniGame::BindRandomPin()
{
	auto UnsetPins = GetNotSetPins();

	if (UnsetPins.Num() > 0)
	{
		int Index = FMath::RandRange(0, UnsetPins.Num() - 1);
		int OriginalArrayIndex = UnsetPins[Index];

		for (int i = 0; i < Pins.Num(); i++)
		{
			if (Pins[i].PinState == EPinState::Set)
				continue;
			
			Pins[i].PinState = i == OriginalArrayIndex ? EPinState::Binding : EPinState::Loose;
		}
	}
	else
	{
		CheckLockStatus();	
	}
}

void ULockpickMiniGame::UpdateTriggerEffect()
{
	if (CurrentPin < Pins.Num() && PlayerController)
	{
		FLockPin* Pin = &Pins[CurrentPin];

		if (Pin->PinState == EPinState::Binding)
		{
			float Start = (1.0f / (250 - (Pin->SetPosition / 250.0f)));
			float End = Start + 50.0f;
			
			FAdaptiveTriggerEffect TriggerEffect(ETrigger::Right, ETriggerEffect::Weapon,
				1.0f, Start, End);
			
			PlayerController->SetAdaptiveTriggerEffect(TriggerEffect);
		}
	}
}

void ULockpickMiniGame::UpdateCurrentPin(int Value)
{
	int LastPin = CurrentPin;
	
	CurrentPin += Value;
	CurrentPin = FMath::Clamp(CurrentPin, 0, Pins.Num() - 1);

	if (LastPin != CurrentPin)
	{
		Pins[LastPin].TargetPosition = 230;
	}
}


void ULockpickMiniGame::MovePinToTarget(FLockPin& Pin)
{
	if(Pin.PinState == EPinState::Set)
		return;
	
	FVector2D CurrentPosition = Pin.Pin->GetRenderTransform().Translation;

	float InterpSpeed = PinFallSpeed;
	
	if(FMath::Abs(Pin.TargetPosition - CurrentPosition.Y) < 10)
		InterpSpeed *= 10;
	
	float NewY = FMath::FInterpTo(CurrentPosition.Y, Pin.TargetPosition, GetWorld()->GetDeltaSeconds(), InterpSpeed); 
	Pin.Pin->SetRenderTranslation(FVector2D(CurrentPosition.X, NewY));
	
	// if (FMath::IsNearlyEqual(NewY, Pin.TargetPosition, 1.0f))
	// {
	// 	Pin.TargetPosition = 250;
	// }
}

