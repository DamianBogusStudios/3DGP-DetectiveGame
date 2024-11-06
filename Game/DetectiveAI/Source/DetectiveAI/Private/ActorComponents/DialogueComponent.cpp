// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DialogueComponent.h"
#include "Subsystems/UISystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UI/DialogueWidget.h"
#include "Interfaces/DialogueProvider.h"
#include "Settings/InteractionSettings.h"
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
    if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
    {
        UI->OnStartUIAction.AddDynamic(this, &UDialogueComponent::OnDialogueStarted);
        UI->OnAdvanceUIAction.AddDynamic(this, &UDialogueComponent::OnAdvanceDialogue);
        UI->OnFinishUIAction.AddDynamic(this, &UDialogueComponent::OnFinishDialogue);
    }
    
    for(auto Subsystem : GetWorld()->GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>())
    {
        if(Subsystem->Implements<UDialogueProvider>())
        {
            DialogueProvider = TScriptInterface<IDialogueProvider>(Subsystem);
            DialogueOptionsDelegate.BindDynamic(this, &UDialogueComponent::OnDialogueOptionsReceived);
            //DialogueProvider->GetDialogueOptionsDelegate().AddDynamic(this, &UDialogueComponent::OnDialogueOptionsReceived);
            break;
        }
    }


}

void UDialogueComponent::UnBindDialogueDelegates()
{
    if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
    {
        UI->OnStartUIAction.RemoveDynamic(this, &UDialogueComponent::OnDialogueStarted);
        UI->OnAdvanceUIAction.RemoveDynamic(this, &UDialogueComponent::OnAdvanceDialogue);
        UI->OnFinishUIAction.RemoveDynamic(this, &UDialogueComponent::OnFinishDialogue);
    }
}


void UDialogueComponent::StartDialogue() const
{
    if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
    {
        UI->RequestStartWidget(GetOwner(), EWidgetType::NPCDialogue);
        // InteractionSubsystem->RequestDialogueAction(GetOwner());
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
        if (auto UI = GetWorld()->GetGameInstance()->GetSubsystem<UUISystem>())
        {
            UI->RequestFinishWidget(GetOwner(), EWidgetType::NPCDialogue);
        }
    }
}

#pragma region Delegate Callbacks

void UDialogueComponent::OnDialogueStarted(AActor* Caller, UUserWidget* Widget)
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

            if(DialogueOptions.OptionOne.IsEmpty() && DialogueProvider)
            {
                DialogueProvider->RequestDialogueOptions(this, ActorDescription, DialogueOptionsDelegate);
            }
        }
    }
}

void UDialogueComponent::OnAdvanceDialogue(AActor* Caller, UUserWidget* Widget)
{
    if(Caller == GetOwner())
    {
       
    }
}


void UDialogueComponent::OnFinishDialogue(AActor* Caller, UUserWidget* Widget)
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
FDialogueOptions UDialogueComponent::GetDialogueOptions()
{
    return DialogueOptions;
}

void UDialogueComponent::OnDialogueOptionsReceived(FDialogueOptions& InDialogueOptions)
{
    UE_LOG(LogTemp, Log, TEXT("DialogueComponent: DialogueOptions Received"));
     
    DialogueOptions = InDialogueOptions;
}



#pragma endregion
