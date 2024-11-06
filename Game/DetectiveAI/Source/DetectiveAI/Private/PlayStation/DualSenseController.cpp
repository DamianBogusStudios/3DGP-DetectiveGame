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

#endif
*/


void UDualSenseController::SetLightBarColour(const FColor& LightColour)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s SetLightBarColour()", *PlatformWarningPrefix));
}
