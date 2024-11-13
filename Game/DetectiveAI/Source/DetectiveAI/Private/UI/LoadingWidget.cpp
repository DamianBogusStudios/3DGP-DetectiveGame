// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadingWidget.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameClasses/MGameInstance.h"
#include "Subsystems/CaseSystem.h"
#include "Subsystems/UISystem.h"

void ULoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	StartGameBtn->OnClicked.AddDynamic(this, &ULoadingWidget::OnStartGamePressed);

	if(auto GI = Cast<UMGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->OnGameFinishedLoading.AddDynamic(this, &ULoadingWidget::OnGameLoaded);
		GI->OnGenProgressUpdated.AddDynamic(this, &ULoadingWidget::OnProgressUpdated);
	}
}

void ULoadingWidget::OnStartGamePressed()
{
	if(auto GameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		GameInstance->StartGame();
		StartGameBtn->SetVisibility(ESlateVisibility::Hidden);
		LoadingBar->SetVisibility(ESlateVisibility::Visible);
		LoadingText->SetText(FText::FromString("Loading..."));
	}
}

void ULoadingWidget::OnGameLoaded()
{
	FadeOut();
}

void ULoadingWidget::OnProgressUpdated(const FString& ProgressMessage, float PercentComplete)
{
	LoadingText->SetText(FText::FromString(ProgressMessage));
	TargetPerc = PercentComplete;
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

void ULoadingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
	if (LoadingBar->GetVisibility() == ESlateVisibility::Visible && !FMath::IsNearlyEqual(CurrentPerc, TargetPerc))
	{
		CurrentPerc = FMath::FInterpTo(CurrentPerc, TargetPerc, InDeltaTime, BarSpeed);
		LoadingBar->SetPercent(CurrentPerc);
	}
}
