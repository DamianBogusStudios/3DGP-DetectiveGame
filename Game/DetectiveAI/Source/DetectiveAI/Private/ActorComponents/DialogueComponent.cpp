// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DialogueComponent.h"
#include "Handlers/ServiceLocator.h"
#include "Interfaces/LLMService.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWaveProcedural.h"
#include "Settings/LLMSettings.h"
#include "Subsystems/UISystem.h"

// Fill out your copyright notice in the Description page of Project Settings.

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#pragma region Initalisation
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	GetServices();
	BindCallbacks();
}

void UDialogueComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UServiceLocator::Cleanup();
}

void UDialogueComponent::GetServices()
{
	LLMService = UServiceLocator::GetService_LLM();
	TTSService = UServiceLocator::GetService_TTS();
}

void UDialogueComponent::BindCallbacks()
{
	MessageDelegate.BindUObject(this, &UDialogueComponent::OnMessageReceived);
	VoiceDelegate.BindUObject(this, &UDialogueComponent::OnVoiceReceived);
}


void UDialogueComponent::SetDescription(const FActorDescription& Description)
{
	ActorDescription = Description;
}

void UDialogueComponent::RegisterWitness()
{
	if(LLMService)
	{
		if(const ULLMSettings* Settings = GetDefault<ULLMSettings>())
		{
			auto PromptConfig = Settings->GetPromptConfigData();
	
			LLMService->SendCustomInstructions(this, PromptConfig->WitnessCustomInstructions);
			LLMService->SendCustomInstructions(this, ActorDescription.ToString());
			bActorRegistered = true;
		}
	}
}

#pragma endregion

#pragma region Requests

void UDialogueComponent::SendMessageToActor(const FString& Prompt)
{
	if(!bActorRegistered)
	{
		RegisterWitness();
	}
	
	if(LLMService && bActorRegistered)
	{
		LLMService->SendMessage(this, Prompt, MessageDelegate);
	}
}

#pragma endregion 

#pragma region Callbacks
void UDialogueComponent::OnMessageReceived(FString& Message)
{
	UE_LOG(LogTemp, Display, TEXT("Actor Spoke %s"), *Message);

	MessageRelayDelegate.ExecuteIfBound(Message);
	
	if(TTSService)
	{
		TTSService->SendTextToVoice(this, Message, VoiceDelegate);
	}
}

void UDialogueComponent::OnTranscriptReceived(const FString& Message)
{
//	TranscriptRelayDelegate.ExecuteIfBound(Message);
}

void UDialogueComponent::OnVoiceReceived(USoundWaveProcedural* SoundWave)
{
	ProcessedSoundWave = SoundWave;
	UGameplayStatics::PlaySound2D(this, ProcessedSoundWave);
}
#pragma endregion 


void UDialogueComponent::StartDialogue() 
{
    if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
    {
    	UI->RequestStartWidget(this, EUIElementType::NPCDialogue);
    	bInDialogue = true;
    }
	else
    {
#if WITH_EDITOR
        FString Message = FString::Printf(TEXT("InteractionSystem not initialised"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif

    }
}

void UDialogueComponent::FinishDialogue()
{
    if(bInDialogue)
    {
        if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
        {
            UI->RequestFinishWidget(GetOwner(), EUIElementType::NPCDialogue);
        }
    }
}

// void UDialogueComponent::BeginPlay()
// {
//     Super::BeginPlay();
//     
//     BindDialogueDelegates();
// }
//
// void UDialogueComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
// {
//  //   UnBindDialogueDelegates();
// }
//
//
// void UDialogueComponent::BindDialogueDelegates()
// {
//     if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
//     {
//         UI->OnStartUIAction.AddDynamic(this, &UDialogueComponent::OnDialogueStarted);
//         UI->OnAdvanceUIAction.AddDynamic(this, &UDialogueComponent::OnAdvanceDialogue);
//         UI->OnFinishUIAction.AddDynamic(this, &UDialogueComponent::OnFinishDialogue);
//     }
//     
//     for(auto Subsystem : GetWorld()->GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>())
//     {
//         if(Subsystem->Implements<UDialogueProvider>())
//         {
//             DialogueProvider = TScriptInterface<IDialogueProvider>(Subsystem);
//             DialogueOptionsDelegate.BindDynamic(this, &UDialogueComponent::OnDialogueOptionsReceived);
//             //DialogueProvider->GetDialogueOptionsDelegate().AddDynamic(this, &UDialogueComponent::OnDialogueOptionsReceived);
//             break;
//         }
//     }
//
//
// }
//
// void UDialogueComponent::UnBindDialogueDelegates()
// {
//     if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
//     {
//         UI->OnStartUIAction.RemoveDynamic(this, &UDialogueComponent::OnDialogueStarted);
//         UI->OnAdvanceUIAction.RemoveDynamic(this, &UDialogueComponent::OnAdvanceDialogue);
//         UI->OnFinishUIAction.RemoveDynamic(this, &UDialogueComponent::OnFinishDialogue);
//     }
// }
//
//
// 
//
// #pragma region Delegate Callbacks
//
// void UDialogueComponent::OnDialogueStarted(AActor* Caller, UUserWidget* Widget)
// {
//     if(Caller == GetOwner())
//     {
//         bInDialogue = true;
//         
//         auto Pawn = Cast<APawn>(Caller);
//         auto Controller = Pawn->GetController<AAIController>();
//         UBlackboardComponent* BlackboardComponent;
//       
//         if(Controller && Controller->UseBlackboard(DialogueTree->GetBlackboardAsset(), BlackboardComponent))
//         {                
//             BlackboardComponent->SetValueAsObject("DialogueWidget", Widget);
//
//             
//             FString BoolValue = BlackboardComponent->GetValueAsBool("HasMetPlayer") ? "true" : "false";  
//             
//             if(auto BrainComponent =Controller->GetBrainComponent())
//             {
//                 if(auto BTComp =  Cast<UBehaviorTreeComponent>(BrainComponent))
//                 {
//                     if(BTComp->GetCurrentTree() == DialogueTree)
//                     {
//                         Controller->BrainComponent->RestartLogic();
//                     }
//                 }
//             }
//             Controller->RunBehaviorTree(DialogueTree);
//
//             if(DialogueOptions.OptionOne.IsEmpty() && DialogueProvider)
//             {
//                 DialogueProvider->RequestDialogueOptions(this, ActorDescription, DialogueOptionsDelegate);
//             }
//         }
//     }
// }
//
// void UDialogueComponent::OnAdvanceDialogue(AActor* Caller, UUserWidget* Widget)
// {
//     if(Caller == GetOwner())
//     {
//        
//     }
// }
//
//
// void UDialogueComponent::OnFinishDialogue(AActor* Caller, UUserWidget* Widget)
// {
//     if(Caller == GetOwner())
//     {
//         bInDialogue = false;
//         
//         auto Pawn = Cast<APawn>(Caller);
//         auto Controller = Pawn->GetController<AAIController>();
//
//         if(auto BlackboardComponent = Controller->GetBlackboardComponent())
//         {
//             BlackboardComponent->SetValueAsBool("HasMetPlayer", true);
//
//             FString BoolValue = BlackboardComponent->GetValueAsBool("HasMetPlayer") ? "true" : "false";   
//             FString Message = FString::Printf(TEXT("HasMetPlayer %s"), *BoolValue);
//             GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
//         }
//     }
// }
//
// FString UDialogueComponent::GetGreeting()
// {
//     auto Pawn = Cast<APawn>(GetOwner());
//     auto Controller = Pawn->GetController<AAIController>();
//
//     if(auto BlackboardComponent = Controller->GetBlackboardComponent())
//     {
//         auto bHasMet = BlackboardComponent->GetValueAsBool("HasMetPlayer");
//
//         if(bHasMet)
//             return FString("Hi Again");
//     }
//
//
//     return FString("Hi Detective");
// }
// FDialogueOptions UDialogueComponent::GetDialogueOptions()
// {
//     return DialogueOptions;
// }
//
// void UDialogueComponent::OnDialogueOptionsReceived(FDialogueOptions& InDialogueOptions)
// {
//     UE_LOG(LogTemp, Log, TEXT("DialogueComponent: DialogueOptions Received"));
//      
//     DialogueOptions = InDialogueOptions;
// }
//
//
//
// #pragma endregion
