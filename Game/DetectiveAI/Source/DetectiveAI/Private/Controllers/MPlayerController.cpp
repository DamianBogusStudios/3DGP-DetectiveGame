


#include "Controllers/MPlayerController.h"
#include "Characters/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Subsystems/InteractionSystem.h"


void AMPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* ControlledPawn = GetPawn())
	{
		if(UCameraComponent* Camera = ControlledPawn->GetComponentByClass<UCameraComponent>())
		{
			CachedCameraRotation = FRotator(0, Camera->GetComponentRotation().Yaw, 0);
		}
	}

	if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
	{
	 	InteractionSubsystem->OnRequestDialogueAction.AddDynamic(this, &AMPlayerController::OnDialogueRequested);
	}

}
void AMPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);


	if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
	{
		InteractionSubsystem->OnRequestDialogueAction.RemoveDynamic(this, &AMPlayerController::OnDialogueRequested);
	}
}

void AMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputContextMap.Contains(EInputContext::Default) && InputContextMap[EInputContext::Default])
		{
			SubSystem->AddMappingContext(InputContextMap[EInputContext::Default], 0);
		}
	}

	BindDefaultContextActions();
}

void AMPlayerController::BindDefaultContextActions()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMPlayerController::OnMoveInput);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMPlayerController::OnInteractInput);
	}
}


void AMPlayerController::OnMoveInput(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	const FVector ForwardDir = FRotationMatrix(CachedCameraRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(CachedCameraRotation).GetUnitAxis(EAxis::Y);

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		ControlledPawn->AddMovementInput(ForwardDir * Input.Y);
		ControlledPawn->AddMovementInput(RightDir * Input.X);
	}
}

void AMPlayerController::OnInteractInput(const FInputActionValue& Value)
{
	if (BaseCharacter)
	{
		BaseCharacter->Interact();
	}
}

void AMPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BaseCharacter = Cast<ABaseCharacter>(InPawn);
}

void AMPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	BaseCharacter = nullptr;
}


void AMPlayerController::OnDialogueRequested(APawn* Caller, UBehaviorTree* BT, UDialogueWidget* WidgetClass)
{
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputContextMap.Contains(EInputContext::Dialogue) && InputContextMap[EInputContext::Dialogue])
		{
			SubSystem->AddMappingContext(InputContextMap[EInputContext::Dialogue].Get(), 1);
		}
	}
}