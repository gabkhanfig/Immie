// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainerDataObject.h"
#include "../../Immies/ImmieObject.h"

TArray<UImmie*> UTrainerDataObject::GetTeamCopy_Implementation(UObject* Owner)
{
  return UImmie::MakeTeamCopy(Team, Owner);
}

void UTrainerDataObject::LoadTrainerJsonData(const FJsonObjectBP& Json)
{
  Team = UImmie::JsonToTeam(Json, "Team", this);

  Json.TryGetStringField("BattleIntroDialogue", BattleIntroDialogue);
  Json.TryGetStringField("BattleEndWinDialogue", BattleEndWinDialogue);
  Json.TryGetStringField("BattleEndLoseDialogue", BattleEndLoseDialogue);

  BP_LoadTrainerJsonData(Json);
}

const FString& UTrainerDataObject::GetTrainerBlueprintFolder()
{
  static const FString TrainerBlueprintFolder = "/Script/Engine.Blueprint'/Game/Overworld/Trainer/Trainers/";
  return TrainerBlueprintFolder;
}
