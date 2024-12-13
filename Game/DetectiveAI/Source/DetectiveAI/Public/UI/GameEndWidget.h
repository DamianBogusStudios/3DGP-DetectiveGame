// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/WidgetInterface.h"
#include "GameEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UGameEndWidget : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SuspectText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MotiveText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WeaponText;

	
	/* IWidgetInterface */
	virtual void Advance_Implementation() override;
	virtual void Setup_Implementation(UObject* Caller) override;
	virtual void Cleanup_Implementation() override;
};
