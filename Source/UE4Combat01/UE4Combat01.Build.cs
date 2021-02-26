// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE4Combat01 : ModuleRules
{
	public UE4Combat01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
