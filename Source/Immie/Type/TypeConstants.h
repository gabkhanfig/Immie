// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TypeConstants.generated.h"

USTRUCT(BlueprintType)
/* Container for data that is persistent between all types within a given group (ie system default, battle override) */
struct FTypeConstants 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float WeaknessMultiplier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float ResistanceMultiplier;
};
