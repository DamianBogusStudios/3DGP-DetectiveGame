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
            "UMG",
            "DeveloperSettings",
            "Json",
            "JsonUtilities",
            "HttpGPTChatModule",
            "HttpGPTCommonModule"
        });
    }
}
