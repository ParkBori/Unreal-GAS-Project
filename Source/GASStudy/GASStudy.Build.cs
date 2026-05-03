// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASStudy : ModuleRules
{
	public GASStudy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

			
		PublicDependencyModuleNames.AddRange(new string[] 
        { 
            /////////////////////////////////
			// [Core Modules]
			/////////////////////////////////
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",

            /////////////////////////////////
			// [UI Modules]
			/////////////////////////////////
            "UMG",

            /////////////////////////////////
			// [CommonUI Modules]
			/////////////////////////////////
            "CommonUI",
            "CommonInput",

            /////////////////////////////////
			// [AI Modules]
			/////////////////////////////////
            "AIModule",

            /////////////////////////////////
			// [Navigation Modules]
			/////////////////////////////////
            "NavigationSystem", 

            /////////////////////////////////
			// [MotionWarping Modules]
			/////////////////////////////////
            "MotionWarping", 

            /////////////////////////////////
			// [Niagara Modules]
			/////////////////////////////////
            "Niagara",

            /////////////////////////////////
			// [Media Modules]
            // 동영상 재생관련
			/////////////////////////////////
            "MediaAssets",

            /////////////////////////////////
			// [애니메이션 그래프]
            // 동영상 재생관련
			/////////////////////////////////
            "AnimGraphRuntime"

        });

        PublicIncludePaths.Add("GASStudy");

        PrivateDependencyModuleNames.AddRange(new string[] {
            
            /////////////////////////////////
			// [UI Modules]
			/////////////////////////////////
            "Slate",
            "SlateCore",
            
            /////////////////////////////////
			// [GAS Modules]
			/////////////////////////////////
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags",

            /////////////////////////////////
			// [GameplayCamera Modules]
			/////////////////////////////////
            "GameplayCameras",

            "DeveloperSettings",

            /////////////////////////////////
			// [Json]
			/////////////////////////////////
            "Json",
            "JsonUtilities"
        });

        // 에디터 환경에서 종료시 저장로직 테스틀 위해 추가
        //if (Target.bBuildEditor)
        //{
        //    PrivateDependencyModuleNames.Add("UnrealEd");
        //}
    }
}
