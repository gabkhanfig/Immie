// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <Immie/Ability/AbilityDataTypes.h>
#include "BattleActor.generated.h"

class UDamageComponent;
class UImmieType;
class UAbility;

UINTERFACE(MinimalAPI, Blueprintable)
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
		/**/
		UDamageComponent* GetDamageComponent() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/* Get the total healing an ability will do. Does not factor in healing over time multipliers. */
		float TotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		/* Get the total damage an ability will do. Does not factor in damage over time multipliers. */
		float TotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/**/
		void AuthorityBattleTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/**/
		void ClientBattleTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/* Called to update the visuals of this battle actor. Only executes on non-server-only side. This means clients and standalone. */
		void UpdateVisuals();

};
