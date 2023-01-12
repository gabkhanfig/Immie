// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleActor.h"
#include <Immie/Battle/Team/BattleTeam.h>

// Add default functionality here for any IBattleActor functions that are not pure virtual.

UDamageComponent* IBattleActor::BattleActorGetDamageComponent() const
{
  return GetDamageComponent();
}

UDamageComponent* IBattleActor::GetDamageComponent() const
{
  return nullptr;
}

bool IBattleActor::BattleActorIsValidAbilityCollider(UPrimitiveComponent* Collider) const
{
  return IsValidAbilityCollider(Collider);
}

bool IBattleActor::IsValidAbilityCollider(UPrimitiveComponent* Collider) const
{
  return false;
}

float IBattleActor::BattleActorTotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const
{
  return TotalHealingFromAbility(AbilityHealing);
}

float IBattleActor::TotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const
{
  return 0.0f;
}

float IBattleActor::BattleActorTotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const
{
  return TotalDamageFromAbility(AbilityDamage);
}

float IBattleActor::TotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const
{
  return 0.0f;
}

bool IBattleActor::BattleActorCanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const
{
  return CanBeHealedByAbilityActor(AbilityActor);
}

bool IBattleActor::CanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const
{
  return false;
}

bool IBattleActor::BattleActorCanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const
{
  return CanBeDamagedByAbilityActor(AbilityActor);
}

bool IBattleActor::CanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const
{
  return false;
}

ABattleTeam* IBattleActor::BattleActorGetTeam() const
{
  return GetTeam();
}

ABattleTeam* IBattleActor::GetTeam() const
{
  return nullptr;
}

bool IBattleActor::IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const
{
  ABattleTeam* Team = GetTeam();
  if (!IsValid(Team)) return false;
  return Team != nullptr && Team == OtherBattleActor->GetTeam();
}

FBattleStats IBattleActor::BattleActorGetActiveStats() const
{
  return GetActiveStats();
}

FBattleStats IBattleActor::GetActiveStats() const
{
  return FBattleStats();
}

void IBattleActor::BattleActorAuthorityBattleTick(float DeltaTime)
{
  AuthorityBattleTick(DeltaTime);
}

void IBattleActor::AuthorityBattleTick(float DeltaTime)
{
}

void IBattleActor::BattleActorClientBattleTick(float DeltaTime)
{
  ClientBattleTick(DeltaTime);
}

void IBattleActor::ClientBattleTick(float DeltaTime)
{
}

void IBattleActor::BattleActorUpdateVisuals()
{
  UpdateVisuals();
}

void IBattleActor::UpdateVisuals()
{
}

void IBattleActor::BattleActorIncreaseHealth(float Amount)
{
  IncreaseHealth(Amount);
}

void IBattleActor::IncreaseHealth(float Amount)
{
}

void IBattleActor::BattleActorDecreaseHealth(float Amount)
{
  DecreaseHealth(Amount);
}

void IBattleActor::DecreaseHealth(float Amount)
{
}
