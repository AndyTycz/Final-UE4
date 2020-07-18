// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TP_UnrealEngineEditorTarget : TargetRules
{
	public TP_UnrealEngineEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "TP_UnrealEngine" } );
	}
}
