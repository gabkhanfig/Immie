// Fill out your copyright notice in the Description page of Project Settings.


#include "Formula.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

#define STAT_LEVEL_SUM(S) UKismetMathLibrary::Abs((double(S) - 1.0 + 1.0) * (1.0 + double(S)) / 2.0)

float UFormula::HealthStat(uint8 BaseStat, uint8 Level, uint8 StatLevel)
{
	if (BaseStat == 0) return 1;

	const float BaseStatF = float(BaseStat);
	const float StatLevelF = float(StatLevel);
	const float LevelF = float(Level);
	const float StatSumF = STAT_LEVEL_SUM(StatLevelF);

	return FMath::CeilToFloat
	((((StatLevelF + 4.f) * BaseStatF) / 44.f) * FMath::FloorToFloat(LevelF / 10.f)
		+ ((12.1f * BaseStatF * LevelF) + (BaseStatF * StatSumF)) / 100.f
		+ (StatLevelF * LevelF) + 198.f);
}

float UFormula::AttackStat(uint8 BaseStat, uint8 Level, uint8 StatLevel)
{
	if (BaseStat == 0) return 1;

	const float BaseStatF = float(BaseStat);
	const float StatLevelF = float(StatLevel);
	const float LevelF = float(Level);
	const float StatSumF = STAT_LEVEL_SUM(StatLevelF);

	return FMath::CeilToFloat
	((((StatLevelF + 4.f) * BaseStatF) / 44.f) * FMath::FloorToFloat(LevelF / 10.f)
		+ ((9.f * BaseStatF * LevelF) + (BaseStatF * StatSumF)) / 100.f
		+ (StatLevelF * LevelF) + 8.f);
}

float UFormula::DefenseStat(uint8 BaseStat, uint8 Level, uint8 StatLevel)
{
	return AttackStat(BaseStat, Level, StatLevel);
}

float UFormula::SpeedStat(uint8 BaseStat, uint8 StatLevel)
{
	const float BaseStatF = float(BaseStat);
	const float StatSumF = STAT_LEVEL_SUM(float(StatLevel));
	return FMath::CeilToFloat(BaseStatF * (StatSumF / 4.f) + (BaseStatF * 7.5));
}

float UFormula::Stat(TEnumAsByte<EImmieStat> StatType, uint8 BaseStat, uint8 Level, uint8 StatLevel)
{
	switch (StatType) {
	case Stat_Health:
		return HealthStat(BaseStat, Level, StatLevel);
	case Stat_Attack:
		return AttackStat(BaseStat, Level, StatLevel);
	case Stat_Defense:
		return DefenseStat(BaseStat, Level, StatLevel);
	case Stat_Speed:
		return SpeedStat(BaseStat, StatLevel);
	default:
		return 0.f;
	}
}

uint8 UFormula::LevelFromXp(int Xp)
{
	if (Xp <= 0) {
		return 1;
	}

	return floorf(99.0 * powf(float(Xp), 1.0 / 3.0) / 100.0 + 1);
}

int UFormula::MinimumXpForLevel(uint8 Level)
{
	if (Level < 1) {
		return 0;
	}

	return floorf(1000000.0 * powf((double(Level) - 1.0), 3.0) / 970299.0);
}

int UFormula::XpToNextLevel(int Xp)
{
	int NextLevelXp = MinimumXpForLevel(LevelFromXp(Xp) + 1);
	return NextLevelXp - Xp;
}

float UFormula::ProgressToNextLevel(int Xp)
{
	const double LowerBound = MinimumXpForLevel(LevelFromXp(Xp));
	const double UpperBound = MinimumXpForLevel(LevelFromXp(Xp) + 1);
	const double XpBetweenLevels = UpperBound - LowerBound;

	return double(Xp - LowerBound) / XpBetweenLevels;
}
