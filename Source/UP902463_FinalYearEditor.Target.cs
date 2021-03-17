// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UP902463_FinalYearEditorTarget : TargetRules
{
	public UP902463_FinalYearEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("UP902463_FinalYear");
	}
}
