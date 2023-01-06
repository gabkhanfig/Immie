// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfigDataManager.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include <Immie/Core/Formula.h>

void UConfigDataManager::LoadDefaultGameData()
{
	static const FString ConfigJsonPath = FPaths::Combine(FPaths::ProjectContentDir(), "Data/config.json");

	FJsonObjectBP ConfigJson;
	if (!FJsonObjectBP::LoadJsonFile(ConfigJsonPath, ConfigJson)) {
		return;
	}

	MaxLevel = ConfigJson.GetIntegerField("MaxLevel");
	MaxStatLevel = ConfigJson.GetIntegerField("MaxStatLevel");
	MaxBaseStat = ConfigJson.GetIntegerField("MaxBaseStat");
	MaxPlayerTeamSize = ConfigJson.GetIntegerField("MaxPlayerTeamSize");

	MaxXp = UFormula::MinimumXpForLevel(MaxLevel);
}
