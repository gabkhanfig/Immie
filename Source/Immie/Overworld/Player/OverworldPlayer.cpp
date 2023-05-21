// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldPlayer.h"
#include <Immie/ImmieCore.h>
#include "../../Game/Player/PlayerImmies.h"
#include <Kismet/KismetMathLibrary.h>
#include <Immie/Game/ImmieGameMode.h>
#include <Kismet/GameplayStatics.h>
#include "../Trainer/TrainerPawn.h"
#include "../../Battle/Team/BattleTeam.h"

// Sets default values
AOverworldPlayer::AOverworldPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TimerForBattleReady = 0; /// Script / Engine.Blueprint'/Game/Battle/Team/BP_PlayerSingleplayerBattleTeam.BP_PlayerSingleplayerBattleTeam'

	static ConstructorHelpers::FClassFinder<ABattleTeam> BattleTeamFoundClass(TEXT("/Game/Battle/Team/BP_PlayerSingleplayerBattleTeam"));
	BattleTeamClass = BattleTeamFoundClass.Class;
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

	for (int i = 0; i < NearbyBattleEligibleTrainers.Num(); i++) {
		ATrainerPawn* Trainer = NearbyBattleEligibleTrainers[i];
		if (!IsValid(Trainer)) {
			NearbyBattleEligibleTrainers.RemoveSingle(Trainer);
			i--;
		}
	}

	if (TimerForBattleReady > 0) {
		TimerForBattleReady -= DeltaTime;
		if (TimerForBattleReady <= 0) {
			TimerForBattleReady = 0;
			iLog("You are ready to battle again!", LogVerbosity_Display, 15, FLinearColor(0.6, 0.9, 0.2));
		}
	}
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

EBattleTeamType AOverworldPlayer::GetBattleTeamType_Implementation() const
{
	return EBattleTeamType::BattleTeam_PlayerSingleplayer;
}

TArray<UImmie*> AOverworldPlayer::GetTeam_Implementation() const
{
	return GetPlayerImmies()->GetTeam();
}

APawn* AOverworldPlayer::GetPawn_Implementation() const
{
	return (APawn*)this;
}

FBattleTeamInit AOverworldPlayer::GetBattleTeamInit_Implementation() const
{
	FBattleTeamInit TeamInit = DefaultBattleTeamInit();
	TeamInit.Controller = GetController();
	TeamInit.BattleTeamClass = BattleTeamClass;
	return TeamInit;
}

void AOverworldPlayer::OnBattleStart_Implementation()
{
	DisablePawn();
}

void AOverworldPlayer::OnBattleEnd_Implementation(EBattleTeamWinState WinState)
{
	EnablePawn();
	TimerForBattleReady = 10;

	//IBattler::OnBattleEnd();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->Possess(this);
}

bool AOverworldPlayer::CanBeBattled_Implementation() const
{
	return true;
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

void AOverworldPlayer::AddNearbyBattleEligibleTrainer(ATrainerPawn* Trainer)
{
	NearbyBattleEligibleTrainers.Add(Trainer);
}

void AOverworldPlayer::RemoveNearbyBattleEligibleTrainer(ATrainerPawn* Trainer)
{
	NearbyBattleEligibleTrainers.RemoveSingle(Trainer);
}

void AOverworldPlayer::StartBattleWithFirstNearbyBattleEligibleTrainer()
{
	if (NearbyBattleEligibleTrainers.Num() == 0) return;
	
	ATrainerPawn* Trainer = nullptr;
	for (int i = 0; i < NearbyBattleEligibleTrainers.Num(); i++) {
		if (IsValid(NearbyBattleEligibleTrainers[i])) {
			Trainer = NearbyBattleEligibleTrainers[i];
			break;
		}
	}
	if (Trainer == nullptr) return;

	StartBattleWithBattler(Trainer);
}

void AOverworldPlayer::StartBattleWithBattler(const TScriptInterface<IBattler>& Battler)
{
	TArray<FBattleTeamInit> Teams;
	Teams.Add(IBattler::Execute_GetBattleTeamInit(this));
	Teams.Add(IBattler::Execute_GetBattleTeamInit(Battler->_getUObject()));
	AImmieGameMode* GameMode = Cast<AImmieGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	checkf(IsValid(GameMode), TEXT("Overworld player attempting to start a trainer battle on an invalid ImmieGameMode"));
	GameMode->StartBattle("Singleplayer", Teams, { 0, 0, 0 });
}
