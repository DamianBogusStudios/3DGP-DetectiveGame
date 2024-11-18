


#include "Controllers/MPlayerController.h"
#include "Characters/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GameClasses/MGameInstance.h"
#include "PlayStation/DualSenseController.h"
// #include "Subsystems/InteractionSystem.h"
#include "Subsystems/UISystem.h"


void AMPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//if ps5 dev kit
	DualSense = NewObject<UDualSenseController>(this, UDualSenseController::StaticClass());
	DualSense->SetLightBarColour(FColor::Yellow);
	
	if (APawn* ControlledPawn = GetPawn())
	{
		if(UCameraComponent* Camera = ControlledPawn->GetComponentByClass<UCameraComponent>())
		{
			CachedCameraRotation = FRotator(0, Camera->GetComponentRotation().Yaw, 0);
		}
	}

	if(auto GameInstance = GetWorld()->GetGameInstance())
	{
		if(auto CustomGameInstance = Cast<UMGameInstance>(GameInstance))
		{
			CustomGameInstance->OnGameFinishedLoading.AddDynamic(this, &AMPlayerController::OnGameLoaded);
		}
	}
	
	if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
	{
	 	InteractionSubsystem->RequestInput.AddDynamic(this, &AMPlayerController::UIInputRequested);
	}

}
void AMPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);


	if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
	{
		InteractionSubsystem->RequestInput.RemoveDynamic(this, &AMPlayerController::UIInputRequested);
	}
}

void AMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	BindDefaultContextActions();
	BindUIContextActions();
}

void AMPlayerController::OnGameLoaded()
{
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputContextMap.Contains(EInputContext::Default) && InputContextMap[EInputContext::Default])
		{
			SubSystem->AddMappingContext(InputContextMap[EInputContext::Default], 0);
		}
	}

}

void AMPlayerController::BindDefaultContextActions()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMPlayerController::OnMoveInput);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMPlayerController::OnInteractInput);
	}
}
void AMPlayerController::BindUIContextActions()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(UIAdvanceAction, ETriggerEvent::Triggered, this, &AMPlayerController::OnAdvanceInput_UI);
		EnhancedInputComponent->BindAction(UIExitAction, ETriggerEvent::Triggered, this, &AMPlayerController::OnExitInput_UI);
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

void AMPlayerController::OnAdvanceInput_UI(const FInputActionValue& Value)
{
	if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
	{
		UI->Advance();
	}
}
void AMPlayerController::OnExitInput_UI(const FInputActionValue& Value)
{
	if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
	{
		UI->Exit();
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


void AMPlayerController::UIInputRequested(bool InputNeeded)
{
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputContextMap.Contains(EInputContext::Dialogue) && InputContextMap[EInputContext::Dialogue])
		{
 		   if(InputNeeded)
			{
				SubSystem->AddMappingContext(InputContextMap[EInputContext::Dialogue].Get(), 1);
 		   	    SubSystem->RemoveMappingContext(InputContextMap[EInputContext::Default]);

				bShowMouseCursor = true;
				bEnableClickEvents = true;
				bEnableMouseOverEvents = true;
			}
			else
			{
				SubSystem->AddMappingContext(InputContextMap[EInputContext::Default], 0);
				SubSystem->RemoveMappingContext(InputContextMap[EInputContext::Dialogue].Get()); 
				bShowMouseCursor =  bEnableClickEvents =  bEnableMouseOverEvents = false;
			}
		}
	}
}