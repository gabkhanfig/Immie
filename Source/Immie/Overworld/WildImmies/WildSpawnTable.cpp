// Fill out your copyright notice in the Description page of Project Settings.


#include "WildSpawnTable.h"
#include "../../Immies/ImmieSpawnData.h"
#include "../../Game/Global/Managers/SpecieDataManager.h"
#include "../../Immies/SpecieDataObject.h"

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
			iLog("[UWildSpawnTable Json]: Immie spawn data json requires the field \"ImmieName\"", LogVerbosity_Error);
			continue;
		}

		FString ImmieName = TableElements[i].GetStringField("ImmieName");
		if (!USpecieDataManager::IsValidSpecieNameString(ImmieName)) {
			iLog("[UWildSpawnTable Json]: ImmieName of " + ImmieName + " is not a valid specie", LogVerbosity_Error);
		}
		UClass* SpawnDataClass = GetSpecieDataManager()->GetSpecieDataObject(FName(ImmieName))->GetSpawnDataClass();
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
