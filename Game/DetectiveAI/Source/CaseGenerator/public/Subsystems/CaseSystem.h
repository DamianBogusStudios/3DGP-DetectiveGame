// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Types/CommonCaseTypes.h"
#include "CaseSystem.generated.h"

class ILLMService;

/**
 * 
 */
UCLASS()
class CASEGENERATOR_API UCaseSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	UPROPERTY()
	TScriptInterface<ILLMService> LLMService;

	FCaseFile CaseFile;
	TArray<FString> GeneratedActorSchemas;
	
	int NumberOfActors;

	void BindServiceCallbacks();
	void GenerateCase();
	void GenerateActor();
	void GenerateConnections();
	void RandomiseInitialParams();

	/*callbacks*/
	
	UFUNCTION()
	void MessagedReceived(UObject* Caller, FString& Message);
	UFUNCTION()
	void StructuredMessageReceived(UObject* Caller, FString& Message, UScriptStruct* Struct);
	UFUNCTION()
	void FunctionCallReceived(UObject* Caller, FString& Message, FName& FunctionName, TArray<FString>& ArgNames, TArray<FString>& ArgValues);
};
