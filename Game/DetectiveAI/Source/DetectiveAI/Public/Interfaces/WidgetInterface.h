// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DETECTIVEAI_API IWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	void Cleanup();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	void Setup(UObject* Caller);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	void Advance();


};
