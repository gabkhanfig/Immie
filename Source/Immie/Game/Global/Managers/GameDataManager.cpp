// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataManager.h"

const FString& UGameDataManager::GetGameDataDir()
{
	static const FString GameDataDir = FPaths::Combine(FPaths::ProjectContentDir(), "Data/GameData/");
	return GameDataDir;
}

void UGameDataManager::Load()
{
	if (!bLoadedData)
		LoadDefaultGameData();

	bLoadedData = true;
}

void UGameDataManager::LoadDefaultGameData()
{
}

void UGameDataManager::Reload()
{
	bLoadedData = false;
	Load();
}
