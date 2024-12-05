// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/WidgetInterface.h"
#include "PickupWidget.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UPickupWidget : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TypeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DescText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loading")
	float FadeOutDuration = 1.0f;

	/* IWidgetInterface */
	virtual void Advance_Implementation() override;
	virtual void Setup_Implementation(UObject* Caller) override;
	virtual void Cleanup_Implementation() override;

protected:

	FTimerHandle TimerHandle;
	float CurrentFadeTime = 0.0f;
	
	void FadeOut();
	void UpdateFadeOut();
};
