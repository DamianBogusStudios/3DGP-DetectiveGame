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
		SpeechText->SetText(FText::FromString(Text));
		ToggleBoxes(EDialogueVisibility::EBotSpeaking);
	}
}

void UDialogueWidget::Reply(TArray<FString> TextReplies)
{
	if(ReplyListView)
	{

		ReplyListView->ClearListItems();

		for(auto Reply : TextReplies)
		{
			 UDialogueReplyObject* ReplyObject = NewObject<UDialogueReplyObject>();
			 ReplyObject->Reply = Reply;
			 ReplyListView->AddItem(ReplyObject);
			 ReplyObject->ReplyEvent.AddDynamic(this, &UDialogueWidget::OptionSelected);
		}
		
		ToggleBoxes(EDialogueVisibility::EPlayerReply);
	}
}

void UDialogueWidget::Cleanup_Implementation()
{
	ResetDialogueWidget();
}


void UDialogueWidget::ResetDialogueWidget()
{
	ToggleBoxes(EDialogueVisibility::EHidden);
}


void UDialogueWidget::ToggleBoxes(EDialogueVisibility DialogueVisibility)
{
	if (SpeechBox)
		SpeechBox->SetVisibility(DialogueVisibility == EDialogueVisibility::EBotSpeaking
			? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (ReplyBox)
		ReplyBox->SetVisibility(DialogueVisibility == EDialogueVisibility::EPlayerReply
			? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UDialogueWidget::OptionSelected()
{
	
}