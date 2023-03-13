// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include "AbilityDataTypes.generated.h"

#define INVALID_ABILITY_ID -1

class UAbility;
class AAbilityActor;
class UImmieType;
class UPrimitiveComponent;

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
		TArray<UImmieType*> InstigatorType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		TArray<UImmieType*> DefenderType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float Power;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float AttackerStat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
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

	FAbilityInstigatorDamage()
		: Multiplier(1)
	{}
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
/**/
struct FBattleActorColliderHitArray
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		TArray<UPrimitiveComponent*> Colliders;
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAbilityHitscanShotTarget : uint8
{
	None = 0 UMETA(Hidden),
	Enemies = 1,
};
ENUM_CLASS_FLAGS(EAbilityHitscanShotTarget);