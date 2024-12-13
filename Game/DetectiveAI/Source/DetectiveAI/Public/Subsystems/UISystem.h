// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISystem.generated.h"


class UWidgetClassSettings;
class UActorComponent;

UENUM(BlueprintType)
enum class EUIElementType : uint8
{
	NPCDialogue,
	LockpickMiniGame,
	PauseMenu,
	HUD,
	Loading,
	MicFeedback,
	Pickup,
	GameEnd
};

USTRUCT(BlueprintType)
struct FWidgetInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> WidgetInstance;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUIActionDelegate, AActor*, Caller, UUserWidget*, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestInputDelegate, const TArray<EUIElementType>&, UIElements);
/**
 * 
 */
UCLASS()
class DETECTIVEAI_API UUISystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintAssignable)
	FUIActionDelegate OnStartUIAction;
	
	UPROPERTY(BlueprintAssignable)
	FUIActionDelegate OnAdvanceUIAction;

	UPROPERTY(BlueprintAssignable)
	FUIActionDelegate OnFinishUIAction;

	UPROPERTY(BlueprintAssignable)
	FRequestInputDelegate RequestInput;

	
	UFUNCTION(BlueprintCallable, meta = (Tooltip = "returns whether widget was successfully added"))
	UUserWidget* RequestStartWidget(UObject* InCaller, EUIElementType WidgetType);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "returns whether widget was successfully removed"))
	bool RequestFinishWidget(UObject* InCaller, EUIElementType WidgetType);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* GetWidget(EUIElementType WidgetType);

	UFUNCTION(BlueprintCallable)
	void Advance();
	UFUNCTION(BlueprintCallable)
	void Exit();
	UFUNCTION()
	void OnFinishedLoading();

	
protected:
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:

	UPROPERTY()
	TObjectPtr<UObject> ActiveUObject;

	UPROPERTY()
	TMap<EUIElementType, FWidgetInfo> WidgetMap;

	TArray<EUIElementType> WidgetQueue;

	UFUNCTION()
	void CheckInputNeeded() const;
	
	UFUNCTION()
	void LoadWidgetClasses();
	
	
	
};
