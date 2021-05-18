// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PerforamanceProfiler.h"
#include "PerforamanceProfilerStyle.h"
#include "PerforamanceProfilerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName PerforamanceProfilerTabName("PerforamanceProfiler");

#define LOCTEXT_NAMESPACE "FPerforamanceProfilerModule"

void FPerforamanceProfilerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FPerforamanceProfilerStyle::Initialize();
	FPerforamanceProfilerStyle::ReloadTextures();

	FPerforamanceProfilerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPerforamanceProfilerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPerforamanceProfilerModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FPerforamanceProfilerModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FPerforamanceProfilerModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PerforamanceProfilerTabName, FOnSpawnTab::CreateRaw(this, &FPerforamanceProfilerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FPerforamanceProfilerTabTitle", "PerforamanceProfiler"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FPerforamanceProfilerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPerforamanceProfilerStyle::Shutdown();

	FPerforamanceProfilerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PerforamanceProfilerTabName);
}

TSharedRef<SDockTab> FPerforamanceProfilerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FPerforamanceProfilerModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("PerforamanceProfiler.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FPerforamanceProfilerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(PerforamanceProfilerTabName);
}

void FPerforamanceProfilerModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FPerforamanceProfilerCommands::Get().OpenPluginWindow);
}

void FPerforamanceProfilerModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FPerforamanceProfilerCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPerforamanceProfilerModule, PerforamanceProfiler)