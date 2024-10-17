using UnrealBuildTool;

public class CaseGenerator: ModuleRules
{
    public CaseGenerator(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "OpenAI",
            "UMG",
            "Json",
            "JsonUtilities"
        });
    }
}
