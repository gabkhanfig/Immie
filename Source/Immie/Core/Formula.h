// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Enums.h"
#include "Formula.generated.h"

UCLASS()
class IMMIE_API UFormula : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
		/* Calculate the health stat. */
		static float HealthStat(uint8 BaseStat, uint8 Level, uint8 StatLevel);

	UFUNCTION(BlueprintPure)
		/* Calculate the attack stat. */
		static float AttackStat(uint8 BaseStat, uint8 Level, uint8 StatLevel);

	UFUNCTION(BlueprintPure)
		/* Calculate the defense stat (same formula as attack stat). */
		static float DefenseStat(uint8 BaseStat, uint8 Level, uint8 StatLevel);

	UFUNCTION(BlueprintPure)
		/* Calculate the speed stat. Does not require a specified level. */
		static float SpeedStat(uint8 BaseStat, uint8 StatLevel);

	UFUNCTION(BlueprintPure)
		/* Calculate any stat. Formula used depends on stat type. */
		static float Stat(TEnumAsByte<EImmieStat> StatType, uint8 BaseStat, uint8 Level, uint8 StatLevel);

	UFUNCTION(BlueprintPure)
		/**/
		static uint8 LevelFromXp(int Xp);

	UFUNCTION(BlueprintPure)
		/**/
		static int MinimumXpForLevel(uint8 Level);

	UFUNCTION(BlueprintPure)
		/**/
		static int XpToNextLevel(int Xp);

	UFUNCTION(BlueprintPure)
		/**/
		static float ProgressToNextLevel(int Xp);

};
