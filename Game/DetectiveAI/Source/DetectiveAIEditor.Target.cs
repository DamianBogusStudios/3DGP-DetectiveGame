// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DetectiveAIEditorTarget : TargetRules
{
	public DetectiveAIEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("DetectiveAI");
		ExtraModuleNames.Add("CaseGenerator");
		ExtraModuleNames.Add("InventorySystem");
	}
}
