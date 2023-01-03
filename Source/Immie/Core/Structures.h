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
};