// Fill out your copyright notice in the Description page of Project Settings.


#include "WildSpawnTable.h"
#include "ImmieSpawnData.h"
#include "../../Game/Global/Managers/SpecieDataManager.h"

#define ERROR_PREFIX FString("[UWildSpawnTable Json]: ") 

UWildSpawnTable::UWildSpawnTable()
{
}

void UWildSpawnTable::LoadJsonData(const FJsonObjectBP& Json)
{
	TArray<FJsonObjectBP> TableElements = FJsonObjectBP::ArrayFromObjects(Json.GetArrayField("SpawnTable").GetObjectArray());
	for (int i = 0; i < TableElements.Num(); i++) {
		if (!TableElements[i].HasField("ImmieName")) {
			iLog("Immie spawn data json requires the field \"ImmieName\"", LogVerbosity_Error);
			continue;
		}
		FName ImmieName = FName(TableElements[i].GetStringField("ImmieName"));
		if (!GetSpecieDataManager()->IsValidSpecie(GetSpecieDataManager()->GetSpecieId(ImmieName))) {
			iLog(ERROR_PREFIX + "ImmieName of " + ImmieName.ToString() + " is not a valid specie");
		}
	}
}
