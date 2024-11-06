// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUIActionDelegate, AActor*, Caller, UUserWidget*, Widget);

class UInteractionSettings;

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
	bool RequestStartWidget(AActor* InCaller, EWidgetType WidgetType);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "returns whether widget was successfully removed"))
	bool RequestFinishWidget(AActor* InCaller, EWidgetType WidgetType);


protected:
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:

	UUserWidget* GetWidget(EWidgetType WidgetType);
	
	TObjectPtr<AActor> ActiveActor;
	TMap<EWidgetType, TObjectPtr<UUserWidget>> WidgetMap;
	
	
	
	
};
