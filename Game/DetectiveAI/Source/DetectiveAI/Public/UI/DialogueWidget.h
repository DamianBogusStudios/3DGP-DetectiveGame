// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/WidgetInterface.h"
#include "DialogueWidget.generated.h"


class UDialogueComponent;
class UButton;
class UEditableTextBox;
class USizeBox;
class UTextBlock;
class UListView;
class UActorComponent;

UENUM(BlueprintType)
enum class EDialogueVisibility : uint8
{
	EBotSpeaking,
	EPlayerReply,
	EHidden
};

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UDialogueWidget : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> SpeechBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> ReplyBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SpeechText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> InputTextBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SendMessageBtn;

	UFUNCTION(BlueprintCallable)
	void ResetDialogueWidget();

	UFUNCTION()
	void TranscriptReceived(const FString& Message);

	virtual void Advance_Implementation() override;
	virtual void Setup_Implementation(UObject* Caller) override;
	virtual void Cleanup_Implementation() override;
	
protected:

	UPROPERTY()
	TObjectPtr<UDialogueComponent> DialogueComponent;
	
	EDialogueVisibility WidgetVisibility;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void Speak(FString Text);
	
	UFUNCTION(BlueprintCallable)
	void Reply();

	UFUNCTION()
	void ToggleBoxes(EDialogueVisibility DialogueVisibility);

	UFUNCTION()
	void SpeechReceived(const FString& Message);

	
};
