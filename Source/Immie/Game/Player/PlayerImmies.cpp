// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerImmies.h"
#include "../../Immies/ImmieObject.h"

#define PLAYER_MAX_TEAM_SIZE 8

UPlayerImmies::UPlayerImmies()
{
	Team.Reserve(PLAYER_MAX_TEAM_SIZE);
}

bool UPlayerImmies::DoesPlayerTeamHaveSpace()
{
	return Team.Num() < PLAYER_MAX_TEAM_SIZE;
}

void UPlayerImmies::CaptureWildImmie(UImmie* ImmieObject, bool Heal)
{
	ImmieObject->ChangeOuter(this);
	if (!AddToTeam(ImmieObject, Heal)) {
		AddToChester(ImmieObject, Heal);
	}
}

bool UPlayerImmies::AddToTeam(UImmie* ImmieObject, bool Heal)
{
	if (DoesPlayerTeamHaveSpace()) {
		Team.Add(ImmieObject);
		return true;
	}
	return false;
}

void UPlayerImmies::AddToChester(UImmie* ImmieObject, bool Heal)
{
	Chester.Add(ImmieObject);
}

void UPlayerImmies::AddImmie(UImmie* ImmieObject, bool Heal)
{
	if (!AddToTeam(ImmieObject, Heal)) {
		AddToChester(ImmieObject, Heal);
	}
}
