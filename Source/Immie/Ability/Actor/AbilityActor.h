// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Immie/Core/Structures.h>
#include <Immie/Battle/Interfaces/BattleActor.h>
#include <Immie/Ability/AbilityDataTypes.h>
#include "AbilityActor.generated.h"

class UAbility;
class AImmieCharacter;
class UDamageComponent;

UCLASS()
class IMMIE_API AAbilityActor : public AActor, public IBattleActor
{
	GENERATED_BODY()

private:

	UPROPERTY(replicated)
		/* The ability that "owns" this ability actor. Replicated via lifetime props. */
		UAbility* Ability;

	UPROPERTY(replicated)
		/* The damage component of this ability actor. Replicated via lifetime props. */
		UDamageComponent* DamageComponent;

	UPROPERTY(replicated)
		/* The active stats of the owning ability component when this ability actor was spawned. Replicated via lifetime props. */
		FBattleStats SpawnedActiveStats;

protected:

	UPROPERTY(BlueprintReadWrite, replicated)
		/* The active stats of this ability actor. Health is the current health. Uses stat modifiers. Replicated via lifetime props. */
		FBattleStats ActiveStats;

	UPROPERTY(BlueprintReadWrite)
		/* Only exists on client. */
		AAbilityActor* VisualDummy;

public:	
	// Sets default values for this actor's properties
	AAbilityActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeForBattle(UAbility* _Ability);

	/**/
	virtual UDamageComponent* GetDamageComponent_Implementation() const;

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE UAbility* GetAbility() const { return Ability; }

	UFUNCTION(BlueprintPure)
		/* Get the id number of this ability. */
		int GetAbilityId() const;

	UFUNCTION(BlueprintPure)
		/* Get the Immie character that owns this ability. */
		AImmieCharacter* GetImmieCharacter() const;

	UFUNCTION(BlueprintPure)
		/* Get this abilities battle team. */
		ABattleTeam* GetTeam() const;

	UFUNCTION(BlueprintPure)
		/**/
		ABattleInstance* GetBattleInstance() const;

	UFUNCTION(BlueprintPure)
		/* Does this ability's team have authority in battle. */
		bool HasBattleAuthority() const;

	UFUNCTION(BlueprintPure)
		/* Get the ability data object that corresponds to this ability. */
		UAbilityDataObject* GetAbilityDataObject() const;

	UFUNCTION(BlueprintPure)
		/* Get the name of this ability. */
		FName GetAbilityName() const;

	UFUNCTION(BlueprintPure)
		/**/
		FAbilityFlags GetAbilityFlags() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetDamagePower() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetDamageDuration() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetHealingPower() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetSpeed() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetRange() const;

	UFUNCTION(BlueprintPure)
		/**/
		TArray<UImmieType*> GetType();

	UFUNCTION(BlueprintPure)
		/**/
		FBattleStats GetSpawnedActiveStats() const { return SpawnedActiveStats; }

	UFUNCTION(BlueprintPure)
		/**/
		FBattleStats GetActiveStats() const { return ActiveStats; }

	UFUNCTION(BlueprintPure)
		/**/
		FBattleStats GetAbilityInitialStats() const;

	UFUNCTION(BlueprintPure)
		/**/
		FBattleStats GetAbilityActiveStats() const;

};
