// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadingWidget.h"
#include "Subsystems/UISystem.h"

void ULoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULoadingWidget::OnGameLoaded()
{
	FadeOut();
}

void ULoadingWidget::FadeOut()
{
	CurrentFadeTime = 0.0f;

	SetRenderOpacity(1.0f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULoadingWidget::UpdateFadeOut, 0.01f, true);
}

void ULoadingWidget::UpdateFadeOut()
{
	CurrentFadeTime += 0.01f;
	
	float Alpha = FMath::Clamp(1.0f - (CurrentFadeTime / FadeOutDuration), 0.0f, 1.0f);
	SetRenderOpacity(Alpha);

	if (CurrentFadeTime >= FadeOutDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		
		if(auto UISystem = GetGameInstance()->GetSubsystem<UUISystem>())
			UISystem->RequestFinishWidget(GetWorld()->GetFirstPlayerController(), EUIElementType::Loading);
		else
			RemoveFromParent();
	}
}