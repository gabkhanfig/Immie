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

UINTERFACE(NotBlueprintable)
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

	UFUNCTION(BlueprintPure, DisplayName = "Get Damage Component")
		/* Get the damage component of this battle actor. DO NOT OVERRIDE. */
		virtual UDamageComponent* BattleActorGetDamageComponent() const;
	virtual UDamageComponent* GetDamageComponent() const = 0;

	UFUNCTION(BlueprintPure)
		/* Check if the implemented interface object is using the collider for abilities. DO NOT OVERRIDE. */
		virtual bool BattleActorIsValidAbilityCollider(UPrimitiveComponent* Collider) const;
	virtual bool IsValidAbilityCollider(UPrimitiveComponent* Collider) const = 0;

	UFUNCTION(BlueprintPure)
		/* Get the total healing an ability will do. Does not factor in healing over time multipliers. DO NOT OVERRIDE. */
		virtual float BattleActorTotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const;
	virtual float TotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const = 0;

	UFUNCTION(BlueprintPure)
		/* Get the total damage an ability will do. Does not factor in damage over time multipliers. DO NOT OVERRIDE. */
		virtual float BattleActorTotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const;
	virtual float TotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const = 0;

	UFUNCTION(BlueprintPure)
		/* DO NOT OVERRIDE. */
		virtual bool BattleActorCanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const;
	virtual bool CanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const = 0;

	UFUNCTION(BlueprintPure)
		/* DO NOT OVERRIDE. */
		virtual bool BattleActorCanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const;
	virtual bool CanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const = 0;

	UFUNCTION(BlueprintPure)
		/* DO NOT OVERRIDE. */
		virtual ABattleTeam* BattleActorGetTeam() const;
	virtual ABattleTeam* GetTeam() const = 0;

	UFUNCTION(BlueprintPure)
		/**/
		virtual TArray<UImmieType*> BattleActorGetType() const;
	virtual TArray<UImmieType*> GetType() const = 0;

	UFUNCTION(BlueprintPure)
		/* Check if another battle actor is an ally or not. DO NOT OVERRIDE. */
		virtual bool IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const;

	UFUNCTION(BlueprintPure)
		/**/
		virtual FBattleStats BattleActorGetInitialStats() const;
	virtual FBattleStats GetInitialStats() const = 0;

	UFUNCTION(BlueprintPure)
		/* DO NOT OVERRIDE. */
		virtual FBattleStats BattleActorGetActiveStats() const;
	virtual FBattleStats GetActiveStats() const = 0;

	UFUNCTION(BlueprintPure)
		/**/
		virtual FString BattleActorGetDisplayName() const;
	virtual FString GetDisplayName() const = 0;
	
	UFUNCTION(BlueprintCallable)
		/* DO NOT OVERRIDE. */
		virtual void BattleActorAuthorityBattleTick(float DeltaTime);
	virtual void AuthorityBattleTick(float DeltaTime) = 0;

	UFUNCTION(BlueprintCallable)
		/* DO NOT OVERRIDE. */
		virtual void BattleActorClientBattleTick(float DeltaTime);
	virtual void ClientBattleTick(float DeltaTime) = 0;

	UFUNCTION(BlueprintCallable)
		/* Called to update the visuals of this battle actor. Only executes on non-server-only side. This means clients and standalone. DO NOT OVERRIDE. */
		virtual void BattleActorUpdateVisuals();
	virtual void UpdateVisuals() = 0;

	UFUNCTION(BlueprintCallable)
		/* DO NOT OVERRIDE. */
		virtual void BattleActorIncreaseHealth(float Amount);
	virtual void IncreaseHealth(float Amount) = 0;

	UFUNCTION(BlueprintCallable)
		/* DO NOT OVERRIDE. */
		virtual void BattleActorDecreaseHealth(float Amount);
	virtual void DecreaseHealth(float Amount) = 0;

};
