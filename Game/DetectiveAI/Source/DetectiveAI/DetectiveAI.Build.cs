// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DetectiveAI : ModuleRules
{
	public DetectiveAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"NavigationSystem",
			"AIModule",
			"Niagara",
			"EnhancedInput",
			"InventorySystem",
			"CaseGenerator",
			"JsonUtilities"
        });
    }
}
