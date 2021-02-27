// Copyright © 2020 Isaac Montagne All Rights Reserved.

using UnrealBuildTool;

public class SafeDelete : ModuleRules
{
	public SafeDelete(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
                "Slate"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new[]
			{
                "Core",
				"CoreUObject",
				"Engine",
				"Projects",
				"Slate",
				"SlateCore",
                "ContentBrowser",
                "UnrealEd"
			}
		);
	}
}
