
#include "Controllers/MainPlayerController.h"
#include "Characters/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GameClasses/AGPGameInstance.h"
#include "PlayStation/DualSenseController.h"
#include "Interfaces/MiniGameInputInterface.h"
#include "Subsystems/UISystem.h"



// ReSharper disable once CppPossiblyUninitializedMember
AMainPlayerController::AMainPlayerController()
{
	for (int i = 0; i < static_cast<int>(EInputContext::MAX); ++i)
	{
		InputContextMap.Add(static_cast<EInputContext>(i), nullptr);
	}
}


void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	BindCallbacks();

	RequestInputContext(EInputContext::UI, true);
	DualSense = NewObject<UDualSenseController>(this, UDualSenseController::StaticClass());
	DualSense->SetLightBarColour(FColor::Yellow);	
}

void AMainPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	BindDefaultActions();
	BindUIActions();
	BindMiniGameActions();
}


void AMainPlayerController::OnGameLoaded()
{
	RequestInputContext(EInputContext::DefaultContext, true);

	if (auto UI = GetGameInstance()->GetSubsystem<UUISystem>())
	{
		//UI->RequestStartWidget(this, EUIElementType::LockpickMiniGame);
	}
}

void AMainPlayerController::BindCallbacks()
{
	if(auto GameInstance = GetWorld()->GetGameInstance())
	{
		if(auto CustomGameInstance = Cast<UAGPGameInstance>(GameInstance))
		{
			CustomGameInstance->OnGameFinishedLoading.AddDynamic(this, &AMainPlayerController::OnGameLoaded);
		}
	}
	
	if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
	{
		UI->RequestInput.AddDynamic(this, &AMainPlayerController::RequestUIInput);
	}
}

void AMainPlayerController::BindDefaultActions()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(DefaultInputActions.Move, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleMove);
		EnhancedInputComponent->BindAction(DefaultInputActions.Interact, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleInteract);
	}
}
void AMainPlayerController::BindUIActions()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(UIInputActions.Advance, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleAdvance_UI);
		EnhancedInputComponent->BindAction(UIInputActions.Exit, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleExit_UI);
	}
}

void AMainPlayerController::BindMiniGameActions()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MiniGameInputActions.ApplyTension, ETriggerEvent::Triggered,
			this, &AMainPlayerController::HandleApplyTension_MiniGame);
		EnhancedInputComponent->BindAction(MiniGameInputActions.ApplyTension, ETriggerEvent::Completed,
			this, &AMainPlayerController::HandleApplyTensionCompleted_MiniGame);
		EnhancedInputComponent->BindAction(MiniGameInputActions.RaisePin, ETriggerEvent::Triggered,
			this, &AMainPlayerController::HandleRaisePin_MiniGame);
		EnhancedInputComponent->BindAction(MiniGameInputActions.RaisePin, ETriggerEvent::Completed,
			this, &AMainPlayerController::HandleRaisePinCompleted_MiniGame);
		EnhancedInputComponent->BindAction(MiniGameInputActions.MovePin, ETriggerEvent::Triggered,
		this, &AMainPlayerController::HandleMovePick_MiniGame);
		EnhancedInputComponent->BindAction(MiniGameInputActions.SetPin, ETriggerEvent::Triggered,
			this, &AMainPlayerController::HandleSetPin_MiniGame);
	}
}


void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BaseCharacter = Cast<AMainCharacter>(InPawn);
	if(UCameraComponent* Camera = InPawn->GetComponentByClass<UCameraComponent>())
	{
		CachedCameraRotation = FRotator(0, Camera->GetComponentRotation().Yaw, 0);
	}
}

void AMainPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	BaseCharacter = nullptr;
}

void AMainPlayerController::RequestInputContext(EInputContext Context, bool bAdd)
{
	if (auto EnhancedInput = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputContextMap[Context])
		{
			if (bAdd && !EnhancedInput->HasMappingContext(InputContextMap[Context].Get()))
			{
				UE_LOG(LogTemp, Log, TEXT("Context Added: %s"), *UEnum::GetValueAsString(Context));
				EnhancedInput->AddMappingContext(InputContextMap[Context].Get(), static_cast<int>(Context));
			}
			else if (!bAdd && EnhancedInput->HasMappingContext(InputContextMap[Context].Get()))
			{
				UE_LOG(LogTemp, Log, TEXT("Context Removed: %s"), *UEnum::GetValueAsString(Context));
				EnhancedInput->RemoveMappingContext(InputContextMap[Context].Get());
			}
		}
	}
}

void AMainPlayerController::RequestUIInput(const TArray<EUIElementType>& UIElements)
{
	if (auto EnhancedInput = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		TSet<EInputContext> InputContextsNeeded;
		
		for (auto& UIElement : UIElements)
		{
			auto InputMaps = UIToInputContext(UIElement);
			InputContextsNeeded.Append(InputMaps);
		}

		for (EInputContext Context : TEnumRange<EInputContext>())
		{
			if (ShouldSkipInputCheck(Context))
				continue;
					
			RequestInputContext(Context, InputContextsNeeded.Contains(Context));
		}

		UpdateMouseSettings(InputContextsNeeded.Contains(EInputContext::UI));
	}
}

#pragma region Static Functions
TSet<EInputContext> AMainPlayerController::UIToInputContext(EUIElementType Context)
{
	switch (Context)
	{
		case EUIElementType::NPCDialogue:
			return {EInputContext::UI, EInputContext::Dialogue};
		case EUIElementType::LockpickMiniGame:
			return {EInputContext::UI, EInputContext::MiniGame};
		case EUIElementType::Loading:
		case EUIElementType::Pickup:
			return {EInputContext::UI};
		default:
			return {};
	}
}
#pragma endregion

#pragma region Input Callbacks

void AMainPlayerController::HandleMove(const FInputActionValue& Value)
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

void AMainPlayerController::HandleInteract(const FInputActionValue& Value)
{
	if (BaseCharacter)
	{
		BaseCharacter->Interact();
	}
}

void AMainPlayerController::HandleAdvance_UI(const FInputActionValue& Value)
{
	if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
	{
		UI->Advance();
	}
}
void AMainPlayerController::HandleExit_UI(const FInputActionValue& Value)
{
	if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
	{
		UI->Exit();
	}
}

void AMainPlayerController::HandleApplyTensionCompleted_MiniGame(const FInputActionValue& Value)
{
	if(MiniGameInputInterface)
	{
		MiniGameInputInterface->HandleApplyTensionCompleted_MiniGame();
	}
}
void AMainPlayerController::HandleRaisePinCompleted_MiniGame(const FInputActionValue& Value)
{
	if(MiniGameInputInterface)
	{
		MiniGameInputInterface->HandleRaisePinCompleted_MiniGame();
	}
}
void AMainPlayerController::HandleApplyTension_MiniGame(const FInputActionValue& Value)
{
	if(MiniGameInputInterface)
	{
		float Input = Value.Get<float>();
		MiniGameInputInterface->HandleApplyTension_MiniGame(Input);
	}
}
void AMainPlayerController::HandleRaisePin_MiniGame(const FInputActionValue& Value)
{
	if(MiniGameInputInterface)
	{
		float Input = Value.Get<float>();
		MiniGameInputInterface->HandleRaisePin_MiniGame(Input);
	}
}
void AMainPlayerController::HandleMovePick_MiniGame(const FInputActionValue& Value)
{
	if(MiniGameInputInterface)
	{
		float Input = Value.Get<float>();
		MiniGameInputInterface->HandleMovePick_MiniGame(Input == 1 ? true : false);
	}
}
void AMainPlayerController::HandleSetPin_MiniGame(const FInputActionValue& Value)
{
	if(MiniGameInputInterface)
	{
		MiniGameInputInterface->HandleSetPin_MiniGame();
	}
}


#pragma endregion 

#pragma region DualSense
void AMainPlayerController::SetAdaptiveTriggerEffect(FAdaptiveTriggerEffect TriggerEffect)
{
	uint8 StartPos = FMath::RoundToInt(TriggerEffect.StartPos * 255);
	uint8 EndPos = FMath::RoundToInt(TriggerEffect.EndPos * 255);
	uint8 Strength = FMath::RoundToInt(TriggerEffect.Strength * 255);
	int Trigger = static_cast<int>(TriggerEffect.Trigger);
	int Effect = static_cast<int>(TriggerEffect.Effect);
	
	DualSense->SetTriggerEffectProperty(StartPos, EndPos, Strength, Trigger, Effect);
}

void AMainPlayerController::ResetTriggerEffect(ETrigger Trigger)
{
	DualSense->ResetTrigger(static_cast<int>(Trigger));
}
#pragma endregion

