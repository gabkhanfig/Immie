// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAiController.h"
#include "../../Immies/ImmieCharacter.h"
#include "../Team/BattleTeam.h"
#include "../BattleInstance/BattleInstance.h"
#include "../../Ability/Ability.h"

ABattleAiController::ABattleAiController() :
	_ImmieCharacter(nullptr)
{
}

void ABattleAiController::Tick(float DeltaTime)
{
	if (!HasAuthority() || !_ImmieCharacter->IsImmieEnabled()) return;
}

void ABattleAiController::Initialize(AImmieCharacter* ImmieCharacter)
{
	_ImmieCharacter = ImmieCharacter;

	const FName EmptyAbilityName = FName("EmptyAbility");

	TArray<UAbility*> Abilities = _ImmieCharacter->GetAbilities();
	for (UAbility* Ability : Abilities) {
		check(IsValid(Ability));
		if (Ability->GetAbilityName() == EmptyAbilityName) continue;
		
		_Abilities.Add(Ability);
	}
}

ABattleTeam* ABattleAiController::GetBattleTeam() const
{
	return IBattleActor::Execute_GetTeam(_ImmieCharacter);
}

ABattleInstance* ABattleAiController::GetBattleInstance() const
{
	return _ImmieCharacter->GetBattleInstance();
}
