// Fill out your copyright notice in the Description page of Project Settings.

using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class UETCPFlatBuffer : ModuleRules
{
	public UETCPFlatBuffer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "Networking" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        string IncludePath = Path.Combine(ModuleDirectory, "..", "ThirdParty");

        //header .h -> 플랫폼 상관이 없음 source 니까.
        PublicIncludePaths.Add(IncludePath);

        //Library Window, .lib, Linux, Android, .so Ios. .a
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            //windows
            //         string LibraryPath = Path.Combine(ModuleDirectory, "..", "ThirdParty", "lib");

            ////library 파일 추가 
            //         PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "Win64", "MySQL.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
        }
    }
}
