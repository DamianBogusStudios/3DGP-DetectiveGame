#pragma once

#include "CoreMinimal.h"
#include "CommonCaseTypes.generated.h"

UENUM(BlueprintType)
enum class EGender : uint8
{
	Male,
	Female,
	Unknown
};

UENUM(BlueprintType)
enum class EMyersBriggsType : uint8
{
	/*will most likely shorten this.*/
	INTJ,	INTP,	ENTJ,	ENTP,
	INFJ,	INFP,	ENFJ,	ENFP,
	ISTJ,	ISFJ,	ESTJ,	ESFJ,
	ISTP,	ISFP,	ESTP,	ESFP
};

UENUM(BlueprintType)
enum class EActorRole : uint8
{
	PoliceOfficer,
	Detective,
	Witness,
	Suspect,
	Bystander,
	Friend
};

UENUM(BlueprintType)
enum class EPlayerFamiliarity : uint8
{
	Stranger,
	Acquaintance,
	Coworker,
	Friend,
	Partner
};

USTRUCT(BlueprintType, meta = (Tooltip = "dialogue options for player to pick when interacting with this NPC"))
struct CASEGENERATOR_API  FDialogueOptions
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Meta = (Tooltip = "First Dialogue Option"))
	FString OptionOne;
	
	UPROPERTY(BlueprintReadOnly, Meta = (Tooltip = "Second Dialogue Option"))
	FString OptionTwo;

	UPROPERTY(BlueprintReadOnly, Meta = (Tooltip = "Third Dialogue Option"))
	FString OptionThree;
};

USTRUCT(BlueprintType)
struct CASEGENERATOR_API FActorDescription
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGender Gender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Age;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMyersBriggsType MyersBriggsType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EActorRole> Roles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerFamiliarity PlayerFamiliarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Context; /* description of current world and actor state */
};

#pragma region DELEGATES

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMessageDelegate,
	UObject*, Caller,
	FString&, Message);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStructuredMessageDelegate,
	UObject*, Caller,
	FString&, Message,
	UScriptStruct*, StructSchema);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueOptionsDelegate,
	UObject*, Caller,
	FDialogueOptions&, DialogueOptions);
#pragma endregion 