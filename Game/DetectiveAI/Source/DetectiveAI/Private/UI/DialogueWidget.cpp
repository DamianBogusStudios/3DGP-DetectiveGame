// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UDialogueWidget::Speak(FString Text)
{
	if(SpeechText)
	{
		ToggleBoxes(true);
		SpeechText->SetText(FText::FromString(Text));
	}
}

void UDialogueWidget::ToggleBoxes(bool IsSpeaking)
{
	if (SpeechBox)
		SpeechBox->SetVisibility(IsSpeaking ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (ReplyBox)
		ReplyBox->SetVisibility(IsSpeaking ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}
