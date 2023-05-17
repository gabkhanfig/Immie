// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Util/Json/BlueprintJsonObject.h"
#include "WildSpawnTable.generated.h"

class UImmieSpawnData;

USTRUCT(BlueprintType)
/**/
struct FWildImmieSpawnTableElement 
{
	GENERATED_BODY(BlueprintReadWrite)

	UPROPERTY()
		UImmieSpawnData* ImmieSpawnData;

	UPROPERTY()
		int WeightIncrement;
};

/* Class that stores multiple immie spawn data objects. 
Wild immie spawners use these to spawn immies into the world. */
UCLASS()
class IMMIE_API UWildSpawnTable : public UObject
{
	GENERATED_BODY()

public:

	UWildSpawnTable();

	void LoadJsonData(const FJsonObjectBP& Json);

	UFUNCTION(BlueprintCallable)
		/**/
		UImmieSpawnData* GetWeightedRandomSpawnData();

private:

	UPROPERTY()
		/**/
		TArray<FWildImmieSpawnTableElement> Spawns;

	UPROPERTY()
		/**/
		int MaxWeight;

};
