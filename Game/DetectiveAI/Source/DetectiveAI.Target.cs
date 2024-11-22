// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DetectiveAITarget : TargetRules
{
	public DetectiveAITarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("DetectiveAI");
		ExtraModuleNames.Add("CaseGenerator");
		ExtraModuleNames.Add("InventorySystem");
	}
}
