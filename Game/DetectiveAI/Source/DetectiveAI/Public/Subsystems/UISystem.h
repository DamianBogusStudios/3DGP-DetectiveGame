// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUIActionDelegate, AActor*, Caller, UUserWidget*, Widget);


class UInteractionSettings;

UENUM(BlueprintType)
enum class EUIElementType : uint8
{
	NPCDialogue,
	LockpickMiniGame,
	PauseMenu,
	HUD,
	Loading
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

	
	UFUNCTION(BlueprintCallable, meta = (Tooltip = "returns whether widget was successfully added"))
	bool RequestStartWidget(AActor* InCaller, EUIElementType WidgetType);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "returns whether widget was successfully removed"))
	bool RequestFinishWidget(AActor* InCaller, EUIElementType WidgetType);
	
	UFUNCTION()
	void OnFinishedLoading();
	
protected:
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:

	UPROPERTY()
	TObjectPtr<AActor> ActiveActor;

	UPROPERTY()
	TMap<EUIElementType, FWidgetInfo> WidgetMap;


	UFUNCTION()
	void LoadWidgetClasses();
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* GetWidget(EUIElementType WidgetType);
	
	
	
	
};
