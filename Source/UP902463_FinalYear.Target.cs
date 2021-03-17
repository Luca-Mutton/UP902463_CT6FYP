// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UP902463_FinalYearTarget : TargetRules
{
	public UP902463_FinalYearTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("UP902463_FinalYear");
	}
}
