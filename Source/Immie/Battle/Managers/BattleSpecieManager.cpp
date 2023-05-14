// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleSpecieManager.h"
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Game/Global/Managers/SpecieDataManager.h>
#include <Immie/Immies/SpecieDataObject.h>

UBattleSpecieManager::UBattleSpecieManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UBattleSpecieManager::Initialize(ABattleInstance* _BattleInstance, bool UseOverride, FString OverrideFolder)
{
	BattleInstance = _BattleInstance;
	if (UseOverride) {
		LoadOverrideSpecieData(OverrideFolder);
	}
}

void UBattleSpecieManager::LoadOverrideSpecieData(const FString& FolderName)
{
	bUseOverride = true;

	ULogger::Log("Loading override specie battle data for battle at folder " + FolderName, LogVerbosity_Warning);
	GetSpecieDataManager()->RegisterSpeciesFromDisk(this, FolderName, &SpeciesOverride, false);
}

void UBattleSpecieManager::SyncToClients()
{
	TArray<FSerializedSpecieData> SerializedSpecieDataObjects;
	TArray<FName> Keys;
	SpeciesOverride.GetKeys(Keys);
	SerializedSpecieDataObjects.Reserve(Keys.Num());
	for (auto Key : Keys) {
		USpecieDataObject* DataObject = GetSpecieDataObject(Key);
		FSerializedSpecieData SpecieData;
		SpecieData.SpecieName = DataObject->GetSpecieName();

		FJsonObjectBP JsonData = DataObject->SpecieDataToJson();
		FString JsonString = JsonData.ToString();
		SpecieData.JsonString = JsonString;
		SerializedSpecieDataObjects.Add(SpecieData);
	}

	RecieveSerializedSpecieData(BattleInstance, SerializedSpecieDataObjects);
}

void UBattleSpecieManager::RecieveSerializedSpecieData_Implementation(ABattleInstance* _BattleInstance, const TArray<FSerializedSpecieData>& SpecieData)
{
	BattleInstance = _BattleInstance;
	if (BattleInstance->HasAuthority()) {
		return;
	}

	ULogger::Log("Recieved serialized specie data from server");
	bUseOverride = true;
	GetSpecieDataManager()->RegisterSpeciesFromSerialized(BattleInstance, SpecieData, &SpeciesOverride, false);
}

USpecieDataObject* UBattleSpecieManager::GetSpecieDataObject(FName SpecieName)
{
	if (!bUseOverride) {
		return GetSpecieDataManager()->GetSpecieDataObject(SpecieName);
	}

	return USpecieDataManager::GetSpecieDataObjectFromMap(SpeciesOverride, SpecieName);
}

int UBattleSpecieManager::GetSpecieTypeBitmask(FName SpecieName)
{
	return GetSpecieDataObject(SpecieName)->GetTypeBitmask();
}

FBaseStats UBattleSpecieManager::GetSpecieBaseStats(FName SpecieName)
{
	return GetSpecieDataObject(SpecieName)->GetBaseStats();
}

