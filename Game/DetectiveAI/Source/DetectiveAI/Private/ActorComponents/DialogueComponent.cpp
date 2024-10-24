// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DialogueComponent.h"
#include "Subsystems/InteractionSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "JsonObjectConverter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UI/DialogueWidget.h"
#include "Interfaces/DialogueProvider.h"
#include "Types/CommonCaseTypes.h"


// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UDialogueComponent::BeginPlay()
{
    Super::BeginPlay();
    
    BindDialogueDelegates();
}

void UDialogueComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
 //   UnBindDialogueDelegates();
}


void UDialogueComponent::BindDialogueDelegates()
{
    if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
    {
        InteractionSubsystem->OnStartDialogueAction.AddDynamic(this, &UDialogueComponent::OnDialogueStarted);
        InteractionSubsystem->OnAdvanceDialogueAction.AddDynamic(this, &UDialogueComponent::OnAdvanceDialogue);
        InteractionSubsystem->OnFinishDialogueAction.AddDynamic(this, &UDialogueComponent::OnFinishDialogue);
    }

    for(auto Subsystem : GetWorld()->GetSubsystemArray<UWorldSubsystem>())
    {
        if(Subsystem->Implements<UDialogueProvider>())
        {
            DialogueProvider = TScriptInterface<IDialogueProvider>(Subsystem);
            DialogueProvider->GetDialogueOptionsDelegate().AddDynamic(this, &UDialogueComponent::OnDialogueOptionsReceived);
            break;
        }
    }

}

void UDialogueComponent::UnBindDialogueDelegates()
{
    if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
    {
        InteractionSubsystem->OnStartDialogueAction.RemoveDynamic(this, &UDialogueComponent::OnDialogueStarted);
        InteractionSubsystem->OnAdvanceDialogueAction.RemoveDynamic(this, &UDialogueComponent::OnAdvanceDialogue);
        InteractionSubsystem->OnFinishDialogueAction.RemoveDynamic(this, &UDialogueComponent::OnFinishDialogue);
    }
}


void UDialogueComponent::StartDialogue() const
{
    if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
    {
        InteractionSubsystem->RequestDialogueAction(GetOwner());
    }
	else
    {
#if WITH_EDITOR
        FString Message = FString::Printf(TEXT("InteractionSystem not initialised"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
#endif

    }
}

void UDialogueComponent::FinishDialogue() const
{
    if(bInDialogue)
    {
        if (auto InteractionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInteractionSystem>())
        {
            InteractionSubsystem->FinishDialogueAction(GetOwner());
        }
    }
}

#pragma region Delegate Callbacks

void UDialogueComponent::OnDialogueStarted(AActor* Caller, UDialogueWidget* Widget)
{
    if(Caller == GetOwner())
    {
        bInDialogue = true;
        
        auto Pawn = Cast<APawn>(Caller);
        auto Controller = Pawn->GetController<AAIController>();
        UBlackboardComponent* BlackboardComponent;
      
        if(Controller && Controller->UseBlackboard(DialogueTree->GetBlackboardAsset(), BlackboardComponent))
        {                
            BlackboardComponent->SetValueAsObject("DialogueWidget", Widget);

            
            FString BoolValue = BlackboardComponent->GetValueAsBool("HasMetPlayer") ? "true" : "false";  
            
            if(auto BrainComponent =Controller->GetBrainComponent())
            {
                if(auto BTComp =  Cast<UBehaviorTreeComponent>(BrainComponent))
                {
                    if(BTComp->GetCurrentTree() == DialogueTree)
                    {
                        Controller->BrainComponent->RestartLogic();
                    }
                }
            }
            Controller->RunBehaviorTree(DialogueTree);

            if(DialogueProvider)
            {
                DialogueProvider->RequestDialogueOptions(this, ActorDescription);
            }
        }
    }
}

void UDialogueComponent::OnAdvanceDialogue(AActor* Caller, UDialogueWidget* Widget)
{
    if(Caller == GetOwner())
    {
       
    }
}


void UDialogueComponent::OnFinishDialogue(AActor* Caller, UDialogueWidget* Widget)
{
    if(Caller == GetOwner())
    {
        bInDialogue = false;
        
        auto Pawn = Cast<APawn>(Caller);
        auto Controller = Pawn->GetController<AAIController>();

        if(auto BlackboardComponent = Controller->GetBlackboardComponent())
        {
            BlackboardComponent->SetValueAsBool("HasMetPlayer", true);

            FString BoolValue = BlackboardComponent->GetValueAsBool("HasMetPlayer") ? "true" : "false";   
            FString Message = FString::Printf(TEXT("HasMetPlayer %s"), *BoolValue);
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
        }
    }
}

FString UDialogueComponent::GetGreeting()
{
    auto Pawn = Cast<APawn>(GetOwner());
    auto Controller = Pawn->GetController<AAIController>();

    if(auto BlackboardComponent = Controller->GetBlackboardComponent())
    {
        auto bHasMet = BlackboardComponent->GetValueAsBool("HasMetPlayer");

        if(bHasMet)
            return FString("Hi Again");
    }


    return FString("Hi Detective");
}

void UDialogueComponent::OnDialogueOptionsReceived(UObject* Caller, FDialogueOptions& DialogueOptions)
{
    if(Caller == this)
    {

        
    }
}

#pragma endregion
