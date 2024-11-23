// Fill out your copyright notice in the Description page of Project Settings.


#ifndef PLATFORM_PS5
#define PLATFORM_PS5 0 // Define it as 0 (false)
#endif

#include "PlayStation/DualSenseController.h"



#if PLATFORM_PS5
PS5 DevKit

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

	if(ensure(SonyInputInterafe !+ nullptr))
	{
		SonyInputInterface->SetLightColour(0, LightColour);
		UE_LOG(LogTemp, Warning, TEXT("Lightbar Colour Set"));
	}
}
/*
Resistance	Simulates constant resistance for tension mechanics like pulling or braking.
Weapon	Adds a click or recoil for shooting or mechanical actions.
Vibration	Simulates vibrations like engine or terrain feedback.
Custom	Use for complex, multi-layered interactions requiring zones of resistance.
Feedback	Basic tactile feedback for general actions.
*/
void UDualSenseController::SetTriggerEffectProperty(uint8 StartPos, uint8 EndPos, uint8 Strength, int Trigger,int Effect)
{
	FPS5TriggerEffectProperty TriggerProperty(SetTrigger(Trigger));

	switch (TriggerEffect.Effect)
	{
	case ETriggerEffect::Weapon:
		TriggerProperty.SetWeapon(StartPos, EndPos, Strength);
		break;

	case ETriggerEffect::Vibration:
		TriggerProperty.SetVibration(StartPos, Strength);
		break;

	case ETriggerEffect::Feedback:
		TriggerProperty.SetFeedback(StartPos, EndPos, Strength);
		break;
	}
	TriggerProperty.SetWeapon(StartPos, EndPos, Strength);
	TriggerProperty.SetUseEffectForThreshold(true);

	SendTriggerEffectProperty(TriggerProperty);
}

void UDualSenseController::ResetTriggers()
{
	// FPS5TriggerEffectProperty TriggerPropertyLeft(SetTrigger(static_cast<int>(ETrigger::Left)));
	// FPS5TriggerEffectProperty TriggerPropertyRight(SetTrigger(static_cast<int>(ETrigger::Right)));
	//
	// // Reset effects for both triggers
	// TriggerPropertyLeft.ResetEffect();
	// TriggerPropertyRight.ResetEffect();
	//
	// // Send the reset properties to the DualSense controller
	// SendTriggerEffectProperty(TriggerPropertyLeft);
	// SendTriggerEffectProperty(TriggerPropertyRight);
}
#endif


void UDualSenseController::SetLightBarColour(const FColor& LightColour)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s SetLightBarColour()", *PlatformWarningPrefix));
}
void UDualSenseController::SetTriggerEffectProperty(uint8 StartPos, uint8 EndPos, uint8 Strength, int Trigger,int Effect)
{
	UE_LOG(LogTemp, Warning, TEXT("Trigger Effect Applied"));
}
void UDualSenseController::ResetTriggers()
{
	UE_LOG(LogTemp, Warning, TEXT("Trigger Effect Reset"));
}

// FPS5TriggerEffectProperty::TriggerMask SetTrigger(int trigger)
// {
// 	FPS5TriggerEffectProperty Property;
//
// 	switch (trigger)
// 	{
// 	case 0:
// 		Property.Triggers = FPS5TriggerEffectProperty::TriggerMask::None;
// 		break;
// 	case 1:
// 		Property.Triggers = FPS5TriggerEffectProperty::TriggerMask::Left;
// 		break;
// 	case 2:
// 		Property.Triggers = FPS5TriggerEffectProperty::TriggerMask::Right;
// 		break;
// 	case 3:
// 		Property.Triggers = FPS5TriggerEffectProperty::TriggerMask::Both;
// 		break;
// 	}
//
// 	return Property.Triggers;
// }
//
// bool SendTriggerEffectProperty(const FPS5TriggerEffectProperty& DeviceProperty)
// {
// 	FSonyInputInterface* SonyInputInterface = TryGetPlayformInputInterface();
//
// 	if(SonyInputInterface == nullptr)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("no interface found"));
// 		return false;
// 	}
//
// 	SonyInputInterface->SetDeviceProperty(0, &DeviceProperty);
// 	return true;
// }