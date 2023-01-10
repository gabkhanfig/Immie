// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Immie/Battle/BattleStructures.h>
#include <Immie/Ability/AbilityDataTypes.h>
#include "DamageComponent.generated.h"

class IBattleActor;

UCLASS()
class IMMIE_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	/* The healing values to iterate through every authoritative battle tick. */
	TArray<FBattleDamage> Healing;

	/* The damage values to iterate through every authoritative battle tick. */
	TArray<FBattleDamage> Damage;

protected:



public:	

	// Sets default values for this component's properties
	UDamageComponent();

	UFUNCTION(BlueprintCallable)
		/**/
		static UDamageComponent* NewDamageComponent(TScriptInterface<IBattleActor> Owner);

	UFUNCTION(BlueprintCallable)
		/**/
		void AddHealing(const FAbilityInstigatorDamage& AbilityHealing);

	UFUNCTION(BlueprintCallable)
		/**/
		void AddDamage(const FAbilityInstigatorDamage& AbilityDamage);

	/* Authority active battle ticking for this damage component. */
	void AuthorityBattleTick(float DeltaTime);

	/* Client active battle ticking for this damage component. */
	void ClientBattleTick(float DeltaTime);

	UFUNCTION(BlueprintPure)
		/* SHOULD be valid on replicated instances. */
		TScriptInterface<IBattleActor> GetBattleActor();

	static float ExecuteAmount(FBattleDamage& Values, float DeltaTime);

	static float GetMultiplierThisTick(const FBattleDamage& Values);

		
};
