// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataManager.h"
#include "SpawnTableManager.generated.h"

class UWildSpawnTable;

/**
 * 
 */
UCLASS()
class IMMIE_API USpawnTableManager : public UGameDataManager
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
		/**/
		bool IsValidSpawnTable(FName TableName);

	UFUNCTION(BlueprintPure)
		/**/
		UWildSpawnTable* GetImmieSpawnTable(FName TableName);

private:

	virtual void LoadDefaultGameData() override;

	void LoadSpawnTableData(FName TableName, TMap<FName, UWildSpawnTable*>* MapOut);

	FString LoadSpawnTableJsonToString(FName TableName);

private:

	UPROPERTY()
		/**/
		TMap<FName, UWildSpawnTable*> ImmieSpawnTables;
	
};
