// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include <Immie/Type/ImmieType.h>
#include "AbilityDataTypes.generated.h"

class UAbility;
class UBattleTypeComponent;
class AAbilityActor;

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
		TArray<EImmieType> InstigatorType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		UBattleTypeComponent* DefenderTypeComponent;

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

	FAbilityInstigatorDamage();
		
};

USTRUCT(BlueprintType)
/**/
struct FSerializedAbilityData
{
	GENERATED_BODY();

	UPROPERTY()
		/**/
		FName AbilityName;;

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