// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Immie/ImmieCore.h>
#include "../../Util/Json/BlueprintJsonObject.h"
#include "ImmieSpawnData.generated.h"

/* Object containing spawn data for particular spawn table set with conditions and more. */
UCLASS(Blueprintable)
class IMMIE_API UImmieSpawnData : public UObject
{
	GENERATED_BODY()

public:

	UImmieSpawnData();

	void LoadJsonData(const FJsonObjectBP& Json);

protected:

	UFUNCTION(BlueprintImplementableEvent)
		/**/
		void BP_LoadJsonData(const FJsonObjectBP& Json);

protected:

	UPROPERTY(BlueprintReadOnly)
		/**/
		FName ImmieName;

	UPROPERTY(BlueprintReadWrite)
		/* Minimum spawn level (default 1). Json field: "MinLevel" */
		uint8 MinLevel;

	UPROPERTY(BlueprintReadWrite)
		/* Maximum spawn level (default 1). Json field: "MaxLevel" */
		uint8 MaxLevel;

	UPROPERTY(BlueprintReadWrite)
		/* Immies, when spawned, can spawn in batches of 1 - MaxBatchSize randomly (default 1). Json field "MaxBatchSize" */
		int MaxBatchSize;

	UPROPERTY(BlueprintReadWrite)
		/* Likelihood that this Immie(s) will be spawned relative to others within the spawn table (default 1). Json field "Weight" */
		int Weight;

	
	
};
