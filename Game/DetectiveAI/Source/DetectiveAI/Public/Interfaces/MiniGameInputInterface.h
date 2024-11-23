// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MiniGameInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UMiniGameInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DETECTIVEAI_API IMiniGameInputInterface
{
	GENERATED_BODY()

public:

	virtual void HandleApplyTension_MiniGame(float Value) = 0;
	virtual void HandleApplyTensionCompleted_MiniGame() = 0;
	virtual void HandleRaisePin_MiniGame(float Value) = 0;
	virtual void HandleRaisePinCompleted_MiniGame() = 0;
	virtual void HandleMovePin_MiniGame(bool Right) = 0;
	
};
