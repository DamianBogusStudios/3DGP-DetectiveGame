// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

class UProgressBar;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DETECTIVEAI_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* StartGameBtn;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LoadingText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* LoadingBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loading")
	float FadeOutDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loading")
	float BarSpeed = 0.8f;
	
	
	UFUNCTION()
	void OnStartGamePressed();

	UFUNCTION()
	void OnGameLoaded();

	UFUNCTION()
	void OnProgressUpdated(const FString& ProgressMessage, float PercentComplete);
	
protected:

	FTimerHandle TimerHandle;
	float CurrentFadeTime = 0.0f;
	
	void FadeOut();
	void UpdateFadeOut();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

	float CurrentPerc;
	float TargetPerc;
	
};
