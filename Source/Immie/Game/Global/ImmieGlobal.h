// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImmieGlobal.generated.h"

class UImmieGameInstance;
class UTypeDataManager;
class UAbilityDataManager;
class USpecieDataManager;
class UBattleDataManager;
class UPlayerImmies;
class USaveManager;
class UConfigDataManager;

extern UImmieGameInstance* _ImmieGameInstance;

/**/
UImmieGameInstance* GetImmieGameInstance();

/**/
UTypeDataManager* GetTypeDataManager();

/**/
UAbilityDataManager* GetAbilityDataManager();

/**/
USpecieDataManager* GetSpecieDataManager();

/**/
UBattleDataManager* GetBattleDataManager();

/**/
UPlayerImmies* GetPlayerImmies();

/**/
USaveManager* GetSaveManager();

/**/
UConfigDataManager* GetConfigDataManager();

UCLASS()
class UImmieGlobal : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, DisplayName="Get Immie Game Instance")
		/**/
		static inline UImmieGameInstance* BP_GetImmieGameInstance() { return GetImmieGameInstance(); }

	UFUNCTION(BlueprintPure, DisplayName="Get Type Data Manager")
		/**/
		static inline UTypeDataManager* BP_GetTypeDataManager() { return GetTypeDataManager(); }

	UFUNCTION(BlueprintPure, DisplayName="Get Ability Data Manager")
		/**/
		static inline UAbilityDataManager* BP_GetAbilityDataManager() { return GetAbilityDataManager(); }
		
	UFUNCTION(BlueprintPure, DisplayName = "Get Specie Data Manager")
		/**/
		static inline USpecieDataManager* BP_GetSpecieDataManager() { return GetSpecieDataManager();}

	UFUNCTION(BlueprintPure, DisplayName = "Get Player Immies")
		/**/
		static inline UPlayerImmies* BP_GetPlayerImmies() { return GetPlayerImmies(); }

	UFUNCTION(BlueprintPure, DisplayName = "Get Save Manager")
		/**/
		static inline USaveManager* BP_GetSaveManager() { return GetSaveManager(); }

	UFUNCTION(BlueprintPure)
		/**/
		static inline UConfigDataManager* BP_GetConfigDataManager() { return GetConfigDataManager(); }
};
