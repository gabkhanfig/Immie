// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmiePlayerController.h"
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/ImmieCore.h>
#include <Immie/Game/Player/PlayerImmies.h>
#include <Immie/Immies/ImmieObject.h>
#include <Immie/Game/GameMode/MultiplayerGameMode.h>
#include <Kismet/GameplayStatics.h>

void AImmiePlayerController::SetClientBattleTeamIndex_Implementation(ABattleInstance* BattleInstance, int NewClientTeam)
{
	BattleInstance->SetClientTeamIndex(NewClientTeam);
}

void AImmiePlayerController::RequestClientPlayerTeam_Implementation()
{
	FJsonObjectBP TeamJson = UImmie::TeamToJson(GetPlayerImmies()->GetTeam(), "PlayerTeam");
	SendPlayerTeamToServer(TeamJson.ToString());
}

void AImmiePlayerController::SendPlayerTeamToServer_Implementation(const FString& TeamJsonString)
{
	AMultiplayerGameMode* MultiplayerGameMode =	Cast<AMultiplayerGameMode>(GameMode);
	MultiplayerGameMode->AddPlayerToBattle(this, TeamJsonString);
}

void AImmiePlayerController::InformBattleInstanceClientSpawned_Implementation(ABattleInstance* BattleInstance)
{
	BattleInstance->ClientSpawned(this);
}

void AImmiePlayerController::InformBattleInstanceClientValidManagers_Implementation(ABattleInstance* BattleInstance)
{
	BattleInstance->ClientManagersValid(this);
}

void AImmiePlayerController::InformBattleInstanceClientValidTeams_Implementation(ABattleInstance* BattleInstance)
{
	BattleInstance->ClientTeamsValid(this);
}

void AImmiePlayerController::StartMultiplayerBattle()
{
	iLog("You have decided to force start a multiplayer battle", LogVerbosity_Warning);
	ExecuteStartMultiplayerBattle();
}

void AImmiePlayerController::ExecuteStartMultiplayerBattle_Implementation()
{
	AMultiplayerGameMode* MultiplayerGameMode = Cast<AMultiplayerGameMode>(GameMode);
	MultiplayerGameMode->ForceStartMultiplayerBattle(this);
}
