


#include "GameClasses/BaseGameMode.h"

#include "GameClasses/MGameInstance.h"


void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(auto GI = Cast<UMGameInstance>(GetGameInstance()))
	{
		GI->BeginInit();	
	}
}
