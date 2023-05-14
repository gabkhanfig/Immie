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
		TArray<FName> Initial;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level10;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level20;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level30;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level40;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level50;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level60;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level70;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level80;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level90;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<FName> Level100;
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
		FString JsonString;
};

UENUM(BlueprintType)
enum class EImmieCharacterMode : uint8
{
	None,
	Battle,
	Wild
};

