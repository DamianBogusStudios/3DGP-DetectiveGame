#pragma once

#include "CoreMinimal.h"
#include "CommonCaseTypes.generated.h"

/* used for runtime lookup */

USTRUCT(BlueprintType)
struct FDescriptionTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;
};

/***************************/


UENUM(BlueprintType)
enum class EMotive : uint8
{
	PersonalVendetta,
	FinancialGain,
	Power,
	Psychological,
	Accidental,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMurderWeapon : uint8
{
	Shotgun,
	Revolver,	
	Rifle,
	Explosive,
	Knife,
	IcePick,
	BrokenGlass,
	LetterOpener,	
	Dagger,
	Candlestick,
	Wrench,
	BaseballBat,
	MetalPipe,	
	Fists,
	Cyanide,
	Arsenic,
	DeadlyNightshade,
	MAX UMETA(Hidden)
};

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
	Witness,
	Suspect,
	Bystander,
	Friend,
	Victim
};

UENUM(BlueprintType)
enum class EPlayerFamiliarity : uint8
{
	Stranger,
	Acquaintance,
	Coworker,
	Friend,
	Partner,
};

UENUM(BlueprintType)
enum class EVictimFamiliarity : uint8
{
	Stranger,
	Acquaintance,
	Coworker,
	Friend,
	Partner,
	Victim
};

USTRUCT(BlueprintType, meta = (LLMDescription = "dialogue options for player to pick when interacting with this NPC"))
struct CASEGENERATOR_API  FDialogueOptions
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Meta = (LLMDescription = "First Dialogue Option"))
	FString OptionOne;
	
	UPROPERTY(BlueprintReadOnly, Meta = (LLMDescription = "Second Dialogue Option"))
	FString OptionTwo;

	UPROPERTY(BlueprintReadOnly, Meta = (LLMDescription = "Third Dialogue Option"))
	FString OptionThree;
};

USTRUCT(BlueprintType, meta = (LLMDescription = "Description of a character in the case."))
struct CASEGENERATOR_API FActorDescription
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (LLMDescription = "Full name of the Actor"))
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGender Gender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Age;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMyersBriggsType MyersBriggsType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (LLMDescription = "Roles that the character has in the stories, can have multiple"))
	TArray<EActorRole> Roles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerFamiliarity PlayerFamiliarity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVictimFamiliarity VictimFamiliarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (LLMDescription = "Description of the current world and knowledge from the perspective of this character. Some characters may know more than others. can be later used in dialogue to acquire information"))
	FString Context; 
};

USTRUCT(BlueprintType)
struct CASEGENERATOR_API FCaseFile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMotive Motive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMurderWeapon MurderWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FActorDescription> Actors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (LLMDescription = "Description of the current world. In this object the knowledge is absolute and contains all details about the case."))
	FString Context;
};

#pragma region DELEGATES

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMessageDelegate,
	UObject*, Caller,
	FString&, Message);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStructuredMessageDelegate,
	UObject*, Caller,
	FString&, Message,
	UScriptStruct*, StructSchema);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FFunctionCallDelegate,
	UObject*, Caller,
	FString&, Message,
	FName&, FunctionName,
	TArray<FString>&, ArgumentNames,
	TArray<FString>&, ArgumentValues);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDialogueOptionsDelegate,
	FDialogueOptions&, DialogueOptions);

#pragma endregion 