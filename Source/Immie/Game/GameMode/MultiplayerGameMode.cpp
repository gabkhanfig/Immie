// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include <Immie/Controller/Player/ImmiePlayerController.h>
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Battle/Team/BattleTeam.h>
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

AMultiplayerGameMode::AMultiplayerGameMode()
{
	static ConstructorHelpers::FClassFinder<ABattleTeam> BattleTeamFoundClass(TEXT("/Game/Battle/Team/BP_PlayerMultiplayerBattleTeam"));
	PlayerTeamClass = BattleTeamFoundClass.Class;

	static ConstructorHelpers::FClassFinder<ABattleTeam> AiTeamFoundClass(TEXT("/Game/Battle/Team/BP_TrainerBattleTeam"));
	AiTeamClass = AiTeamFoundClass.Class;
}

void AMultiplayerGameMode::AddPlayerToBattle(AImmiePlayerController* Player, const FString& TeamJsonString)
{
	FMultiplayerTeamContainer Team;
	Team.Controller = Player;
	Team.TeamJsonString = TeamJsonString;
	if (!Team.TeamJsonString.IsEmpty()) {
		PlayerTeams.Add(Team);
	}
	ULogger::Log("Added player team to upcoming battle");
}

bool AMultiplayerGameMode::IsReadyForBattle()
{
	constexpr int RequiredTeamCount = 1;
	return PlayerTeams.Num() >= RequiredTeamCount;
}

void AMultiplayerGameMode::ForceStartMultiplayerBattle(AImmiePlayerController* Player)
{
	checkf(IsValid(PlayerTeamClass), TEXT("Player team class must be valid to force start a multiplayer battle"));

	ULogger::Log("Player has force started a multiplayer battle");

	const int RequiredTeamCount = 2;
	const int TeamCount = PlayerTeams.Num();

	TArray<FBattleTeamInit> BattleTeams;
	BattleTeams.Reserve(RequiredTeamCount);

	Battle = ABattleInstance::NewBattleInstance(this, "Multiplayer", { 0, 0, 0 });
	check(Battle);

	for (int i = 0; i < FMath::Max(RequiredTeamCount, TeamCount); i++) {
		if (!PlayerTeams.IsValidIndex(i)) {
			BattleTeams.Add(GenerateTestAiTeam());
			continue;
		}

		FBattleTeamInit BattleTeam;
		BattleTeam.PlayerController = PlayerTeams[i].Controller;
		BattleTeam.BattleTeamClass = PlayerTeamClass;
		//BattleTeam.TeamType = EBattleTeamType::BattleTeam_PlayerMultiplayer;
		FJsonObjectBP PlayerImmiesJson;
		if (!FJsonObjectBP::LoadJsonString(PlayerTeams[i].TeamJsonString, PlayerImmiesJson)) {
			ULogger::Log("Unable to parse player supplied team string into a json object from player team index " + FString::FromInt(i) + ". Outputting string.", LogVerbosity_Error);
			ULogger::Log(PlayerTeams[i].TeamJsonString, LogVerbosity_Error);
			continue;
		}

		BattleTeam.Team = UImmie::JsonToTeam(PlayerImmiesJson, "PlayerTeam", Battle);
		ULogger::Log("Successfully parsed player's multiplayer team");
		BattleTeams.Add(BattleTeam);
	}

	Battle->BattleInit(BattleTeams);
}

FBattleTeamInit AMultiplayerGameMode::GenerateTestAiTeam() const
{
	TArray<UImmie*> Immies;
	for (int i = 0; i < 2; i++) {
		UImmie* Immie = UImmie::NewImmieObject((UObject*)this, "Snamdon");
		Immie->SetDisplayName("Ai Trainer Immie " + FString::FromInt(i + 1));
		Immie->SetHealth(10000);
		Immies.Add(Immie);
	}

	FBattleTeamInit Team;
	Team.Team = Immies;
	Team.BattleTeamClass = AiTeamClass;
	//Team.TeamType = EBattleTeamType::BattleTeam_PlayerSingleplayer;
	return Team;
}


