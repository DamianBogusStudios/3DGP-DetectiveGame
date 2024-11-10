// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class DETECTIVEAI_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LoadingText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* LoadingGizmo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loading")
	float FadeOutDuration = 1.0f;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnGameLoaded();

protected:

	FTimerHandle TimerHandle;
	float CurrentFadeTime = 0.0f;
	
	void FadeOut();
	void UpdateFadeOut();
	
};
