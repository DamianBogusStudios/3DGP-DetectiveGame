// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameEndWidget.h"

#include "ActorComponents/DialogueComponent.h"
#include "Components/TextBlock.h"
#include "Subsystems/CaseSystem.h"

void UGameEndWidget::Advance_Implementation() 
{
	//FadeOut();
}
void UGameEndWidget::Setup_Implementation(UObject* Caller)
{
	SetRenderOpacity(1.0f);

	FString GuessedSuspect, GuessedMotive, GuessedWeapon;
	
	if (const auto& CaseSystem = GetWorld()->GetGameInstance()->GetSubsystem<UCaseSystem>())
	{
		auto CaseFile = CaseSystem->GetCaseFile();

		const FString Suspect = CaseFile.Actors.FindByPredicate([](const FActorDescription& Actor) {
		return Actor.Roles.Contains(EActorRole::Victim);})->Name;
		const FString Motive = UEnum::GetDisplayValueAsText(CaseFile.Motive).ToString();
		const FString Weapon =  UEnum::GetDisplayValueAsText(CaseFile.MurderWeapon).ToString();

		if (const auto& DialogueComp = Cast<UDialogueComponent>(Caller))
		{
			if (DialogueComp->FunctionParams.Num() >= 3)
			{
				GuessedSuspect = DialogueComp->FunctionParams[0];			
				GuessedMotive = DialogueComp->FunctionParams[1];			
				GuessedWeapon = DialogueComp->FunctionParams[2];			
			}
		}

		SuspectText->SetText(FText::FromString(FString::Printf(TEXT("%s ::: %s"), *Suspect, *GuessedSuspect)));
		MotiveText->SetText(FText::FromString(FString::Printf(TEXT("%s ::: %s"), *Motive, *GuessedMotive)));
		WeaponText->SetText(FText::FromString(FString::Printf(TEXT("%s ::: %s"), *Weapon, *GuessedWeapon)));
	}
}
void UGameEndWidget::Cleanup_Implementation()
{
	
}
