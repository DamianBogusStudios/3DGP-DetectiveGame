#pragma once

#include "CoreMinimal.h"
#include "CommonCaseTypes.generated.h"

/* used for runtime lookup */
class USoundWaveProcedural;

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
	Victim,
	PoliceOfficer,
	Witness,
	Suspect,
	Bystander,
	Friend,
	Partner,
	Acquaintance,
	Stranger,
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

// UENUM(BlueprintType)
// enum class EVictimFamiliarity : uint8
// {
// 	Stranger,
// 	Acquaintance,
// 	Coworker,
// 	Friend,
// 	Partner,
// 	Victim
// };

UENUM(BlueprintType)
enum class ERoom : uint8
{
	Atrium,
	Corridor,
	Bathroom,
	Bedroom,
	MasterBedroom,
	DiningRoom,
	Kitchen,
	Study,
	MAX UMETA(Hidden)
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
	EActorGender Gender = EActorGender::Male;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEyeColour EyeColour = EEyeColour::Brown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHairColour HairColour = EHairColour::Brown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Age = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMyersBriggsType MyersBriggsType = EMyersBriggsType::ENFJ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (LLMDescription = "Roles that the character has in the stories, can have multiple"))
	TArray<EActorRole> Roles;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// EVictimFamiliarity VictimFamiliarity = EVictimFamiliarity::Stranger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (LLMDescription = "Description of the current world and knowledge from the perspective of this character. Some characters may know more than others. can be later used in dialogue to acquire information"))
	FString Context;

	FString ToString() const
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

		// Result += FString::Printf(TEXT("\nVictim Familiarity: %s\nContext: %s"),
		// 						  *UEnum::GetValueAsString(VictimFamiliarity),
		// 						  *Context);
		
		Result += FString::Printf(TEXT("\nContext: %s"), *Context);
		return Result;
	}
};

USTRUCT(BlueprintType)
struct CASEGENERATOR_API FClue
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (LLMDescription = "The type of clue that this is"))
	EClueType ClueType = EClueType::Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (LLMDescription = "Where the clue is located"))
	ERoom Room = ERoom::Atrium;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (LLMDescription = "detailed description of the context of this clue in the case"))
	FString Description;

	FString ToString() const
	{
		FString Result = FString::Printf(TEXT("Clue Type: %s\nRoom: %s\nDescription: %s"),
									 *UEnum::GetValueAsString(ClueType),
									 *UEnum::GetValueAsString(Room),
									 *Description);
		return Result;
	}
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
	EMotive Motive = EMotive::PersonalVendetta;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMurderWeapon MurderWeapon = EMurderWeapon::Arsenic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoom MurderRoom = ERoom::Atrium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FActorDescription> Actors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FClue> Clues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (LLMDescription = "Description of the current world. In this object the knowledge is absolute and contains all details about the case."))
	FString Context;

	FString ToString() const
	{
		const FString MotiveStr = UEnum::GetDisplayValueAsText(Motive).ToString();
		const FString WeaponStr = UEnum::GetDisplayValueAsText(MurderWeapon).ToString();
		const FString RoomStr = UEnum::GetDisplayValueAsText(MurderRoom).ToString();

		FString ActorsStr;
		for (const auto& Actor : Actors)
		{
			ActorsStr += Actor.ToString() + TEXT("\n,");
		}
		if (ActorsStr.Len() > 3)
		{
			ActorsStr = ActorsStr.LeftChop(3);
		}

		FString CluesStr;
		for (const FClue& Clue : Clues)
		{
			CluesStr += Clue.ToString() + TEXT("\n,");
		}
		if (CluesStr.Len() > 3)
		{
			CluesStr = CluesStr.LeftChop(3);
		}

		return FString::Printf(
			TEXT("FCaseFile:\nMotive: %s\nMurder Weapon: %s\nMurder Room: %s\nActors: [%s]\nClues: [%s]\nContext: %s"),
			*MotiveStr,
			*WeaponStr,
			*RoomStr,
			*ActorsStr,
			*CluesStr,
			*Context
		);
	}
};

#pragma region DELEGATES

DECLARE_DELEGATE_OneParam(FMessageDelegate,
FString& /* Message */ );

DECLARE_DELEGATE_OneParam(FTranscriptDelegate,
FString& /* Message */ );

DECLARE_DELEGATE_OneParam(FVoiceDelegate,
	USoundWaveProcedural* /* VoiceAudioData */ );

DECLARE_DELEGATE_TwoParams(FStructuredMessageDelegate,
	FString& /*Message*/,
	UScriptStruct* /* StructSchema*/);

DECLARE_DELEGATE_TwoParams(FErrorReceivedDelegate,
	FString& /*Message*/,
	UScriptStruct* /* StructSchema*/);

DECLARE_DELEGATE_TwoParams(FFunctionCallDelegate,
	FName& /*FunctionName*/,
	TArray<FString> /*ArgumentValues*/);

#pragma endregion 