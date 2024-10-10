

#include "Container.h"
#include "Inventory.h"
#include "InventoryInterface.h"



// Sets default values
AContainer::AContainer(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Create the Inventory component
    InventoryComponent = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
    Inventory =  InventoryComponent;

    InventoryComponent->OnInventoryInitialised.AddDynamic(this, &AContainer::AddInventoryItems);
}

// Called when the game starts or when spawned
void AContainer::BeginPlay()
{
	Super::BeginPlay();
	
}


