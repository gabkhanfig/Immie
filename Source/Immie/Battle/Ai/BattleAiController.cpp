// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAiController.h"
#include "../../Immies/ImmieCharacter.h"
#include "../Team/BattleTeam.h"
#include "../BattleInstance/BattleInstance.h"

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
}

ABattleTeam* ABattleAiController::GetBattleTeam() const
{
	return IBattleActor::Execute_GetTeam(_ImmieCharacter);
}

ABattleInstance* ABattleAiController::GetBattleInstance() const
{
	return _ImmieCharacter->GetBattleInstance();
}
