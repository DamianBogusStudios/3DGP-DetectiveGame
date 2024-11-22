// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DetectiveAIEditorTarget : TargetRules
{
	public DetectiveAIEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("DetectiveAI");
		ExtraModuleNames.Add("CaseGenerator");
		ExtraModuleNames.Add("CaseGeneratorEditor");
		ExtraModuleNames.Add("InventorySystem");
	}
}
