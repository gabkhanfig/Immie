// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTypeManager.h"
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Game/Global/Managers/TypeDataManager.h>
#include <Immie/Type/ImmieType.h>
#include <Immie/Type/BattleTypeComponent.h>

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
	UTypeDataManager::RegisterTypeConstants(FolderName, TypeConstantsOverride);

	ULogger::Log("Loading override types for battle at folder " + FolderName, LogVerbosity_Warning);
	UTypeDataManager::RegisterTypes(BattleInstance, FolderName, &TypesOverride, TypeConstantsOverride);
}

void UBattleTypeManager::SyncToClients()
{
	//iLog("Performing type data syncing");
	//TArray<EImmieType> Keys;
	//TypesOverride.GetKeys(Keys);
	//for (EImmieType Key : Keys) {
	//	UBattleTypeComponent* Type = *TypesOverride.Find(Key);
	//	Type->SyncToClients();
	//}
}

FImmieType UBattleTypeManager::GetType(EImmieType Type)
{
	if (!bUseOverride) {
		return GetTypeDataManager()->GetType(Type);
	}

	FImmieType* Found = TypesOverride.Find(Type);
	checkf(Found, TEXT("Cannot find overridden FImmieType entry for type %s"), *UTypeDataManager::GetTypeName(Type).ToString());
	return *Found;
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

TArray<FImmieType> UBattleTypeManager::GetTypes(FTypeBitmask TypeBitmask)
{
	const TArray<EImmieType> TypeEnums = TypeBitmask.GetTypes();
	TArray<FImmieType> Types;
	for (int i = 0; i < TypeEnums.Num(); i++) {
		Types.Add(GetType(TypeEnums[i]));
	}
	return Types;
}

