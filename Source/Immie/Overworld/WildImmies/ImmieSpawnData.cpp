// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieSpawnData.h"
#include "../../Game/Global/Managers/ConfigDataManager.h"
#include "../../Game/Global/Managers/SpecieDataManager.h"
#include "../../Immies/ImmieCharacter.h"
#include "WildImmieSpawner.h"
#include "../../Immies/ImmieObject.h"

UImmieSpawnData::UImmieSpawnData()
	: MinLevel(1), MaxLevel(1), Weight(1)
{
}

#define ERROR_PREFIX FString("[UImmieSpawnData Json]: ") 

void UImmieSpawnData::LoadJsonData(const FJsonObjectBP& Json)
{
	if (!Json.HasField("ImmieName")) {
		iLog("Immie spawn data json requires the field \"ImmieName\"", LogVerbosity_Error);
		return;
	}

	ImmieName = FName(Json.GetStringField("ImmieName"));
	if (!GetSpecieDataManager()->IsValidSpecie(ImmieName)) {
		iLog(ERROR_PREFIX + "ImmieName of " + ImmieName.ToString() + " is not a valid specie");
	}

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
