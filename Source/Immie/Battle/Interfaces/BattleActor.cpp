// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleActor.h"
#include <Immie/Battle/Team/BattleTeam.h>
#include <Immie/ImmieCore.h>

// Add default functionality here for any IBattleActor functions that are not pure virtual.

//UDamageComponent* IBattleActor::BattleActorGetDamageComponent_Implmementation() const
//{
//  return GetDamageComponent();
//}
//
//bool IBattleActor::BattleActorIsValidAbilityCollider_Implmementation(UPrimitiveComponent* Collider) const
//{
//  return IsValidAbilityCollider(Collider);
//}
//
//float IBattleActor::BattleActorTotalHealingFromAbility_Implmementation(const FAbilityInstigatorDamage& AbilityHealing) const
//{
//  return TotalHealingFromAbility(AbilityHealing);
//}
//
//float IBattleActor::BattleActorTotalDamageFromAbility_Implmementation(const FAbilityInstigatorDamage& AbilityDamage) const
//{
//  return TotalDamageFromAbility(AbilityDamage);
//}
//
//bool IBattleActor::BattleActorCanBeHealedByAbilityActor_Implmementation(AAbilityActor* AbilityActor) const
//{
//  return CanBeHealedByAbilityActor(AbilityActor);
//}
//
//bool IBattleActor::BattleActorCanBeDamagedByAbilityActor_Implmementation(AAbilityActor* AbilityActor) const
//{
//  return CanBeDamagedByAbilityActor(AbilityActor);
//}
//
//ABattleTeam* IBattleActor::BattleActorGetTeam_Implmementation() const
//{
//  return GetTeam();
//}
//
//TArray<UImmieType*> IBattleActor::BattleActorGetType_Implmementation() const
//{
//  return GetType();
//}
//
bool IBattleActor::IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const
{
  ABattleTeam* Team = GetTeam();
  if (!IsValid(Team)) return false;
  return Team != nullptr && Team == OtherBattleActor->GetTeam();
}
//
//FBattleStats IBattleActor::BattleActorGetInitialStats_Implmementation() const
//{
//  return GetInitialStats();
//}
//
//FBattleStats IBattleActor::BattleActorGetActiveStats_Implmementation() const
//{
//  return GetActiveStats();
//}
//
//FString IBattleActor::BattleActorGetDisplayName_Implmementation() const
//{
//  return GetDisplayName();
//}
//
//void IBattleActor::BattleActorAuthorityBattleTick_Implmementation(float DeltaTime)
//{
//  AuthorityBattleTick(DeltaTime);
//}
//
//void IBattleActor::BattleActorClientBattleTick_Implmementation(float DeltaTime)
//{
//  ClientBattleTick(DeltaTime);
//}
//
//void IBattleActor::BattleActorUpdateVisuals_Implmementation()
//{
//  UpdateVisuals();
//}
//
//void IBattleActor::BattleActorIncreaseHealth_Implmementation(float Amount)
//{
//  IncreaseHealth(Amount);
//}
//
//void IBattleActor::BattleActorDecreaseHealth_Implmementation(float Amount)
//{
//  DecreaseHealth(Amount);
//}
