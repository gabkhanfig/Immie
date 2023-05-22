// Fill out your copyright notice in the Description page of Project Settings.


#include "Battler.h"

// Add default functionality here for any ITrainer functions that are not pure virtual.

void IBattler::DisablePawn()
{
	APawn* AsPawn = IBattler::Execute_GetPawn(this->_getUObject());

	AsPawn->SetActorEnableCollision(false);
	AsPawn->SetActorHiddenInGame(true);
	AsPawn->SetActorTickEnabled(false);
}

void IBattler::EnablePawn()
{
	APawn* AsPawn = IBattler::Execute_GetPawn(this->_getUObject());

	AsPawn->SetActorEnableCollision(true);
	AsPawn->SetActorHiddenInGame(false);
	AsPawn->SetActorTickEnabled(true);
}

FBattleTeamInit IBattler::DefaultBattleTeamInit() const
{
	APawn* AsPawn = IBattler::Execute_GetPawn(this->_getUObject());

	FBattleTeamInit TeamInit;
	TeamInit.Team = IBattler::Execute_GetBattlerTeam(this->_getUObject());
	TeamInit.SpawnTransform = AsPawn->GetActorTransform();
	TeamInit.TeamOwner = this->_getUObject();
	TeamInit.BattleTeamClass = IBattler::Execute_GetBattleTeamClass(this->_getUObject());
	return TeamInit;
}
