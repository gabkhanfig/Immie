// Fill out your copyright notice in the Description page of Project Settings.


#include "WildSpawnTable.h"
#include "../../Immies/ImmieSpawnData.h"
#include "../../Game/Global/Managers/SpecieDataManager.h"
#include "../../Immies/SpecieDataObject.h"

#define ERROR_PREFIX FString("[UWildSpawnTable Json]: ") 

UWildSpawnTable::UWildSpawnTable()
{
	MaxWeight = 0;
}

void UWildSpawnTable::LoadJsonData(const FJsonObjectBP& Json)
{
	MaxWeight = 0;

	TArray<FJsonObjectBP> TableElements = FJsonObjectBP::ArrayFromObjects(Json.GetArrayField("SpawnTable").GetObjectArray());
	for (int i = 0; i < TableElements.Num(); i++) {
		if (!TableElements[i].HasField("ImmieName")) {
			iLog("Immie spawn data json requires the field \"ImmieName\"", LogVerbosity_Error);
			continue;
		}
		FName ImmieName = FName(TableElements[i].GetStringField("ImmieName"));
		if (!GetSpecieDataManager()->IsValidSpecie(ImmieName)) {
			iLog(ERROR_PREFIX + "ImmieName of " + ImmieName.ToString() + " is not a valid specie");
		}
		UClass* SpawnDataClass = GetSpecieDataManager()->GetSpecieDataObject(ImmieName)->GetSpawnDataClass();
		UImmieSpawnData* SpawnDataObject = NewObject<UImmieSpawnData>(this, SpawnDataClass);
		SpawnDataObject->LoadJsonData(TableElements[i]);

		const int Weight = SpawnDataObject->GetWeight();
		MaxWeight += Weight;

		FWildImmieSpawnTableElement TableElement;
		TableElement.ImmieSpawnData = SpawnDataObject;
		TableElement.WeightIncrement = MaxWeight;
		Spawns.Add(TableElement);
	}
}

UImmieSpawnData* UWildSpawnTable::GetWeightedRandomSpawnData()
{
	checkf(Spawns.Num() > 0, TEXT("Attempted to get a weighted random wild immie spawn data from an empty spawn table"));

	int Rand = FMath::RandRange(0, MaxWeight);
	for (int i = 0; i < Spawns.Num(); i++) {
		const FWildImmieSpawnTableElement TableElement = Spawns[i];
		if (Rand <= TableElement.WeightIncrement) {
			return TableElement.ImmieSpawnData;
		}
	}

	checkf(false, TEXT("Unreachable code block in UWildSpawnTable"));
	return nullptr;
}
