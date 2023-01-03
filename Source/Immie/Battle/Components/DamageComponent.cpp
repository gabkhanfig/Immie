// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include <Immie/Battle/Interfaces/BattleActor.h>
#include <Immie/Ability/Abilities/Ability.h>
#include <Immie/Type/ImmieType.h>

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
	float TotalHealing = GetBattleActor()->TotalHealingFromAbility(AbilityHealing);
	FBattleDamage HealingData;
	HealingData.Ability = AbilityHealing.Instigator;
	HealingData.Amount = TotalHealing;
	HealingData.TotalTime = AbilityHealing.Duration;
	HealingData.RemainingTime = AbilityHealing.Duration;
	HealingData.bGarbage = false;
	Healing.Add(HealingData);
}

void UDamageComponent::AddDamage(const FAbilityInstigatorDamage& AbilityDamage)
{
	float TotalDamage = GetBattleActor()->TotalDamageFromAbility(AbilityDamage);
	FBattleDamage DamageData;
	DamageData.Ability = AbilityDamage.Instigator;
	DamageData.Amount = TotalDamage;
	DamageData.TotalTime = AbilityDamage.Duration;
	DamageData.RemainingTime = AbilityDamage.Duration;
	DamageData.bGarbage = false;
	Damage.Add(DamageData);
}

void UDamageComponent::AuthorityBattleTick(float DeltaTime)
{
}

void UDamageComponent::ClientBattleTick(float DeltaTime)
{
}

TScriptInterface<IBattleActor> UDamageComponent::GetBattleActor()
{
	return GetOuter();
}

