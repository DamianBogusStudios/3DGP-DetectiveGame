// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/ListView.h"
#include "Data/DialogueReplyObject.h"

void UDialogueWidget::Speak(FString Text)
{
	if(SpeechText)
	{
		ToggleBoxes(true);
		SpeechText->SetText(FText::FromString(Text));
	}
}

void UDialogueWidget::Reply(TArray<FString> TextReplies)
{
	if(ReplyListView)
	{
		ToggleBoxes(false);

		ReplyListView->ClearListItems();

		for(auto Reply : TextReplies)
		{
			 UDialogueReplyObject* ReplyObject = NewObject<UDialogueReplyObject>();
			 ReplyObject->Reply = Reply;
			 ReplyListView->AddItem(ReplyObject);
			 ReplyObject->ReplyEvent.AddDynamic(this, &UDialogueWidget::OptionSelected);
		}
	}
}


void UDialogueWidget::ToggleBoxes(bool IsSpeaking)
{
	if (SpeechBox)
		SpeechBox->SetVisibility(IsSpeaking ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (ReplyBox)
		ReplyBox->SetVisibility(IsSpeaking ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}
void UDialogueWidget::OptionSelected()
{
	
}