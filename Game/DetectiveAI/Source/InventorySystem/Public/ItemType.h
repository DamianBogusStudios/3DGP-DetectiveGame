#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ItemType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Currency,
	Consumable,
	Tool,
	Crafting
};
