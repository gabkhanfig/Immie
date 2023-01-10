// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleActor.h"

// Add default functionality here for any IBattleActor functions that are not pure virtual.

bool IBattleActor::IsAlly(const TScriptInterface<IBattleActor>& OtherBattleActor) const
{
  ABattleTeam* Team = IBattleActor::Execute_GetTeam(this->_getUObject());
  return Team != nullptr && Team == IBattleActor::Execute_GetTeam(OtherBattleActor->_getUObject());
}
