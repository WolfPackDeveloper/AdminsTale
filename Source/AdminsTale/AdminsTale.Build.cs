// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdminsTale : ModuleRules
{
	public AdminsTale(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks", "Slate", "SlateCore" });

		PublicIncludePaths.AddRange(new string[]
		{
			"AdminsTale/Public/GameModes",
			"AdminsTale/Public/Characters",
			"AdminsTale/Public/Controllers",
			"AdminsTale/Public/Components",
			"AdminsTale/Public/Actors",
			"AdminsTale/Public/Objects"
		});

		// Пути для поиска файлов проекта. Чтобы не писать полные пути в include.
		//PublicIncludePaths.AddRange(new string[] { "" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
