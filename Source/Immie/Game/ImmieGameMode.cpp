// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieGameMode.h"
#include <Immie/ImmieCore.h>
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Immies/ImmieObject.h>
#include <Immie/Controller/Player/ImmiePlayerController.h>

AImmieGameMode::AImmieGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = AImmiePlayerController::StaticClass();
}

void AImmieGameMode::BeginPlay()
{
	Super::BeginPlay();
	iLog("immie game mode sayin hello!");
}

void AImmieGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//iLog("game mode ticking");
} 

void AImmieGameMode::StartSingleplayerBattle()
{
	iLog("Gonna start the singleplayer battle");
	ABattleInstance* Battle = ABattleInstance::NewBattleInstance(this, "Singleplayer", { 0, 0, 0 }); 
	TArray<FBattleTeamInit> Teams;
	for (int i = 0; i < 2; i++) {
		FBattleTeamInit Team;
		UImmie* Immie = UImmie::NewImmieObject(this, 0);
		Immie->SetDisplayName("some display name idk");
		Team.Team.Add(Immie);
		Team.TeamType = EBattleTeamType::BattleTeam_PlayerSingleplayer;
		Teams.Add(Team);
	}
	
	Teams[0].Controller = GetWorld()->GetFirstPlayerController();
	Battle->BattleInit(Teams);
}
