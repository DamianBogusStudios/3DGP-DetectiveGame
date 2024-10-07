// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueReplyObject.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogueReplyEvent);
/**
 * 
 */
UCLASS(BlueprintType)
class DETECTIVEAI_API UDialogueReplyObject : public UObject
{
	GENERATED_BODY()

public:

	UDialogueReplyObject();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueReplyObject")
	FString Reply;

	UPROPERTY(BlueprintAssignable, Category = "DialogueReplyObject")
	FDialogueReplyEvent ReplyEvent;
};
