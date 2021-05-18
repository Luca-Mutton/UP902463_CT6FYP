// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "PerforamanceProfilerStyle.h"

class FPerforamanceProfilerCommands : public TCommands<FPerforamanceProfilerCommands>
{
public:

	FPerforamanceProfilerCommands()
		: TCommands<FPerforamanceProfilerCommands>(TEXT("PerforamanceProfiler"), NSLOCTEXT("Contexts", "PerforamanceProfiler", "PerforamanceProfiler Plugin"), NAME_None, FPerforamanceProfilerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};