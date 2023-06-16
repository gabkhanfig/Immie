// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieSpawnData.h"
#include "../Game/Global/Managers/ConfigDataManager.h"
#include "../Game/Global/Managers/SpecieDataManager.h"
#include "../Overworld/WildImmies/WildImmieSpawner.h"
#include "ImmieCharacter.h"
#include "ImmieObject.h"

UImmieSpawnData::UImmieSpawnData()
	: MinLevel(1), MaxLevel(1), Weight(1)
{
}

void UImmieSpawnData::PostLoad()
{
	const FName ClassName = GetClass()->GetFName();
	if (ClassName == "ImmieSpawnData") {
		Super::PostLoad();
		return;
	}

	SpecieName = USpecieDataManager::SpecieNameFromBlueprintClassName(ClassName.ToString(), "SpawnData_C");

	Super::PostLoad();
}

#define ERROR_PREFIX FString("[UImmieSpawnData Json]: ") 

void UImmieSpawnData::LoadJsonData(const FJsonObjectBP& Json)
{
	int _MinLevel;
	int _MaxLevel;
	Json.TryGetIntegerField("MinLevel", _MinLevel, true, 1);
	Json.TryGetIntegerField("MaxLevel", _MaxLevel, true, 1);
	if (_MinLevel > _MaxLevel) {
		iLog(ERROR_PREFIX + "MinLevel of " + FString::FromInt(_MinLevel) + " cannot be greater than the MaxLevel of " + FString::FromInt(_MaxLevel), LogVerbosity_Error);
		MinLevel = 1;
		MaxLevel = 1;
	}
	else {
		if (_MinLevel < 1 || _MinLevel > GetConfigDataManager()->GetMaxLevel()) {
			iLog(ERROR_PREFIX + "MinLevel of " + FString::FromInt(_MinLevel) + " is not within bounds. Must be between 1-" + FString::FromInt(GetConfigDataManager()->GetMaxLevel()), LogVerbosity_Error);
			_MinLevel = 1;
		}
		if (_MaxLevel < 1 || _MaxLevel > GetConfigDataManager()->GetMaxLevel()) {
			iLog(ERROR_PREFIX + "MaxLevel of " + FString::FromInt(_MinLevel) + " is not within bounds. Must be between 1-" + FString::FromInt(GetConfigDataManager()->GetMaxLevel()), LogVerbosity_Error);
			_MaxLevel = GetConfigDataManager()->GetMaxLevel();
		}
		MinLevel = _MinLevel;
		MaxLevel = _MaxLevel;
	}

	Json.TryGetIntegerField("Weight", Weight, true, 1); 
	if (Weight < 1) {
		iLog(ERROR_PREFIX + "Weight of " + FString::FromInt(Weight) + " cannot be less than 1", LogVerbosity_Error);
		Weight = 1;
	}

	BP_LoadJsonData(Json);
}
