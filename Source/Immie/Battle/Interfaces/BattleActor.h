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
		/* Get the damage component of this battle actor. DO NOT OVERRIDE. */
		UDamageComponent* BattleActorGetDamageComponent() const;
	virtual UDamageComponent* GetDamageComponent() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Check if the implemented interface object is using the collider for abilities. DO NOT OVERRIDE. */
		bool BattleActorIsValidAbilityCollider(UPrimitiveComponent* Collider) const;
	virtual bool IsValidAbilityCollider(UPrimitiveComponent* Collider) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Get the total healing an ability will do. Does not factor in healing over time multipliers. DO NOT OVERRIDE. */
		float BattleActorTotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const;
	virtual float TotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Get the total damage an ability will do. Does not factor in damage over time multipliers. DO NOT OVERRIDE. */
		float BattleActorTotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const;
	virtual float TotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* DO NOT OVERRIDE. */
		bool BattleActorCanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const;
	virtual bool CanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* DO NOT OVERRIDE. */
		bool BattleActorCanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const;
	virtual bool CanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* DO NOT OVERRIDE. */
		ABattleTeam* BattleActorGetTeam() const;
	virtual ABattleTeam* GetTeam() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		TArray<UImmieType*> BattleActorGetType() const;
	virtual TArray<UImmieType*> GetType() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* Check if another battle actor is an ally or not. DO NOT OVERRIDE. */
		bool BattleActorIsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const;
	virtual bool IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		FBattleStats BattleActorGetInitialStats() const;
	virtual FBattleStats GetInitialStats() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/* DO NOT OVERRIDE. */
		FBattleStats BattleActorGetActiveStats() const;
	virtual FBattleStats GetActiveStats() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		FString BattleActorGetDisplayName() const;
	virtual FString GetDisplayName() const = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* DO NOT OVERRIDE. */
		void BattleActorAuthorityBattleTick(float DeltaTime);
	virtual void AuthorityBattleTick(float DeltaTime) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* DO NOT OVERRIDE. */
		void BattleActorClientBattleTick(float DeltaTime);
	virtual void ClientBattleTick(float DeltaTime) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* Called to update the visuals of this battle actor. Only executes on non-server-only side. This means clients and standalone. DO NOT OVERRIDE. */
		void BattleActorUpdateVisuals();
	virtual void UpdateVisuals() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* DO NOT OVERRIDE. */
		void BattleActorIncreaseHealth(float Amount);
	virtual void IncreaseHealth(float Amount) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/* DO NOT OVERRIDE. */
		void BattleActorDecreaseHealth(float Amount);
	virtual void DecreaseHealth(float Amount) = 0;

};

