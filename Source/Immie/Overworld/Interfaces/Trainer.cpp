// Fill out your copyright notice in the Description page of Project Settings.


#include "Trainer.h"

// Add default functionality here for any ITrainer functions that are not pure virtual.

FBattleTeamInit ITrainer::GetBattleTeamInit() const
{
	AActor* AsActor = Cast<AActor>(this->_getUObject());
	checkf(IsValid(AsActor), TEXT("Casting a ITrainer to AActor must be valid"));

	FBattleTeamInit TeamInit;
	TeamInit.Team = GetTeam();
	TeamInit.SpawnLocation = AsActor->GetActorLocation();
	TeamInit.SpawnRotation = AsActor->GetActorRotation();
	TeamInit.Actor = AsActor;
	TeamInit.TeamType = GetBattleTeamType();
	return TeamInit;
}
