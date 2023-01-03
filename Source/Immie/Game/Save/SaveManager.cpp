// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include <Immie/ImmieCore.h>
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include <Immie/Game/Player/PlayerImmies.h>
#include <Immie/Game/Global/Managers/SpecieDataManager.h>
#include <Immie/Immies/ImmieObject.h>

const FString& USaveManager::GetSaveDataDir()
{
	static const FString SaveDataDir = FPaths::Combine(FPaths::ProjectContentDir(), "Data/Saves/");
	return SaveDataDir;
}

void USaveManager::LoadSave(FName SaveName, bool LoadPlayerImmies)
{
	FString FolderPath = GetSaveDataDir() + SaveName.ToString();

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*FolderPath)) {
		iLog("Unable to load save " + SaveName.ToString(), LogVerbosity_Error);
		return;
	}

	ULogger::Log("Loading save " + SaveName.ToString());
	ActiveSave = SaveName;

	if (LoadPlayerImmies) {
		LoadSavePlayerImmies(FolderPath);
	}
}

void USaveManager::LoadSavePlayerImmies(const FString& SavePath)
{
	FString FolderPath = SavePath + "/PlayerImmies.json";

	FJsonObjectBP PlayerImmiesJson;
	if (!FJsonObjectBP::LoadJsonFile(FolderPath, PlayerImmiesJson)) {
		iLog("Player immies json file at " + FolderPath + " does not exist", LogVerbosity_Error);
		return;
	}

	TArray<UImmie*> PlayerTeam = UImmie::JsonToTeam(PlayerImmiesJson, "PlayerTeam", GetPlayerImmies());
	for (const auto& Immie : PlayerTeam) {
		if (!GetPlayerImmies()->AddToTeam(Immie, false)) {
			iLog("Unable to add loaded save team Immie " + Immie->GetDisplayName() + " to player team");
		}
	}
}

void USaveManager::Save()
{
	FString FolderPath = GetSaveDataDir() + GetActiveSave().ToString();

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*FolderPath)) {
		PlatformFile.CreateDirectory(*FolderPath);
	}

	SavePlayerImmies(FolderPath);
}

void USaveManager::SavePlayerImmies(const FString& SavePath)
{
	FString FilePath = SavePath + "/PlayerImmies.json";

	FJsonObjectBP Root = UImmie::TeamToJson(GetPlayerImmies()->GetTeam(), "PlayerTeam");

	Root.SaveToFile(FilePath);
}

void USaveManager::InitializeSave(FName SaveName)
{
	FString FolderPath = GetSaveDataDir() + SaveName.ToString();

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*FolderPath)) {
		PlatformFile.CreateDirectory(*FolderPath);
	}
	else {
		ULogger::Log("While intiailizing a save, the directory [" + SaveName.ToString() + "] already exists in save data directory", LogVerbosity_Warning);
	}


}
