// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayStation/DualSenseController.h"
/*
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
#if PLATFORM_PS5
	FSonyInputInterface* SonyInputInterface = TryGetPlatformInputInterface();

	if(ensure(SonyInputInterafe !+ nullptr))
	{
		SonyInputInterface->SetLightColour(0, LightColour);
		UE_LOG(LogTemp, Warning, TEXT("Lightbar Colour Set"));
	}
#elif !PlATFORM_PS5
	UE_LOG(LogTemp, Warning, TEXT("%s SetLightBarColour()", *PlatformWarningPrefix));
#endif
}


void UDualSenseController::SetTriggerEffectProperty(int StartPos, int EndPos, int Strength, int Trigger)
{
	FPS5TriggeEffectProperty TriggerProperty(SetTrigger(Trigger));
	TriggerProperty.SetWeapon(StartPos, EndPos, Strength);
	TriggerProperty.SetUseEffectForThreshold(true);

	SendTriggerEffectProperty(TriggerProperty);
}



#endif
*/


void UDualSenseController::SetLightBarColour(const FColor& LightColour)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s SetLightBarColour()", *PlatformWarningPrefix));
}
void UDualSenseController::SetTriggerEffectProperty(int StartPos, int EndPos, int Strength, int Trigger)
{
	//do nothing
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