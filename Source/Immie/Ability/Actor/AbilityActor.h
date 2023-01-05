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
class ADummyAbilityActor;

UCLASS()
class IMMIE_API AAbilityActor : public AActor, public IBattleActor
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/* The ability that "owns" this ability actor. Replicated via serialization on spawn. DO NOT CHANGE ON CLIENT. */
		UAbility* Ability;

	UPROPERTY()
		/* The damage component of this ability actor. Is constructed with this ability actor. Replicated via serialization on spawn. DO NOT CHANGE ON CLIENT. */
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
		ADummyAbilityActor* Dummy;

public:	
	// Sets default values for this actor's properties
	AAbilityActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
		/* Informs clients or standalone of spawn. */
		void InformSpawn();

	UFUNCTION()
		/**/
		void SpawnVisualDummy();

	UFUNCTION()
		/**/
		ADummyAbilityActor* CreateDummyActor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/**/
		FTransform GetDummySpawnTransform();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetAbility(UAbility* _Ability);

	/**/
	void InitializeForBattle();

	virtual void OnActorChannelOpen(class FInBunch& InBunch, class UNetConnection* Connection) override;
	virtual void OnSerializeNewActor(class FOutBunch& OutBunch) override;

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
