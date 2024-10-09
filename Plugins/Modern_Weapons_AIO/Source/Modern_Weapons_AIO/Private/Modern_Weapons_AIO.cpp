// Copyright SOUNDFX STUDIO © 2023

#include "Modern_Weapons_AIO.h"

#define LOCTEXT_NAMESPACE "FModern_Weapons_AIOModule"

void FModern_Weapons_AIOModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModern_Weapons_AIOModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FModern_Weapons_AIOModule, Modern_Weapons_AIO)