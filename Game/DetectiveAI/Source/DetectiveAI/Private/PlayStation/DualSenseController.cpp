// Fill out your copyright notice in the Description page of Project Settings.

DECLARE_LOG_CATEGORY_EXTERN(LogDualSense, Log, All);
DEFINE_LOG_CATEGORY(LogDualSense);

#ifndef PLATFORM_PS5
#define PLATFORM_PS5 0 // Define it as 0 (false)
#endif

#include "PlayStation/DualSenseController.h"



#if PLATFORM_PS5

#include "SonyApplication.h"
#include "PS5Application.h"
#include "PS5Controllers.h"

FSonyInputInterface* TryGetPlatformInputInterface()
{
	if(FPS5Application::IsInitialized())
		return reinterpret_cast<FSonyInputInterface*>(FPS5Application::GetPS5Application()->GetInputInterface());
	else
		return nullptr;
}
void UDualSenseController::SetLightBarColour(const FColor& LightColour)
{
	FSonyInputInterface* SonyInputInterface = TryGetPlatformInputInterface();

	if(ensure(SonyInputInterface != nullptr))
	{
		SonyInputInterface->SetLightColor(0, LightColour);
		LogFunctionCall(__FUNCTION__, LightColour.ToString());
	}
}

FPS5TriggerEffectProperty::TriggerMask SetTrigger(int trigger)
{
	FPS5TriggerEffectProperty Property;

	switch (trigger)
	{
	case 0:
		Property.Triggers = FPS5TriggerEffectProperty::TriggerMask::None;
		break;
	case 1:
		Property.Triggers = FPS5TriggerEffectProperty::TriggerMask::Left;
		break;
	case 2:
		Property.Triggers = FPS5TriggerEffectProperty::TriggerMask::Right;
		break;
	case 3:
		Property.Triggers = FPS5TriggerEffectProperty::TriggerMask::Both;
		break;
	}

	return Property.Triggers;
}

bool SendTriggerEffectProperty(const FPS5TriggerEffectProperty&  DeviceProperty)
{
	if (FSonyInputInterface* SonyInputInterface = TryGetPlatformInputInterface())
	{
		SonyInputInterface->SetDeviceProperty(0, &DeviceProperty);
		return true;
	}
	return false;
}

/*
Resistance	Simulates constant resistance for tension mechanics like pulling or braking.
Weapon	Adds a click or recoil for shooting or mechanical actions.
Vibration	Simulates vibrations like engine or terrain feedback.
Custom	Use for complex, multi-layered interactions requiring zones of resistance.
Feedback	Basic tactile feedback for general actions.
*/
void UDualSenseController::SetTriggerEffectProperty(uint8 StartPos, uint8 EndPos, uint8 Strength, int Trigger, int Effect)
{
	FPS5TriggerEffectProperty TriggerProperty;
	TriggerProperty.Triggers = SetTrigger(Trigger);

	switch (Effect)
	{
	case 1:
		TriggerProperty.SetVibration(StartPos, EndPos, Strength);
		break;
	default:
		TriggerProperty.SetWeapon(StartPos, EndPos, Strength);
		break;
	}

	TriggerProperty.SetUseEffectForThreshold(true);
	bool bSuccess = SendTriggerEffectProperty(TriggerProperty);

	FString Msg = bSuccess ? "Success" : "Failed";
	LogFunctionCall(__FUNCTION__, Msg);
}

void UDualSenseController::ResetTrigger(int Trigger)
{
	FPS5TriggerEffectProperty TriggerProperty(SetTrigger(Trigger));	
	TriggerProperty.SetOff();
	bool bSuccess = SendTriggerEffectProperty(TriggerProperty);

	FString Msg = bSuccess ? "Success" : "Failed";
	LogFunctionCall(__FUNCTION__, Msg);
}

#else


void UDualSenseController::SetLightBarColour(const FColor& LightColour)
{
	LogFunctionCall(__FUNCTION__, LightColour.ToString());
}
void UDualSenseController::SetTriggerEffectProperty(uint8 StartPos, uint8 EndPos, uint8 Strength, int Trigger,int Effect)
{
	FString Msg = FString::Printf( TEXT("StartPos: %d, EndPos: %d, Strength: %d, Trigger: %d, Effect: %d"),
		StartPos, EndPos, Strength, Trigger, Effect);
	LogFunctionCall(__FUNCTION__, Msg);
}
void UDualSenseController::ResetTrigger(int Trigger)
{
	LogFunctionCall(__FUNCTION__, FString::FromInt(Trigger));
}


#endif


void UDualSenseController::LogFunctionCall(const FString& CallingFunction, FString Message)
{
#if PLATFORM_PS5
	UE_LOG(LogDualSense, Log, TEXT("PS5: %s %s"), *CallingFunction, *Message);
#else
	UE_LOG(LogDualSense, Log, TEXT("Non-PS5 Platform: %s %s"), *CallingFunction, *Message);
#endif
}

