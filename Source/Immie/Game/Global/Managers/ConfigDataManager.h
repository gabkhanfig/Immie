// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataManager.h"
#include "ConfigDataManager.generated.h"

/**
 * 
 */
UCLASS()
class IMMIE_API UConfigDataManager : public UGameDataManager
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/**/
		uint8 MaxLevel;

	UPROPERTY()
		/**/
		uint8 MaxStatLevel;

	UPROPERTY()
		/**/
		uint8 MaxBaseStat;

	UPROPERTY()
		/**/
		int MaxXp;

	UPROPERTY()
		/**/
		int MaxPlayerTeamSize;

protected:

	virtual void LoadDefaultGameData() override;

public:

	UFUNCTION(BlueprintPure)
		/**/
		uint8 GetMaxLevel() const { return MaxLevel; }

	UFUNCTION(BlueprintPure)
		/**/
		uint8 GetMaxStatLevel() const { return MaxStatLevel; }

	UFUNCTION(BlueprintPure)
		/**/
		uint8 GetMaxBaseStat() const { return MaxBaseStat; }

	UFUNCTION(BlueprintPure)
		/**/
		int GetMaxXp() const { return MaxXp; }

	UFUNCTION(BlueprintPure)
		/**/
		int GetMaxPlayerTeamSize() const { return MaxPlayerTeamSize; }

};
