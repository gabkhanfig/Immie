// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include <Immie/Battle/Interfaces/BattleActor.h>
#include <Immie/Ability/Ability.h>
#include <Immie/Type/ImmieType.h>
#include <Immie/Immies/ImmieCharacter.h>

UDamageComponent::UDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

UDamageComponent* UDamageComponent::NewDamageComponent(TScriptInterface<IBattleActor> Owner)
{
	UDamageComponent* DamageComponent = NewObject<UDamageComponent>(Cast<AActor>(Owner.GetObject()));
	return DamageComponent;
}

void UDamageComponent::AddHealing(const FAbilityInstigatorDamage& AbilityHealing)
{
	float TotalHealing = IBattleActor::Execute_TotalHealingFromAbility(GetBattleActor().GetObject(), AbilityHealing);
	FBattleDamage HealingData;
	HealingData.Ability = AbilityHealing.Instigator;
	HealingData.Amount = TotalHealing;
	HealingData.TotalTime = AbilityHealing.Duration;
	HealingData.RemainingTime = AbilityHealing.Duration;
	HealingData.Garbage = false;
	Healing.Add(HealingData);
}

void UDamageComponent::AddDamage(const FAbilityInstigatorDamage& AbilityDamage)
{
	float TotalDamage = IBattleActor::Execute_TotalDamageFromAbility(GetBattleActor().GetObject(), AbilityDamage);
	FBattleDamage DamageData;
	DamageData.Ability = AbilityDamage.Instigator;
	DamageData.Amount = TotalDamage;
	DamageData.TotalTime = AbilityDamage.Duration;
	DamageData.RemainingTime = AbilityDamage.Duration;
	DamageData.Garbage = false;
	Damage.Add(DamageData);
}

void UDamageComponent::AuthorityBattleTick(float DeltaTime)
{
	// Do healing to allow battle actors to survive at low health if there is healing they can do.
	for (int i = 0; i < Healing.Num(); i++) {
		FBattleDamage& Heal = Healing[i];
		if (Heal.Garbage) {
			Healing.RemoveAt(i);
			i--;
			continue;
		}
		float Amount = UDamageComponent::ExecuteAmount(Heal, DeltaTime); 
		if (Heal.Ability != nullptr && Amount != 0.f) {
			Heal.Ability->GetImmieCharacter()->EventPlayerDealtHealing(GetBattleActor(), Amount, Heal);
		}
		IBattleActor::Execute_IncreaseHealth(GetBattleActor().GetObject(), Amount);
	}

	for (int i = 0; i < Damage.Num(); i++) {
		FBattleDamage& Dmg = Damage[i];
		if (Dmg.Garbage) {
			Damage.RemoveAt(i);
			i--;
			continue;
		}
		float Amount = UDamageComponent::ExecuteAmount(Dmg, DeltaTime);
		if (Dmg.Ability != nullptr && Amount != 0.f) {
			Dmg.Ability->GetImmieCharacter()->EventPlayerDealtDamage(GetBattleActor(), Amount, Dmg);
		}
		IBattleActor::Execute_DecreaseHealth(GetBattleActor().GetObject(), Amount);
	}
}

void UDamageComponent::ClientBattleTick(float DeltaTime)
{
}

TScriptInterface<IBattleActor> UDamageComponent::GetBattleActor()
{
	return GetOuter();
}

float UDamageComponent::ExecuteAmount(FBattleDamage& Values, float DeltaTime)
{
	const float Multiplier = UDamageComponent::GetMultiplierThisTick(Values);

	if (Values.RemainingTime <= 0.0f) {
		Values.Garbage = true;
		return Values.Amount * Multiplier;
	}
	else {
		const float DamageThisTick = Values.Amount * DeltaTime / Values.RemainingTime;
		Values.Amount -= DamageThisTick;
		Values.RemainingTime -= DeltaTime;

		if (Values.Amount < 0) {
			Values.Amount = 0;
			Values.Garbage = true;
		}

		return DamageThisTick * Multiplier;
	}
}

float UDamageComponent::GetMultiplierThisTick(const FBattleDamage& Values)
{
	const float ElapsedTime = Values.TotalTime - Values.RemainingTime;
	if (Values.Ability == nullptr) return 1;
	return Values.Ability->TimeDamageMultiplier(ElapsedTime);
}

