// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieGlobal.h"
#include <Immie/Game/ImmieGameInstance.h>

UImmieGameInstance* _ImmieGameInstance;

UImmieGameInstance* GetImmieGameInstance()
{
	return _ImmieGameInstance;
}

UTypeDataManager* GetTypeDataManager()
{
	return GetImmieGameInstance()->_GetTypeDataManager();
}

UAbilityDataManager* GetAbilityDataManager()
{
	return GetImmieGameInstance()->_GetAbilityDataManager();
}

USpecieDataManager* GetSpecieDataManager()
{
	return GetImmieGameInstance()->_GetSpecieDataManager();
}

UBattleDataManager* GetBattleDataManager()
{
	return GetImmieGameInstance()->_GetBattleDataManager();
}

UPlayerImmies* GetPlayerImmies()
{
	return GetImmieGameInstance()->_GetPlayerImmies();
}

USaveManager* GetSaveManager()
{
	return GetImmieGameInstance()->_GetSaveManager();
}

UConfigDataManager* GetConfigDataManager()
{
	return GetImmieGameInstance()->_GetConfigDataManager();
}

UTrainerDataManager* GetTrainerDataManager()
{
	return GetImmieGameInstance()->_GetTrainerDataManager();
}

USpawnTableManager* GetSpawnTableManager()
{
	return GetImmieGameInstance()->_GetSpawnTableManager();
}
