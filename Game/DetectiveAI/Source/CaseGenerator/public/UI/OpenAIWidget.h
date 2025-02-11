﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Provider/Types/CommonTypes.h"
//#include "ChatGPT/BaseService.h"
#include "OpenAIWidget.generated.h"

/*
class UVerticalBox;
class UScrollBox;
class UEditableText;
class UButton;
class UChatMessageWidget;
class UComboBoxString;
class UChatGPT;
class UServiceWidget;
class UGridPanel;
class USaveSettings;
class UTextBlock;
class UAttachedFilesContainerWidget;
*/

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UOpenAIWidget : public UUserWidget
{
	GENERATED_BODY()
/*
	protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "OpenAI")
    int32 MaxTokens{2000};

protected:
    
    UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "OpenAI")
    TObjectPtr<UEditableText> UserInputTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SendMessageButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> DumpChatButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UChatMessageWidget> ChatMessageWidgetClass;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ServicesWarningTextBlock;

    virtual void NativeConstruct() override;

private:
    UPROPERTY()
    TObjectPtr<UChatGPT> ChatGPT;

    UPROPERTY()
    TArray<TObjectPtr<UChatMessageWidget>> ChatWidgets;

    UPROPERTY()
    TObjectPtr<USaveSettings> SaveSettings;

private:
    UFUNCTION()
    void OnSendMessage();

    UFUNCTION()
    void OnDumpChat();

    UFUNCTION()
    void OnClearChat();

    UFUNCTION()
    void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    UFUNCTION()
    void OnTextChanged(const FText& Text);

    void InitChatGPT();
    void InitModelsComboBox();

    void OnRequestCompleted();
    void OnRequestUpdated(const FMessage& Message, bool WasError);

    void CreateMessageWidget(const FMessage& Message);
    void EnableControls(bool Enabled);

    FString GenerateFilePath() const;
*/
};
