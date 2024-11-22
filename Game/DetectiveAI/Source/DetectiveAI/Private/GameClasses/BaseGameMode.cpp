


#include "GameClasses/BaseGameMode.h"

#include "GameClasses/AGPGameInstance.h"


void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(auto GI = Cast<UAGPGameInstance>(GetGameInstance()))
	{
		GI->BeginInit();	
	}
}
