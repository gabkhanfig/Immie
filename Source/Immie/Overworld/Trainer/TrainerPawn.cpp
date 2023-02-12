// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainerPawn.h"
#include "../../Immies/ImmieObject.h"

// Sets default values
ATrainerPawn::ATrainerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BattleTeamType = EBattleTeamType::BattleTeam_Trainer;
}

// Called when the game starts or when spawned
void ATrainerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	UImmie* Immie = UImmie::NewImmieObject(this, 0);
	Immie->SetDisplayName("enemy!!! wowow");
	Immie->SetHealth(10000);
	Team.Add(Immie);

}

// Called every frame
void ATrainerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrainerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

EBattleTeamType ATrainerPawn::GetBattleTeamType() const
{
	return BattleTeamType;
}

TArray<UImmie*> ATrainerPawn::GetTeam() const
{
	return Team;
}

