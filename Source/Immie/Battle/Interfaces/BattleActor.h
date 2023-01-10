// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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

Due to the limitations of unreal UInterfaces, this functions more of an abstract class rather than a full interface.
*/
class IMMIE_API IBattleActor
{
	GENERATED_BODY()

protected:
	
public:

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/* Call with IBattleActor::Execute_GetDamageComponent(). */
		UDamageComponent* GetDamageComponent() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/* Check if the implemented interface object is using the collider for abilities. Call with IBattleActor::Execute_IsValidAbilityCollider(). */
		bool IsValidAbilityCollider(UPrimitiveComponent* Collider) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/* Get the total healing an ability will do. Does not factor in healing over time multipliers. Call with IBattleActor::Execute_TotalHealingFromAbility(). */
		float TotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/* Get the total damage an ability will do. Does not factor in damage over time multipliers. Call with IBattleActor::Execute_TotalDamageFromAbility(). */
		float TotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/**/
		bool CanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/* Call with IBattleActor::Execute_CanBeDamagedByAbilityActor(). */
		bool CanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/* Call with IBattleActor::Execute_GetTeam(). */
		ABattleTeam* GetTeam() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, DisplayName = "Is Ally")
		/* Check if another battle actor is an ally or not. */
		bool BP_IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/**/
		FBattleStats GetBattleActorActiveStats() const;

	bool IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/**/
		void AuthorityBattleTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/**/
		void ClientBattleTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/* Called to update the visuals of this battle actor. Only executes on non-server-only side. This means clients and standalone. */
		void UpdateVisuals();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/**/
		void BattleActorIncreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/**/
		void BattleActorDecreaseHealth(float Amount);

};
