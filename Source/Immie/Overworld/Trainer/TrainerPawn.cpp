// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainerPawn.h"
#include "../../Immies/ImmieObject.h"
#include "TrainerDataObject.h"
#include "../../Game/Global/Managers/TrainerDataManager.h"

// Sets default values
ATrainerPawn::ATrainerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BattleTeamType = EBattleTeamType::BattleTeam_Trainer;
	CanOverworldPlayerInitiateBattle = true;
}

// Called when the game starts or when spawned
void ATrainerPawn::BeginPlay()
{
	Super::BeginPlay();

	ConstructTeam();
}

UTrainerDataObject* ATrainerPawn::GetTrainerDataObject()
{
	return GetTrainerDataManager()->GetTrainerDataObject(TrainerName);
}

void ATrainerPawn::ConstructTeam()
{
	UTrainerDataObject* DataObject = GetTrainerDataObject();
	if (!IsValid(DataObject)) {
		iLog("Unable to get trainer data object for trainer " + TrainerName.ToString(), LogVerbosity_Error);
		return;
	}

	Team = DataObject->GetTeamCopy(this);
}

// Called every frame
void ATrainerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EBattleTeamType ATrainerPawn::GetBattleTeamType_Implementation() const
{
	return BattleTeamType;
}

TArray<UImmie*> ATrainerPawn::GetTeam_Implementation() const
{
	return Team;
}

APawn* ATrainerPawn::GetPawn_Implementation() const
{
	return (APawn*)this;
}

FBattleTeamInit ATrainerPawn::GetBattleTeamInit_Implementation() const
{
	return DefaultBattleTeamInit();
}

void ATrainerPawn::OnBattleStart_Implementation()
{
	DisablePawn();
	iLog(GetTrainerDataObject()->GetBattleIntroDialogue());
}

void ATrainerPawn::OnBattleEnd_Implementation(EBattleTeamWinState WinState)
{
	EnablePawn();
	switch (WinState) {
	case BattleTeamWinState_Win:
		iLog(GetTrainerDataObject()->GetBattleEndWinDialogue());
		break;
	case BattleTeamWinState_Lose:
		iLog(GetTrainerDataObject()->GetBattleEndLoseDialogue());
		break;
	case BattleTeamWinState_Draw:
		iLog("Trainer Draw Example Dialogue");
		break;
	}
}

bool ATrainerPawn::CanBeBattled_Implementation() const
{
	return true;
}

