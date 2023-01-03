// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActor.h"
#include <Immie/ImmieCore.h>
#include <Immie/Ability/Abilities/Ability.h>
#include <Immie/Ability/Abilities/AbilityDataObject.h>
#include <Immie/Battle/Components/DamageComponent.h>
#include <Net/UnrealNetwork.h>

AAbilityActor::AAbilityActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
}

void AAbilityActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAbilityActor, Ability);
	DOREPLIFETIME(AAbilityActor, DamageComponent);
	DOREPLIFETIME(AAbilityActor, SpawnedActiveStats);
	DOREPLIFETIME(AAbilityActor, ActiveStats);
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbilityActor::InitializeForBattle(UAbility* _Ability)
{
	Ability = _Ability;
	DamageComponent = UDamageComponent::NewDamageComponent(this);
	SpawnedActiveStats = Ability->GetActiveStats();
	ActiveStats = SpawnedActiveStats;
}

UDamageComponent* AAbilityActor::GetDamageComponent_Implementation() const
{
	return DamageComponent;
}

int AAbilityActor::GetAbilityId() const
{
	return GetAbility()->GetAbilityId();
}

AImmieCharacter* AAbilityActor::GetImmieCharacter() const
{
	return GetAbility()->GetImmieCharacter();
}

ABattleTeam* AAbilityActor::GetTeam() const
{
	return GetAbility()->GetTeam();
}

ABattleInstance* AAbilityActor::GetBattleInstance() const
{
	return GetAbility()->GetBattleInstance();
}

bool AAbilityActor::HasBattleAuthority() const
{
	return GetAbility()->HasBattleAuthority();
}

UAbilityDataObject* AAbilityActor::GetAbilityDataObject() const
{
	return GetAbility()->GetAbilityDataObject();
}

FName AAbilityActor::GetAbilityName() const
{
	return GetAbility()->GetAbilityName();
}

FAbilityFlags AAbilityActor::GetAbilityFlags() const
{
	return GetAbility()->GetAbilityFlags();
}

float AAbilityActor::GetDamagePower() const
{
	return GetAbility()->GetDamagePower();
}

float AAbilityActor::GetDamageDuration() const
{
	return GetAbility()->GetDamageDuration();
}

float AAbilityActor::GetHealingPower() const
{
	return GetAbility()->GetHealingPower();
}

float AAbilityActor::GetSpeed() const
{
	return GetAbility()->GetSpeed();
}

float AAbilityActor::GetRange() const
{
	return GetAbility()->GetRange();
}

TArray<UImmieType*> AAbilityActor::GetType()
{
	return GetAbility()->GetType();
}

FBattleStats AAbilityActor::GetAbilityInitialStats() const
{
	return GetAbility()->GetInitialStats();
}

FBattleStats AAbilityActor::GetAbilityActiveStats() const
{
	return GetAbility()->GetActiveStats();
}

