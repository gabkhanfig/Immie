// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum EImmieStat
{
	Stat_Health		UMETA(DisplayName = "Health"),
	Stat_Attack		UMETA(DisplayName = "Attack"),
	Stat_Defense	UMETA(DisplayName = "Defense"),
	Stat_Speed		UMETA(DisplayName = "Speed"),
};
