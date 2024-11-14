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
enum class EClueType : uint8
{
	WitnessTestimony,
	Forensic,
	Item,
	DigitalRecording,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EActorGender : uint8
{
	Male,
	Female,
	Unknown
};

UENUM(BlueprintType)
enum class EEyeColour : uint8
{
	Brown,
	Blue,
	Green,
	Grey
};

UENUM(BlueprintType)
enum class EHairColour : uint8
{
	Black,
	Brown,
	Blonde,
	Red,
	Grey
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

USTRUCT(BlueprintType)
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

USTRUCT(BlueprintType)
struct CASEGENERATOR_API FActorDescription
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (LLMDescription = "Full name of the Actor"))
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActorGender Gender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEyeColour EyeColour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHairColour HairColour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Age;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMyersBriggsType MyersBriggsType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (LLMDescription = "Roles that the character has in the stories, can have multiple"))
	TArray<EActorRole> Roles;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVictimFamiliarity VictimFamiliarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (LLMDescription = "Description of the current world and knowledge from the perspective of this character. Some characters may know more than others. can be later used in dialogue to acquire information"))
	FString Context;

	FString ToString()
	{
		FString Result = FString::Printf(TEXT("Name: %s\nGender: %s\nEye Colour: %s\nHair Colour: %s\nAge: %d\nMyers-Briggs Type: %s\nRoles: "),
									 *Name,
									 *UEnum::GetValueAsString(Gender),
									 *UEnum::GetValueAsString(EyeColour),
									 *UEnum::GetValueAsString(HairColour),
									 Age,
									 *UEnum::GetValueAsString(MyersBriggsType));
    
		for (const EActorRole& Role : Roles)
		{
			Result += UEnum::GetValueAsString(Role) + TEXT(", ");
		}
		Result.RemoveFromEnd(TEXT(", ")); 

		Result += FString::Printf(TEXT("\nVictim Familiarity: %s\nContext: %s"),
								  *UEnum::GetValueAsString(VictimFamiliarity),
								  *Context);
		return Result;
	}
};

USTRUCT(BlueprintType)
struct CASEGENERATOR_API FClue
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (LLMDescription = "The type of clue that this is"))
	EClueType ClueType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (LLMDescription = "detailed description of the context of this clue in the case"))
	FString Description;
};

USTRUCT(BlueprintType)
struct CASEGENERATOR_API FClueCollection
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FClue> Clues;
};

USTRUCT(BlueprintType)
struct CASEGENERATOR_API FActorContext
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString KnowledgeBase;
};
	
USTRUCT(BlueprintType, meta = (LLMDescription = "A collection for each actors knowledge base. it includes details about clues and other actors in a narratively cohesive manner"))
struct CASEGENERATOR_API FContextCollection
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (LLMDescription = "Same order as generated actors, index[0] == victim, etc."))
	TArray<FActorContext> KnowledgeBases;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FClue> Clues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (LLMDescription = "Description of the current world. In this object the knowledge is absolute and contains all details about the case."))
	FString Context;
};

#pragma region DELEGATES

DECLARE_DELEGATE_OneParam(FMessageDelegate,
	FString& /* Message */ );

DECLARE_DELEGATE_TwoParams(FStructuredMessageDelegate,
	FString& /*Message*/,
	UScriptStruct* /* StructSchema*/);


DECLARE_DELEGATE_TwoParams(FErrorReceivedDelegate,
	FString& /*Message*/,
	UScriptStruct* /* StructSchema*/);

DECLARE_DYNAMIC_DELEGATE_FiveParams(FFunctionCallDelegate,
	UObject*, Caller,
	FString&, Message,
	FName&, FunctionName,
	TArray<FString>&, ArgumentNames,
	TArray<FString>&, ArgumentValues);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDialogueOptionsDelegate,
	FDialogueOptions&, DialogueOptions);

#pragma endregion 