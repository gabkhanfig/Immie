// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <Immie/ImmieCore.h>
#include "ImmieGameInstance.generated.h"

class UTypeDataManager;
class UAbilityDataManager;
class USpecieDataManager;
class UBattleDataManager;
class UPlayerImmies;
class USaveManager;
class UConfigDataManager;

/**
 * 
 */
UCLASS()
class IMMIE_API UImmieGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
		/**/
		UTypeDataManager* TypeDataManager;

	UPROPERTY(BlueprintReadOnly)
		/**/
		UAbilityDataManager* AbilityDataManager;

	UPROPERTY(BlueprintReadOnly)
		/**/
		USpecieDataManager* SpecieDataManager;

	UPROPERTY(BlueprintReadOnly)
		/**/
		UBattleDataManager* BattleDataManager;

	UPROPERTY(BlueprintReadOnly)
		/**/
		UPlayerImmies* PlayerImmies;

	UPROPERTY(BlueprintReadOnly)
		/**/
		USaveManager* SaveManager;

	UPROPERTY(BlueprintReadOnly)
		/**/
		UConfigDataManager* ConfigDataManager;

private:

	void InitializeGameDataManagers();

protected:

	void Init();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="Immie Game Init")
		/* Blueprint-side initialization for Immie application. */
		void BP_ImmieGameInit();

public:

	FORCEINLINE UTypeDataManager* _GetTypeDataManager() const { return TypeDataManager; }

	FORCEINLINE USpecieDataManager* _GetSpecieDataManager() const { return SpecieDataManager; }

	FORCEINLINE UAbilityDataManager* _GetAbilityDataManager() const { return AbilityDataManager; }

	FORCEINLINE UBattleDataManager* _GetBattleDataManager() const { return BattleDataManager; }

	FORCEINLINE UPlayerImmies* _GetPlayerImmies() const { return PlayerImmies; }

	FORCEINLINE USaveManager* _GetSaveManager() const { return SaveManager; }

	FORCEINLINE UConfigDataManager* _GetConfigDataManager() const { return ConfigDataManager; }
};
