

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/DialogueHandler.h"
#include "GameFramework/PlayerController.h"
#include "MPlayerController.generated.h"


class APawn;
class ABaseCharacter;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EInputContext : uint8
{
	Default,
	Dialogue,
	Menu
};

/**
 * 
 */
UCLASS()
class DETECTIVEAI_API AMPlayerController : public APlayerController, public IDialogueHandler
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TMap<EInputContext, TObjectPtr<UInputMappingContext>> InputContextMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Default")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Default")
	UInputAction* InteractAction;

	UPROPERTY()
	TObjectPtr<ABaseCharacter> BaseCharacter;

	/* IDialogueHandler */
	void StartDialogue_Implementation(AActor* Caller, TSubclassOf<UUserWidget> WidgetClass);



protected:

	virtual void SetupInputComponent() override;

	virtual void BeginPlay();

	void OnMoveInput(const FInputActionValue& Value);

	void OnInteractInput(const FInputActionValue& Value);

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:

	FRotator CachedCameraRotation;	

	void BindDefaultContextActions();


};
