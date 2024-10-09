// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GPE340_Shooter_Nick : ModuleRules
{
	public GPE340_Shooter_Nick(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara", "AIModule", "NavigationSystem"  });
	}
}
