// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/WidgetInterface.h"
#include "LockpickMiniGame.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMiniGameFinishedDelegate, bool, bSuccess);

class UImage;

UENUM(BlueprintType)
enum class ELockState : uint8
{
	Locked,
	Tensioned,
	Unlocked
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
	float MinY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetZoneSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMovingUp;
};
/**
 * 
 */
UCLASS()
class DETECTIVEAI_API ULockpickMiniGame : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame Settings")
	float PinFallSpeed = 4.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame Settings")
	float PinRiseSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "MiniGame Settings")
	bool bDrawDebugElements = true;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Pick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TArray<FLockPin> Pins;

	UPROPERTY(VisibleAnywhere)
	FMiniGameFinishedDelegate OnMiniGameFinished;

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "PreInit"))
	void ReceivePreInitialisation();

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Reset MiniGame"))
	void ReceiveReset();
	
	UFUNCTION(BlueprintCallable)
	void TensionLock();

	UFUNCTION(BlueprintCallable)
	void TensionReleased();

	UFUNCTION(BlueprintCallable)
	void SecurePin();
	
	UFUNCTION(BlueprintCallable)
	void PickPin();

	UFUNCTION(BlueprintCallable)
	void RaisePin(float Percentage);

	UFUNCTION(BlueprintCallable)
	void MovePick(bool bLeft);

	

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void Cleanup_Implementation() override;
private:
	
	uint8 CurrentPin = 0;
	ELockState LockState = ELockState::Locked;

	void InitialisePins();
	void MovePins();
	void MovePinToTarget(FLockPin& Pin);

	void CheckLockStatus();
	void SetLockState(ELockState InState);	
};
