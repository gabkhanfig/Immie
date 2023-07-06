// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleStructures.h"

FBattleTeamInit::FBattleTeamInit()
{
	//Team.Reserve(8);
	IsPlayerControlled = false;
	PlayerController = nullptr;
	TeamOwner = nullptr;
}
