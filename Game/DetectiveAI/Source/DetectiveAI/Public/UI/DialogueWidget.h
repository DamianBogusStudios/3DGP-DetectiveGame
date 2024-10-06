// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"


class USizeBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> SpeechBox;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> ReplyBox;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SpeechText;
	


	UFUNCTION(BlueprintCallable)
	void Speak(FString Text);


	UFUNCTION()
	void ToggleBoxes(bool IsSpeaking);
};
