


#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Subsystems/UISystem.h"
#include "MainPlayerController.generated.h"


enum class EUIElementType : uint8;
class IMiniGameInputInterface;
class UDualSenseController;
class UDialogueWidget;
class APawn;
class AMainCharacter;
class UInputMappingContext;
class UInputAction;
class UBehaviorTree;
struct FInputActionValue;


UENUM(BlueprintType)
enum class EInputContext : uint8 
{
	/* entries are ordered by mapping priority*/
	DefaultContext = 0, 
	UI = 1,
	Dialogue = 2,
	MiniGame = 3,
	MAX UMETA(Hidden)
};
ENUM_RANGE_BY_FIRST_AND_LAST(EInputContext, EInputContext::DefaultContext, EInputContext::MiniGame);

UENUM(BlueprintType)
enum class ETrigger: uint8
{
	None = 0,
	Left = 1,
	Right = 2,
	Both = 3
};
UENUM(BlueprintType)
enum class ETriggerEffect: uint8
{
	Weapon = 0,
	Vibration = 1,
	Feedback = 2,
	Custom = 3
};
USTRUCT(BlueprintType)
struct FDefaultInputActions
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* Interact;
};

USTRUCT(BlueprintType)
struct FUIInputActions
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* Advance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* Exit;
};

USTRUCT(BlueprintType)
struct FMiniGameInputActions
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* ApplyTension;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* RaisePin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MovePin;
};

USTRUCT()
struct FAdaptiveTriggerEffect
{
	GENERATED_BODY()

	ETrigger Trigger = ETrigger::None;
	ETriggerEffect Effect = ETriggerEffect::Weapon;
	float Strength = 1.0f;  
	float StartPos = 0.0f;
	float EndPos = 1.0f;    

	FAdaptiveTriggerEffect(ETrigger InTrigger, ETriggerEffect InEffect, float InStrength, float InStartPos, float InEndPos)
		: Trigger(InTrigger)
		, Effect(InEffect)
		, Strength(FMath::Clamp(InStrength, 0.0f, 1.0f)) 
		, StartPos(FMath::Clamp(InStartPos, 0.0f, 1.0f)) 
		, EndPos(FMath::Clamp(InEndPos, 0.0f, 1.0f))     
	{
	}

	FAdaptiveTriggerEffect() {}
};


/**
 * 
 */
UCLASS()
class DETECTIVEAI_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMainPlayerController();

	UPROPERTY()
	TScriptInterface<IMiniGameInputInterface> MiniGameInputInterface;
	
	UFUNCTION()
	void OnGameLoaded();	
	UFUNCTION()
	void SetAdaptiveTriggerEffect(FAdaptiveTriggerEffect TriggerEffect);
	UFUNCTION()
	void ResetTriggerEffect();
	UFUNCTION()
	void RequestInputContext(EInputContext Context, bool bRemove = false);
	UFUNCTION()
	void RequestUIInput(const TArray<EUIElementType>& UIElements);
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TMap<EInputContext, TObjectPtr<UInputMappingContext>> InputContextMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Default")
	FDefaultInputActions DefaultInputActions;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|UI")
	FUIInputActions UIInputActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|MiniGames")
	FMiniGameInputActions MiniGameInputActions;

	UPROPERTY()
	TObjectPtr<AMainCharacter> BaseCharacter;
	UPROPERTY()
	TObjectPtr<UDualSenseController> DualSense;
	
	FRotator CachedCameraRotation;	
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void SetupInputComponent() override;

	/* binding functions */
	void BindCallbacks();
	void BindDefaultActions();
	void BindUIActions();
	void BindMiniGameActions();

	/* default context */
	void HandleMove(const FInputActionValue& Value);
	void HandleInteract(const FInputActionValue& Value);

	/* ui context */
	void HandleAdvance_UI(const FInputActionValue& Value);
	void HandleExit_UI(const FInputActionValue& Value);

	void HandleApplyTension_MiniGame(const FInputActionValue& Value);
	void HandleApplyTensionCompleted_MiniGame(const FInputActionValue& Value);
	void HandleRaisePin_MiniGame(const FInputActionValue& Value);
	void HandleRaisePinCompleted_MiniGame(const FInputActionValue& Value);
	void HandleMovePin_MiniGame(const FInputActionValue& Value);

private:

	TSet<EInputContext> ActiveInputContexts;
	static TSet<EInputContext> UIToInputContext(EUIElementType Context);

	static bool ShouldSkipInputCheck(const EInputContext Context)
	{
		return (Context == EInputContext::MAX || Context == EInputContext::DefaultContext);
	}
	void UpdateMouseSettings(const bool bEnableMouse)
	{
		bShowMouseCursor = bEnableClickEvents = bEnableMouseOverEvents = bEnableMouse;
	}

	
};
