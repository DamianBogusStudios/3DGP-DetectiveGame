// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/MiniGameInputInterface.h"
#include "Interfaces/WidgetInterface.h"
#include "LockpickMiniGame.generated.h"

// static void SetTriggerEffectProperty(int StartPos, int EndPos, int Strength, int Trigger);

class AMainPlayerController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMiniGameFinishedDelegate, bool, bSuccess);

class UImage;

UENUM(BlueprintType)
enum class ELockState : uint8
{
	Locked,
	Tensioned,
	Unlocked
};

UENUM(BlueprintType)
enum class EPinState : uint8
{
	Loose,
	Binding,
	Set
};

USTRUCT(BlueprintType)
struct FLockPin
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UImage> Pin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UImage> TargetZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SetPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetPosition = 230;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPinState PinState = EPinState::Loose;
};
/**
 * 
 */
UCLASS()
class DETECTIVEAI_API ULockpickMiniGame : public UUserWidget, public IWidgetInterface, public IMiniGameInputInterface
{
	GENERATED_BODY()

public:

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame Settings")
	float PinFallSpeed = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "MiniGame Settings")
	bool bDrawDebugElements = true;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Pick;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TensionWrench;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TArray<FLockPin> Pins;

	UPROPERTY(VisibleAnywhere)
	FMiniGameFinishedDelegate OnMiniGameFinished;

	UFUNCTION(BlueprintImplementableEvent, Category = "Initialisation")
	void InitialisePins();

	/* IWidgetInterface */
	virtual void Advance_Implementation() override;
	virtual void Setup_Implementation(UObject* Caller) override;
	virtual void Cleanup_Implementation() override;
	
	/* IMiniGameInputInterface*/
	virtual void HandleApplyTension_MiniGame(float Value) override;
	virtual void HandleApplyTensionCompleted_MiniGame() override;
	virtual void HandleRaisePin_MiniGame(float Value) override;
	virtual void HandleRaisePinCompleted_MiniGame() override;
	virtual void HandleMovePin_MiniGame(bool Right) override;

	
protected:
	
	// virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

	TObjectPtr<AMainPlayerController> PlayerController;
	
	uint8 CurrentPin = 0;
	ELockState LockState = ELockState::Locked;

	void MovePins();
	void MovePinToTarget(FLockPin& Pin);

	void BindRandomPin();
	void UpdateTriggerEffect();
	void UpdateCurrentPin(int Value);
	
	void CheckLockStatus();
	void SetLockState(ELockState InState);

	TArray<int> GetNotSetPins()
	{
		TArray<int> UnsetPins;
	
		for (int Index = 0; Index < Pins.Num(); ++Index)
		{
			if (Pins[Index].PinState != EPinState::Set)
			{
				UnsetPins.Add(Index); 
			}
		}

		return UnsetPins;
	}
};
