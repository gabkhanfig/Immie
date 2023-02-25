// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAbilityManager.h"
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Game/Global/Managers/AbilityDataManager.h>
#include <Immie/Ability/Ability.h>
#include <Immie/Ability/AbilityDataObject.h>

UBattleAbilityManager::UBattleAbilityManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UBattleAbilityManager::Initialize(ABattleInstance* _BattleInstance, bool UseOverride, FString OverrideFolder)
{
	BattleInstance = _BattleInstance;
	if (UseOverride) {
		LoadOverrideAbilityData(OverrideFolder);
	}
}

void UBattleAbilityManager::LoadOverrideAbilityData(const FString& FolderName)
{
	bUseOverride = true;

	ULogger::Log("Loading global ability data for battle at folder " + FolderName, LogVerbosity_Warning);
	GetAbilityDataManager()->RegisterAbilitiesFromDisk(BattleInstance, FolderName, &AbilitiesOverride);
}

void UBattleAbilityManager::SyncToClients()
{
	TArray<FSerializedAbilityData> SerializedAbilityDataObjects;
	TArray<int> Keys;
	AbilitiesOverride.GetKeys(Keys);
	SerializedAbilityDataObjects.Reserve(Keys.Num());
	for (auto Key : Keys) {
		UAbilityDataObject* DataObject = GetAbilityDataObject(Key);
		FSerializedAbilityData AbilityData;
		AbilityData.AbilityId = DataObject->GetAbilityId();
		AbilityData.AbilityName = DataObject->GetAbilityName();
		
		FJsonObjectBP JsonData = DataObject->AbilityDataToJson();
		FString JsonString = JsonData.ToString();
		AbilityData.JsonString = JsonString;
		SerializedAbilityDataObjects.Add(AbilityData);
	}

	RecieveSerializedAbilityData(BattleInstance, SerializedAbilityDataObjects);
}

void UBattleAbilityManager::RecieveSerializedAbilityData_Implementation(ABattleInstance* _BattleInstance, const TArray<FSerializedAbilityData>& AbilityData)
{
	BattleInstance = _BattleInstance;
	if (BattleInstance->HasAuthority()) {
		return;
	}

	ULogger::Log("Recieved serialized ability data from server");
	bUseOverride = true;
	GetAbilityDataManager()->RegisterAbilitiesFromSerialized(BattleInstance, AbilityData, &AbilitiesOverride);
}

UAbilityDataObject* UBattleAbilityManager::GetAbilityDataObject(int AbilityId)
{
	if (!bUseOverride) {
		return GetAbilityDataManager()->GetAbilityDataObject(AbilityId);
	}

	return UAbilityDataManager::GetAbilityDataObjectFromMap(AbilitiesOverride, AbilityId);	
}

