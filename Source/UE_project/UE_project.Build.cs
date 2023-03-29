// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_project : ModuleRules
{
	public UE_project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
	}
}
