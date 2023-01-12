// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include "AbilityDataTypes.generated.h"

#define INVALID_ABILITY_ID -1

class UAbility;
class AAbilityActor;
class UImmieType;

USTRUCT(BlueprintType)
/* Pass around damage or healing values instigated by an ability. */
struct FAbilityInstigatorDamage 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* The ability that is instigating the damage / healing. */
		UAbility* Instigator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		TArray<UImmieType*> Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float Power;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float AttackerStat;

	UPROPERTY()
		/**/
		float DefenderStat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float Duration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		uint8 InstigatorLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float Multiplier;
};

USTRUCT(BlueprintType)
/**/
struct FSerializedAbilityData
{
	GENERATED_BODY();

	UPROPERTY()
		/**/
		FName AbilityName;

	UPROPERTY()
		/**/
		int AbilityId;

	UPROPERTY()
		/**/
		FString JsonString;
};

USTRUCT(BlueprintType)
/* All json flags are the variable name without the b (boolean) prefix. */
struct FAbilityFlags
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Deals damage. */
		uint8 DoesDamage : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Deals healing .*/
		uint8 DoesHealing : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Copies it's owning Immie's type. */
		uint8 SameTypeImmie : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Uses base stats for calculating it's own stats. */
		uint8 BaseStats : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Uses multipliers of it's owning Immie's stats for it's own stats. */
		uint8 RelativeStats : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		uint8 VisualDummy : 1;

private:

	static bool GetJsonAbilityFlag(const FJsonObjectBP& JsonObject, const FString& FlagName);

public:

	static FAbilityFlags LoadJsonAbilityFlags(const FJsonObjectBP& JsonObject);

	FJsonObjectBP ToJson();

};