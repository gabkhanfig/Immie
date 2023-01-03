// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include <Immie/Controller/Player/ImmiePlayerController.h>
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include <Immie/ImmieCore.h>
#include <Immie/Immies/ImmieObject.h>
#include <Immie/Game/Global/Managers/SpecieDataManager.h>

void AMultiplayerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AImmiePlayerController* Player = Cast<AImmiePlayerController>(NewPlayer);
	Player->SetGameMode(this);
	Player->RequestClientPlayerTeam();
}

void AMultiplayerGameMode::AddPlayerToBattle(AImmiePlayerController* Player, const FString& TeamJsonString)
{
	FMultiplayerTeamContainer Team;
	Team.Controller = Player;
	Team.TeamJsonString = TeamJsonString;
	if (!Team.TeamJsonString.IsEmpty()) {
		PlayerTeams.Add(Team);
	}

	if (IsReadyForBattle()) {
		StartMultiplayerBattle();
	}
}

bool AMultiplayerGameMode::IsReadyForBattle()
{
	constexpr int RequiredTeamCount = 1;
	return PlayerTeams.Num() >= RequiredTeamCount;
}

void AMultiplayerGameMode::StartMultiplayerBattle()
{
	const int TeamCount = PlayerTeams.Num();

	Battle = ABattleInstance::NewBattleInstance(this, "Multiplayer", { 0, 0, 0 });
	check(IsValid(Battle));

	TArray<FBattleTeamInit> BattleTeams;
	BattleTeams.Reserve(TeamCount);
	for (int i = 0; i < TeamCount; i++) {
		FBattleTeamInit BattleTeam;
		BattleTeam.Controller = PlayerTeams[i].Controller;
		BattleTeam.TeamType = EBattleTeamType::BattleTeam_PlayerMultiplayer;
		FJsonObjectBP PlayerImmiesJson;
		if (!FJsonObjectBP::LoadJsonString(PlayerTeams[i].TeamJsonString, PlayerImmiesJson)) {
			ULogger::Log("Unable to parse player supplied team string into a json object from player team index " + FString::FromInt(i) + ". Outputting string.", LogVerbosity_Error);
			ULogger::Log(PlayerTeams[i].TeamJsonString, LogVerbosity_Error);
			continue;
		}
		
		BattleTeam.Team = UImmie::JsonToTeam(PlayerImmiesJson, "PlayerTeam", Battle);
		ULogger::Log("Successfully parsed team");
		BattleTeams.Add(BattleTeam);
	}

	Battle->BattleInit(BattleTeams);
}


