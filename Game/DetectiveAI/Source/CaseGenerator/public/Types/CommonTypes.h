#pragma once

#include "CoreMinimal.h"
#include "CommonTypes.generated.h"

USTRUCT(BlueprintType)
struct FDialogueOptions
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString OptionOne;
	
	UPROPERTY(BlueprintReadWrite)
	FString OptionTwo;

	UPROPERTY(BlueprintReadWrite)
	FString OptionThree;

	TSharedPtr<FJsonObject> ToJsonObject() const
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		JsonObject->SetStringField("OptionOne", OptionOne);
		JsonObject->SetStringField("OptionTwo", OptionTwo);
		JsonObject->SetStringField("OptionThree", OptionThree);
		return JsonObject;
	}
};
