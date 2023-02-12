// Fill out your copyright notice in the Description page of Project Settings.


#include "Battler.h"

// Add default functionality here for any ITrainer functions that are not pure virtual.

APawn* IBattler::GetPawn() const
{
	APawn* AsPawn = Cast<APawn>(this->_getUObject());
	checkf(IsValid(AsPawn), TEXT("Casting a ITrainer to APawn must be valid"));
	return AsPawn;
}

FBattleTeamInit IBattler::GetBattleTeamInit() const
{
	APawn* AsPawn = GetPawn();

	FBattleTeamInit TeamInit;
	TeamInit.Team = GetTeam();
	TeamInit.SpawnLocation = AsPawn->GetActorLocation();
	TeamInit.SpawnRotation = AsPawn->GetActorRotation();
	TeamInit.Trainer = this->_getUObject();
	TeamInit.TeamType = GetBattleTeamType();
	return TeamInit;
}

void IBattler::Disable()
{
	APawn* AsPawn = GetPawn();

	AsPawn->SetActorEnableCollision(false);
	AsPawn->SetActorHiddenInGame(true);
	AsPawn->SetActorTickEnabled(false);
}

void IBattler::Enable()
{
	APawn* AsPawn = GetPawn();

	AsPawn->SetActorEnableCollision(true);
	AsPawn->SetActorHiddenInGame(false);
	AsPawn->SetActorTickEnabled(true);
}

void IBattler::OnBattleStart()
{
	Disable();
}

void IBattler::OnBattleEnd()
{
	Enable();
}
