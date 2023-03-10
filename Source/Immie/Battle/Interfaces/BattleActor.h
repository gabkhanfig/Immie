// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <Immie/Core/Structures.h>
#include <Immie/Ability/AbilityDataTypes.h>
#include "BattleActor.generated.h"

class UDamageComponent;
class UImmieType;
class UAbility;
class ABattleTeam;

UINTERFACE(Blueprintable)
/* DO NOT MODIFY. */
class UBattleActor : public UInterface
{
	GENERATED_BODY()
};

/* Interface for battle relevant actors.
*
* NOTE:
* One time the game stopped compiling out of no where. It turned out to be related to the battle actor interface.
* It can occasionally happen where the Execute_ functions will not exist, leading to having to uncomment out everything. 
* This is why each battle actor would need to implement the functions themselves THROUGH their c++ subclasses.
*/
class IMMIE_API IBattleActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Get the damage component of this battle actor. */
		UDamageComponent* GetDamageComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Check if the implemented interface object is using the collider for abilities. */
		bool IsValidAbilityCollider(UPrimitiveComponent* Collider) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Get the total healing an ability will do. Does not factor in healing over time multipliers. */
		float TotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Get the total damage an ability will do. Does not factor in damage over time multipliers. */
		float TotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* See if this battle actor can be healed by a provided ability actor. */
		bool CanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* See if this battle actor can be damaged by a provided ability actor. */
		bool CanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/*  */
		ABattleTeam* GetTeam() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		TArray<UImmieType*> GetType() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Check if another battle actor is an enemy or not. */
		bool IsEnemy(const TScriptInterface<IBattleActor>& OtherBattleActor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		FBattleStats GetInitialStats() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/*  */
		FBattleStats GetActiveStats() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/*  */
		FString GetDisplayName() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/*  */
		void AuthorityBattleTick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/*  */
		void ClientBattleTick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* Called to update the visuals of this battle actor. Only executes on non-server-only side. This means clients and standalone. */
		void UpdateVisuals();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/*  */
		void IncreaseHealth(float Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/*  */
		void DecreaseHealth(float Amount);

};

