// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTypeManager.h"
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Game/Global/Managers/TypeDataManager.h>
#include <Immie/Type/ImmieType.h>

UBattleTypeManager::UBattleTypeManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UBattleTypeManager::Initialize(ABattleInstance* _BattleInstance, bool UseOverride, FString OverrideFolder)
{
	BattleInstance = _BattleInstance;
	if (UseOverride) {
		LoadOverrideTypeData(OverrideFolder);
	}
}

void UBattleTypeManager::LoadOverrideTypeData(const FString& FolderName)
{
	bUseOverride = true;

	ULogger::Log("Loading override type constants for battle at folder " + FolderName, LogVerbosity_Warning);
	GetTypeDataManager()->RegisterTypeConstants(FolderName, TypeConstantsOverride);

	ULogger::Log("Loading override types for battle at folder " + FolderName, LogVerbosity_Warning);
	GetTypeDataManager()->RegisterTypes(BattleInstance, FolderName, &TypesOverride, TypeConstantsOverride);
}

void UBattleTypeManager::SyncToClients()
{
	iLog("Performing type data syncing");
	TArray<int> Keys;
	TypesOverride.GetKeys(Keys);
	for (int Key : Keys) {
		UImmieType* Type = *TypesOverride.Find(Key);
		Type->SyncToClients();
	}
}

UImmieType* UBattleTypeManager::GetType(int TypeBitmask)
{
	if (!bUseOverride) {
		return GetTypeDataManager()->GetType(TypeBitmask);
	}

	UImmieType** Found = TypesOverride.Find(TypeBitmask);
	if (Found) {
		return *Found;
	}
	else {
		iLog("Unable to find overridden type with bitmask " + FString::FromInt(TypeBitmask), LogVerbosity_Error);
		return nullptr;
	}
}

FTypeConstants UBattleTypeManager::GetTypeConstants()
{
	if (!bUseOverride) {
		return GetTypeDataManager()->GetTypeConstants();
	}
	else {
		return TypeConstantsOverride;
	}
}

TArray<UImmieType*> UBattleTypeManager::GetTypes(int TypeBitmask)
{
	if (!bUseOverride) {
		return GetTypeDataManager()->GetTypes(TypeBitmask);
	}

	return UTypeDataManager::GetMultipleTypesFromMap(TypeBitmask, TypesOverride);
}

