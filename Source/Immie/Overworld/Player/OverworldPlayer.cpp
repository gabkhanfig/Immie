// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldPlayer.h"
#include <Immie/ImmieCore.h>
#include "../../Game/Player/PlayerImmies.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AOverworldPlayer::AOverworldPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOverworldPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOverworldPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOverworldPlayer::OnCollision(UPrimitiveComponent* ThisOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent)
{
	const bool ImplementsTrainer = OtherActor->GetClass()->ImplementsInterface(UTrainer::StaticClass());
	if (!ImplementsTrainer) {
		return;
	}

	TScriptInterface<ITrainer> Trainer = OtherActor;
	TrainerCollision(Trainer, ThisOverlappedComponent, OtherActorComponent);
}

void AOverworldPlayer::TrainerCollision_Implementation(const TScriptInterface<ITrainer>& Trainer, UPrimitiveComponent* ThisOverlappedComponent, UPrimitiveComponent* OtherActorComponent)
{
	iLog("trainer collision!");
}

// Called to bind functionality to input
void AOverworldPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Rotation. */
	PlayerInputComponent->BindAxis("Turn", this, &AOverworldPlayer::YawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AOverworldPlayer::PitchInput);

	/* WASD. */
	PlayerInputComponent->BindAxis("MoveForward", this, &AOverworldPlayer::ForwardMovement);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOverworldPlayer::RightMovement);

	/* Jump. */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AOverworldPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AOverworldPlayer::StopJumping);
}

EBattleTeamType AOverworldPlayer::GetBattleTeamType() const
{
	return EBattleTeamType::BattleTeam_PlayerSingleplayer;
}

TArray<UImmie*> AOverworldPlayer::GetTeam() const
{
	return GetPlayerImmies()->GetTeam();
}

FBattleTeamInit AOverworldPlayer::GetBattleTeamInit() const
{
	FBattleTeamInit TeamInit = ITrainer::GetBattleTeamInit();
	TeamInit.Controller = GetController();
	return TeamInit;
}

void AOverworldPlayer::YawInput(float AxisValue)
{
	AddControllerYawInput(AxisValue * 0.5);
}

void AOverworldPlayer::PitchInput(float AxisValue)
{
	AddControllerPitchInput(AxisValue * 0.5);
}

void AOverworldPlayer::ForwardMovement(float ScaleValue)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), ScaleValue, false);
}

void AOverworldPlayer::RightMovement(float ScaleValue)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), ScaleValue, false);
}