// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Structures.generated.h"

USTRUCT(BlueprintType)
struct FBaseStats 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		uint8 Health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		uint8 Attack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		uint8 Defense;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		uint8 Speed;

	bool operator == (const FBaseStats& other) {
		return
			Health == other.Health &&
			Attack == other.Attack &&
			Defense == other.Defense &&
			Speed == other.Speed;
	}

	bool operator != (const FBaseStats& other) {
		return !(*this == other);
	}
};

USTRUCT(BlueprintType)
struct FBattleStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float Health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float Attack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float Defense;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float Speed;

	FBattleStats();

	bool operator == (const FBattleStats& other) {
		return
			Health == other.Health &&
			Attack == other.Attack &&
			Defense == other.Defense &&
			Speed == other.Speed;
	}

	bool operator != (const FBattleStats& other) {
		return !(*this == other);
	}
};