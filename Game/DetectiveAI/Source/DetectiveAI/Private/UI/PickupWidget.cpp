// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PickupWidget.h"

#include "ActorComponents/BasicInventory.h"
#include "Components/TextBlock.h"
#include "Subsystems/UISystem.h"


void UPickupWidget::Advance_Implementation() 
{
	FadeOut();
}
void UPickupWidget::Setup_Implementation(UObject* Caller)
{
	SetRenderOpacity(1.0f);
	if (const auto Inventory = Cast<UBasicInventory>(Caller))
	{
		FClue Clue;
		if (Inventory->GetLastClue(Clue))
		{

			//FString EnumValueString = UEnum::GetValueAsString(Clue.ClueType);
			//EnumValueString = EnumValueString.Mid(EnumValueString.Find(TEXT("::")) + 2);

			const FString ResourceString = StaticEnum<EClueType>()->GetValueAsString(Clue.ClueType);
			
			TypeText->SetText(FText::FromString(ResourceString));
			DescText->SetText(FText::FromString(Clue.Description));
		}
	}
}
void UPickupWidget::Cleanup_Implementation()
{
	
}

void UPickupWidget::FadeOut()
{
	CurrentFadeTime = 0.0f;

	SetRenderOpacity(1.0f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPickupWidget::UpdateFadeOut, 0.01f, true);
}

void UPickupWidget::UpdateFadeOut()
{
	CurrentFadeTime += 0.01f;
	
	float Alpha = FMath::Clamp(1.0f - (CurrentFadeTime / FadeOutDuration), 0.0f, 1.0f);
	SetRenderOpacity(Alpha);

	if (CurrentFadeTime >= FadeOutDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		
		if(auto UISystem = GetGameInstance()->GetSubsystem<UUISystem>())
			UISystem->RequestFinishWidget(GetWorld()->GetFirstPlayerController(), EUIElementType::Pickup);
		else
			RemoveFromParent();
	}
}