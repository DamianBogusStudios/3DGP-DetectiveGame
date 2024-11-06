using UnrealBuildTool;

public class CaseGeneratorEditor : ModuleRules
{
    public CaseGeneratorEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CaseGenerator"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "CaseGenerator",
                "Blutility",
                "UMG",
                "UMGEditor"
            }
        );
    }
}