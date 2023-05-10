// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpecieDataTypes.generated.h"

USTRUCT(BlueprintType)
/**/
struct FSpecieLearnset
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Initial;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level10;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level20;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level30;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level40;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level50;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level60;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level70;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level80;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level90;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<int> Level100;
};

USTRUCT(BlueprintType)
/**/
struct FSerializedSpecieData
{
	GENERATED_BODY();

	UPROPERTY()
		/**/
		FName SpecieName;

	UPROPERTY()
		/**/
		int SpecieId;

	UPROPERTY()
		/**/
		FString JsonString;
};

UENUM(BlueprintType)
enum class EImmieCharacterMode : uint8
{
	None,
	Battle,
	Wild
};

