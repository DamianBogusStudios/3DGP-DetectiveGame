// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LockpickMiniGame.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Controllers/MainPlayerController.h"
#include "Interfaces/MiniGameCallbackInterface.h"

#pragma region IWidgetInterface

void ULockpickMiniGame::Setup_Implementation(UObject* Caller)
{
	ResetPins();
	
	if ((PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())))
	{
		PlayerController->MiniGameInputInterface.SetObject(this); 
		PlayerController->MiniGameInputInterface.SetInterface(Cast<IMiniGameInputInterface>(this)); 

		PlayerController->RequestInputContext(EInputContext::MiniGame, false);
	}

	if(Caller->Implements<UMiniGameCallbackInterface>())
	{
		CallbackInterface.SetInterface(Cast<IMiniGameCallbackInterface>(Caller));
		CallbackInterface.SetObject(Caller);
	}
	

	
	FVector2D CurrentPosition = Pick->GetRenderTransform().Translation;
	Pick->SetRenderTranslation(FVector2D(50, CurrentPosition.Y));


	
	UpdateRightTriggerEffect();
	UpdateLeftTriggerEffect();
	bRunTick = true;
}

void ULockpickMiniGame::ResetPins()
{
	LockState = ELockState::Locked;
	for(auto& Pin : Pins)
	{
		Pin.PinState = EPinState::Loose;
		Pin.SetPosition = FMath::FRandRange(50.0f, 100.0f);
		Pin.TargetPosition = 230;
		FVector2D CurrentPosition = Pin.Pin->GetRenderTransform().Translation;
		Pin.Pin->SetRenderTranslation(FVector2D(CurrentPosition.X, 230));
	}
}

void ULockpickMiniGame::Advance_Implementation()
{
	if(bAllowForceUnlock)
	{
		for(auto& Pin : Pins)
		{
			Pin.PinState = EPinState::Set;
		}
			SetLockState(ELockState::Unlocked);
	}
}



void ULockpickMiniGame::Cleanup_Implementation()
{
	bRunTick = false;

	ResetPins();
	
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
	if (LockState == ELockState::Tensioned)
	{
		SetLockState(ELockState::Locked);
	}
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
	FVector2D CurrentPosition = TensionWrench->GetRenderTransform().Translation;	
	TensionWrench->SetRenderTranslation(FVector2D(250 * Value, CurrentPosition.Y));
	if (Value > 0.5f && LockState == ELockState::Locked)
	{
		SetLockState(ELockState::Tensioned);
	}
}
void ULockpickMiniGame::HandleRaisePin_MiniGame(float Value)
{
	if(CurrentPin < Pins.Num())
	{
		Pins[CurrentPin].TargetPosition = 230 - (230 * Value);
	}
}
void ULockpickMiniGame::HandleMovePick_MiniGame(bool Right)
{
	UpdateCurrentPin(Right ? 1 : -1);
	UpdateRightTriggerEffect();

	FVector2D CurrentPosition = Pick->GetRenderTransform().Translation;	
	Pick->SetRenderTranslation(FVector2D(50 + (100 * (CurrentPin)), CurrentPosition.Y));
}
void ULockpickMiniGame::HandleSetPin_MiniGame()
{
	if(CurrentPin < Pins.Num())
	{
		auto& Pin = Pins[CurrentPin];
		FVector2D CurrentPosition = Pin.Pin->GetRenderTransform().Translation;

		if (FMath::IsNearlyEqual(CurrentPosition.Y, Pin.TargetPosition, 1.0f))
		{
			SetPin(Pin);
		}
	}
}
#pragma endregion


void ULockpickMiniGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(bRunTick)
	{
		MovePins();
		DebugStateToScreen();
	}
}

void ULockpickMiniGame::SetLockState(ELockState InState)
{
	LockState = InState;
	if (LockState == ELockState::Locked)
	{
		for (auto& Pin : Pins)
		{
			Pin.PinState = EPinState::Loose;
		}
		BindRandomPin();
	}
	else if(LockState == ELockState::Unlocked && GetNotSetPins().Num() == 0)
	{
		if(CallbackInterface)
		{
			CallbackInterface->OnMiniGameFinished(true);
		}

		if(auto UI = GetGameInstance()->GetSubsystem<UUISystem>())
		{
			UI->RequestFinishWidget(this, EUIElementType::LockpickMiniGame);
		}
	}
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

			if(i == OriginalArrayIndex)
			{
				Pins[i].PinState = EPinState::Binding;
			}
			else
			{
				Pins[i].PinState = EPinState::Loose;
			}
		}
	}
	else
	{
		SetLockState(ELockState::Unlocked);
	}
}

void ULockpickMiniGame::UpdateRightTriggerEffect()
{
	if (CurrentPin < Pins.Num() && PlayerController)
	{
		FLockPin* Pin = &Pins[CurrentPin];

		if (Pin->PinState == EPinState::Binding && LockState == ELockState::Tensioned)
		{
			float Start = (1.0f / (250 - (Pin->SetPosition / 250.0f)));
			float End = Start + 50.0f;
			
			FAdaptiveTriggerEffect TriggerEffect(ETrigger::Right, ETriggerEffect::Weapon,
				1.0f, Start, End);
			PlayerController->SetAdaptiveTriggerEffect(TriggerEffect);
		}
		else
		{
			PlayerController->ResetTriggerEffect(ETrigger::Right);
		}
	}
}

void ULockpickMiniGame::UpdateLeftTriggerEffect()
{
	FAdaptiveTriggerEffect TriggerEffect(ETrigger::Left, ETriggerEffect::Feedback,0.5f,
		0, 1);			
	PlayerController->SetAdaptiveTriggerEffect(TriggerEffect);
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
	
}

void ULockpickMiniGame::SetPin(FLockPin& Pin)
{
	if (Pin.PinState == EPinState::Binding && LockState == ELockState::Tensioned)
	{
		FVector2D CurrentPosition = Pin.Pin->GetRenderTransform().Translation;
		Pin.Pin->SetRenderTranslation(FVector2D(CurrentPosition.X, Pin.TargetPosition));
		
		Pin.PinState = EPinState::Set;
		UpdateRightTriggerEffect();
		BindRandomPin();
		//todo play sound effect.
	}
}

void ULockpickMiniGame::DebugStateToScreen()
{
    if (!GEngine) return;

    FString DebugMessage = "=== LockpickMiniGame Debug State ===\n";
	
    DebugMessage += FString::Printf(TEXT("Lock State: %s\n"), *UEnum::GetValueAsString(LockState));
    DebugMessage += FString::Printf(TEXT("Current Pin: %d\n"), CurrentPin);

    // // Callback Interface
    // DebugMessage += FString::Printf(TEXT("Callback Interface: %s\n"), CallbackInterface.GetObject() ? TEXT("Set") : TEXT("Unset"));
    //
    // // Run Tick Status
    // DebugMessage += FString::Printf(TEXT("Run Tick: %s\n"), bRunTick ? TEXT("True") : TEXT("False"));

    // Pin States
    DebugMessage += "Pins:\n";
    for (int i = 0; i < Pins.Num(); ++i)
    {
        const auto& Pin = Pins[i];
        FString PinState = UEnum::GetValueAsString(Pin.PinState);
        DebugMessage += FString::Printf(TEXT("  Pin %d: State=%s, SetPos=%.2f, TargetPos=%.2f\n"), 
                                        i, *PinState, Pin.SetPosition, Pin.TargetPosition);

        if (Pin.Pin)
        {
            FVector2D Translation = Pin.Pin->GetRenderTransform().Translation;
            DebugMessage += FString::Printf(TEXT("    Position: X=%.2f, Y=%.2f\n"), Translation.X, Translation.Y);
        }
        else
        {
            DebugMessage += "    Pin Widget: Null\n";
        }
    }
    //
    // // Pick Position
    // if (Pick)
    // {
    //     FVector2D PickPosition = Pick->GetRenderTransform().Translation;
    //     DebugMessage += FString::Printf(TEXT("Pick Position: X=%.2f, Y=%.2f\n"), PickPosition.X, PickPosition.Y);
    // }
    // else
    // {
    //     DebugMessage += "Pick Widget: Null\n";
    // }

    // Tension Wrench Position
    if (TensionWrench)
    {
        FVector2D TensionPosition = TensionWrench->GetRenderTransform().Translation;
        DebugMessage += FString::Printf(TEXT("Tension Wrench Position: X=%.2f, Y=%.2f\n"), TensionPosition.X, TensionPosition.Y);
    }
    else
    {
        DebugMessage += "Tension Wrench Widget: Null\n";
    }

    // Collapse by assigning a unique key
    static const int32 DebugMessageKey = 1;
    GEngine->AddOnScreenDebugMessage(DebugMessageKey, 0.f, FColor::Cyan, DebugMessage);
}
