// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueWidget.h"

#include "ActorComponents/DialogueComponent.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"


void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDialogueWidget::Setup_Implementation(UObject* Caller)
{
	DialogueComponent = Cast<UDialogueComponent>(Caller);

	if (DialogueComponent)
	{
		DialogueComponent->MessageRelayDelegate.BindDynamic(this, &UDialogueWidget::SpeechReceived);	
		Speak("Hello");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DialogueComponent is null"));	
	}
}

void UDialogueWidget::Advance_Implementation()
{
	if(WidgetVisibility == EDialogueVisibility::EBotSpeaking)
		Reply();
	else
	{
		if(InputTextBox && DialogueComponent)
		{
			FString PlayerMessage = InputTextBox->GetText().ToString();
			DialogueComponent->SendMessageToActor(PlayerMessage);
			InputTextBox->SetText(FText::GetEmpty());
			
			Speak("...");
		}
		
	}
}

void UDialogueWidget::Cleanup_Implementation()
{
	if (DialogueComponent)
	{
		DialogueComponent->MessageRelayDelegate.Unbind();
		DialogueComponent = nullptr;
	}
	ResetDialogueWidget();
}

void UDialogueWidget::Speak(FString Text)
{
	if(SpeechText)
	{
		SpeechText->SetText(FText::FromString(Text));
		ToggleBoxes(EDialogueVisibility::EBotSpeaking);
	}
}

void UDialogueWidget::Reply()
{
	if(InputTextBox)
	{
		//InputTextBox->SetHintText(FText::FromString("Type your response"));
		ToggleBoxes(EDialogueVisibility::EPlayerReply);
	}
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

	WidgetVisibility = DialogueVisibility;
}

void UDialogueWidget::SpeechReceived(const FString& Message)
{
	if(SpeechText)
	{
		SpeechText->SetText(FText::FromString(Message));
	}
}

void UDialogueWidget::TranscriptReceived(const FString& Message)
{
	if(InputTextBox)
	{
		InputTextBox->SetText(FText::FromString(Message));
	}
}
