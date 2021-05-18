// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PerforamanceProfilerCommands.h"

#define LOCTEXT_NAMESPACE "FPerforamanceProfilerModule"

void FPerforamanceProfilerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "PerforamanceProfiler", "Bring up PerforamanceProfiler window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
