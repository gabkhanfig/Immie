// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleActor.h"
#include <Immie/Battle/Team/BattleTeam.h>
#include <Immie/ImmieCore.h>

// Add default functionality here for any IBattleActor functions that are not pure virtual.

UDamageComponent* IBattleActor::BattleActorGetDamageComponent() const
{
  return GetDamageComponent();
}

bool IBattleActor::BattleActorIsValidAbilityCollider(UPrimitiveComponent* Collider) const
{
  return IsValidAbilityCollider(Collider);
}

float IBattleActor::BattleActorTotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const
{
  return TotalHealingFromAbility(AbilityHealing);
}

float IBattleActor::BattleActorTotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const
{
  return TotalDamageFromAbility(AbilityDamage);
}

bool IBattleActor::BattleActorCanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const
{
  return CanBeHealedByAbilityActor(AbilityActor);
}

bool IBattleActor::BattleActorCanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const
{
  return CanBeDamagedByAbilityActor(AbilityActor);
}

ABattleTeam* IBattleActor::BattleActorGetTeam() const
{
  return GetTeam();
}

TArray<UImmieType*> IBattleActor::BattleActorGetType() const
{
  return GetType();
}

bool IBattleActor::IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const
{
  ABattleTeam* Team = GetTeam();
  if (!IsValid(Team)) return false;
  return Team != nullptr && Team == OtherBattleActor->GetTeam();
}

FBattleStats IBattleActor::BattleActorGetInitialStats() const
{
  return GetInitialStats();
}

FBattleStats IBattleActor::BattleActorGetActiveStats() const
{
  return GetActiveStats();
}

FString IBattleActor::BattleActorGetDisplayName() const
{
  return GetDisplayName();
}

void IBattleActor::BattleActorAuthorityBattleTick(float DeltaTime)
{
  AuthorityBattleTick(DeltaTime);
}

void IBattleActor::BattleActorClientBattleTick(float DeltaTime)
{
  ClientBattleTick(DeltaTime);
}

void IBattleActor::BattleActorUpdateVisuals()
{
  UpdateVisuals();
}

void IBattleActor::BattleActorIncreaseHealth(float Amount)
{
  IncreaseHealth(Amount);
}

void IBattleActor::BattleActorDecreaseHealth(float Amount)
{
  DecreaseHealth(Amount);
}
