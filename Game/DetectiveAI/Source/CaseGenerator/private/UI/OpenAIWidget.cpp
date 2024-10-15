// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OpenAIWidget.h"

#include "ChatGPT/ChatGPT.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/VerticalBoxSlot.h"
#include "FuncLib/OpenAIFuncLib.h"

void UOpenAIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(SendMessageButton);
	check(DumpChatButton);
	check(MessagesVerticalBox);
	check(UserInputTextBox);
	check(ChatGPTModelComboBox);

	SendMessageButton->OnClicked.AddDynamic(this, &ThisClass::OnSendMessage);
	DumpChatButton->OnClicked.AddDynamic(this, &ThisClass::OnDumpChat);
	UserInputTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnTextCommitted);
	UserInputTextBox->OnTextChanged.AddDynamic(this, &ThisClass::OnTextChanged);

	SendMessageButton->SetIsEnabled(false);
	DumpChatButton->SetIsEnabled(false);

	MessagesVerticalBox->ClearChildren();
	StartVerticalBox->SetVisibility(ESlateVisibility::Visible);
	ChatScrollBox->SetVisibility(ESlateVisibility::HitTestInvisible);

	InitChatGPT();
	InitModelsComboBox();
	
	FSlateApplication::Get().SetKeyboardFocus(UserInputTextBox->GetCachedWidget());
}

void UOpenAIWidget::InitChatGPT()
{
	const auto Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append("OpenAIAuth.ini"));
	const auto Model = UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_3_5_Turbo);
	
	ChatGPT = NewObject<UChatGPT>();
	check(ChatGPT);
	ChatGPT->SetAuth(Auth);
	ChatGPT->SetMaxTokens(MaxTokens);
	ChatGPT->SetModel(Model);
	ChatGPT->OnRequestCompleted().AddUObject(this, &ThisClass::OnRequestCompleted);
	ChatGPT->OnRequestUpdated().AddUObject(this, &ThisClass::OnRequestUpdated);
	
	// SaveSettings = USaveSettings::Load();
	// check(SaveSettings);
}

void UOpenAIWidget::OnSendMessage()
{
	const auto InputText = UserInputTextBox->GetText();
	UserInputTextBox->SetText({});

	FMessage Message;
	Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::User);

	Message.Content = InputText.ToString();
	
	ChatGPT->AddMessage(Message);

	FMessage AssistantMessage;
	AssistantMessage.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant);
	ChatGPT->SetAssistantMessage(AssistantMessage);

	ChatGPT->MakeRequest();
}

void UOpenAIWidget::OnRequestCompleted()
{
	// ChatScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible);
	// EnableControls(true);
	// FSlateApplication::Get().SetKeyboardFocus(UserInputTextBox->GetCachedWidget());
}

void UOpenAIWidget::OnRequestUpdated(const FMessage& Message, bool WasError)
{
	// ChatWidgets.Last()->SetMessage(Message, WasError);
	// ChatScrollBox->ScrollToEnd(); 
}