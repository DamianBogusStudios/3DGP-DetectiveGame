// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Management/HttpGPTSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GPTRealtimeSystem.generated.h"

class IWebSocket;
/**
 * 
 */
UCLASS()
class HTTPGPTVOICEMODULE_API UGPTRealtimeSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return false; // Prevents the subsystem from being created
	}
	
	TSharedPtr<IWebSocket> WebSocket;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:

	static FString GetEndpointURL()
	{
		return TEXT("wss://api.openai.com/v1/realtime?model=gpt-4o-realtime-preview-2024-10-01");
	}

	static FString GetAPIKey()
	{
		if(auto Settings = GetDefault<UHttpGPTSettings>())
		{
			return Settings->CommonOptions.APIKey.ToString();
		}
		return "";
	}

	void OnConnected();
	void OnConnectionError(const FString& ErrorMessage);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessageReceived(const FString& Message);
	
};
