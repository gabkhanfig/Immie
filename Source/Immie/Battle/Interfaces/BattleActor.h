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

protected:
	
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Get the damage component of this battle actor. */
		UDamageComponent* GetDamageComponent() const;
	//virtual UDamageComponent* GetDamageComponent() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Check if the implemented interface object is using the collider for abilities. */
		bool IsValidAbilityCollider(UPrimitiveComponent* Collider) const;
	//virtual bool IsValidAbilityCollider(UPrimitiveComponent* Collider) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Get the total healing an ability will do. Does not factor in healing over time multipliers. */
		float TotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const;
	//virtual float TotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Get the total damage an ability will do. Does not factor in damage over time multipliers. */
		float TotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const;
	//virtual float TotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* DO NOT OVERRIDE. */
		bool CanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const;
	//virtual bool CanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* DO NOT OVERRIDE. */
		bool CanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const;
	//virtual bool CanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* DO NOT OVERRIDE. */
		ABattleTeam* GetTeam() const;
	//virtual ABattleTeam* GetTeam() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		TArray<UImmieType*> GetType() const;
	//virtual TArray<UImmieType*> GetType() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Check if another battle actor is an ally or not. */
		bool IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const;
	//virtual bool IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		FBattleStats GetInitialStats() const;
	//virtual FBattleStats GetInitialStats() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* DO NOT OVERRIDE. */
		FBattleStats GetActiveStats() const;
	//virtual FBattleStats GetActiveStats() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		FString GetDisplayName() const;
	//virtual FString GetDisplayName() const = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* DO NOT OVERRIDE. */
		void AuthorityBattleTick(float DeltaTime);
	//virtual void AuthorityBattleTick(float DeltaTime) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* DO NOT OVERRIDE. */
		void ClientBattleTick(float DeltaTime);
	//virtual void ClientBattleTick(float DeltaTime) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* Called to update the visuals of this battle actor. Only executes on non-server-only side. This means clients and standalone. */
		void UpdateVisuals();
	//virtual void UpdateVisuals() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* DO NOT OVERRIDE. */
		void IncreaseHealth(float Amount);
	//virtual void IncreaseHealth(float Amount) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* DO NOT OVERRIDE. */
		void DecreaseHealth(float Amount);
	//virtual void DecreaseHealth(float Amount) = 0;

};

