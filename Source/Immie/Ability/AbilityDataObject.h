// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include "AbilityDataTypes.h"
#include <Immie/Core/Structures.h>
#include "AbilityDataObject.generated.h"

class UAbility;
class AAbilityActor;
class ADummyAbilityActor;

UCLASS(Blueprintable)
/* Data object storing ability data. Override in blueprints for custom ability data.
Automatically loads references to other ability classes.
Ensure those classes follow correct naming conventions (example ExampleAbility_BP).
The value of SpecieName MUST be set. From the above example, the name would be "exampleAbility". */
class IMMIE_API UAbilityDataObject : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/* This must be set in blueprint. Ability name should be camelCase formatted. */
		FName AbilityName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/**/
		TSubclassOf<UAbility> AbilityClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		bool bUseActorClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bUseActorClass"))
		/**/
		bool bUseDummyActorClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (NoResetToDefault, EditCondition="bUseActorClass"))
		/**/
		TSubclassOf<AAbilityActor> ActorClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (NoResetToDefault, EditCondition="bUseDummyActorClass"))
		/**/
		TSubclassOf<ADummyAbilityActor> DummyActorClass;

	UPROPERTY(BlueprintReadOnly)
		/* Bitmask containing all of this ability's types. Used in MOST cases to figure out which battle types to use. */
		int TypeBitmask;

	UPROPERTY(BlueprintReadOnly)
		/* The initial cooldown amount of this ability. Always starts the cooldown with this amount. */
		float InitialCooldown;

	UPROPERTY(BlueprintReadOnly)
		/* The maximum cooldown of this ability. The current cooldown counts down from this value to 0. */
		float MaxCooldown;

	UPROPERTY(BlueprintReadOnly)
		/* The initial uses amount of this ability. Always starts the uses with this amount. */
		int InitialUses;

	UPROPERTY(BlueprintReadOnly)
		/* The maximum amount of uses this ability has. When the cooldown finishes and the ability uses are restocked, the current uses will be set to this value. */
		int MaxUses;

	UPROPERTY(BlueprintReadOnly)
		/* Used in damage calculation formula. This is separate from any multiplier. */
		float DamagePower;

	UPROPERTY(BlueprintReadOnly)
		/* The amount of time the damage takes to deal all of it's damage. */
		float DamageDuration;

	UPROPERTY(BlueprintReadOnly)
		/* Used in healing calculation formula. This is seperate from any multiplier. */
		float HealingPower;

	UPROPERTY(BlueprintReadOnly)
		/* The amount of time the healing takes to deal all of it's healing. */
		float HealingDuration;

	UPROPERTY(BlueprintReadOnly)
		/* The speed value for the ability actor of this ability. Can be used in any way. */
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		/* Maximum distance value for the ability actor of this ability. Can be used in any way. */
		float Range;

	UPROPERTY(BlueprintReadOnly)
		/**/
		float MaxDelay;

	UPROPERTY(BlueprintReadOnly)
		/**/
		float Gravity;

	UPROPERTY(BlueprintReadOnly)
		/* This ability's own base stats. Not it's owning specie. */
		FBaseStats BaseStats;

	UPROPERTY(BlueprintReadOnly)
		/* The relative stat values for this ability as multipliers of it's owning Immie's stats. */
		FBattleStats RelativeStats;

protected:

	UFUNCTION(BlueprintImplementableEvent)
		/**/
		void BP_LoadAbilityJsonData(const FJsonObjectBP& Json);

	UFUNCTION(BlueprintNativeEvent)
		/**/
		FJsonObjectBP BP_AbilityDataToJson();

public:

	UAbilityDataObject();

	void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	TSubclassOf<UAbility> FetchAbilityComponentClass() const;

	/* Not all abilities require an ability actor class so this function returning nullptr is fine. */
	TSubclassOf<AAbilityActor> FetchAbiltyActorClass() const;

	/* Not all abilities require a dummy ability actor class so this function returning nullptr is fine. */
	TSubclassOf<ADummyAbilityActor> FetchDummyAbilityActorClass() const;

	static const FString& GetAbilitiesBlueprintFolder();

	static UAbilityDataObject* CreateAbilityDataObject(UObject* Outer, UClass* DataObjectClass);

	void LoadAbilityJsonData(const FJsonObjectBP& Json);

	FJsonObjectBP AbilityDataToJson();

	UFUNCTION(BlueprintPure)
		/**/
		FName GetAbilityName() const { return AbilityName; }

	UFUNCTION(BlueprintPure)
		/**/
		TSubclassOf<UAbility> GetAbilityClass() const { return AbilityClass; }

	UFUNCTION(BlueprintPure)
		/**/
		TSubclassOf<AAbilityActor> GetActorClass() const { return ActorClass; }

	UFUNCTION(BlueprintPure)
		/**/
		TSubclassOf<ADummyAbilityActor> GetDummyActorClass() const { return DummyActorClass; }

	UFUNCTION(BlueprintPure)
		/**/
		int GetTypeBitmask() const { return TypeBitmask; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetInitialCooldown() const { return InitialCooldown; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetMaxCooldown() const { return MaxCooldown; }

	UFUNCTION(BlueprintPure)
		/**/
		int GetInitialUses() const { return InitialUses; }

	UFUNCTION(BlueprintPure)
		/**/
		int GetMaxUses() const { return MaxUses; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetDamagePower() const { return DamagePower; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetDamageDuration() const { return DamageDuration; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetHealingPower() const { return HealingPower; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetHealingDuration() const { return HealingDuration; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetRange() const { return Range; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetMaxDelay() const { return MaxDelay; }

	UFUNCTION(BlueprintPure)
		/**/
		float GetGravity() const { return Gravity; }

	UFUNCTION(BlueprintPure)
		/**/
		FBaseStats GetBaseStats() const { return BaseStats; }

	UFUNCTION(BlueprintPure)
		/**/
		FBattleStats GetRelativeStats() const { return RelativeStats; }

};
