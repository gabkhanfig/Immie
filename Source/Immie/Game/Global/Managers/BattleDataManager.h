// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataManager.h"
#include <Immie/Battle/BattleStructures.h>
#include "BattleDataManager.generated.h"

class UBattleInstance;
class UBattleTeam;

/**
 * 
 */
UCLASS()
class IMMIE_API UBattleDataManager : public UGameDataManager
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/**/
		TMap<FName, UClass*> BattleInstanceClasses;

	UPROPERTY()
		/**/
		TMap<TEnumAsByte<EBattleTeamType>, UClass*> TeamClasses;

protected:

	virtual void LoadDefaultGameData() override;

public:

	void RegisterBattleInstanceClasses(TMap<FName, UClass*>* MapOut);

	void RegisterBattleInstanceClass(FName Name, const FString& ClassBlueprintName, TMap<FName, UClass*>* MapOut);

	void RegisterTeamClasses(TMap<TEnumAsByte<EBattleTeamType>, UClass*>* MapOut);

	void RegisterTeamClass(TEnumAsByte<EBattleTeamType> TeamType, const FString& ClassBlueprintName, TMap<TEnumAsByte<EBattleTeamType>, UClass*>* MapOut);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetBattleInstanceClass(FName Name);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetTeamClass(TEnumAsByte<EBattleTeamType> TeamType);


};
