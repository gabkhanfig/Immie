// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldPlayer.h"
#include <Immie/ImmieCore.h>
#include "../../Game/Player/PlayerImmies.h"
#include <Kismet/KismetMathLibrary.h>
#include <Immie/Game/ImmieGameMode.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AOverworldPlayer::AOverworldPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TimerForBattleReady = 0;
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

	if (TimerForBattleReady > 0) {
		TimerForBattleReady -= DeltaTime;
		if (TimerForBattleReady <= 0) {
			TimerForBattleReady = 0;
			iLog("You are ready to battle again!", LogVerbosity_Display, 15, FLinearColor(0.6, 0.9, 0.2));
		}
	}
}

void AOverworldPlayer::OnCollision(UPrimitiveComponent* ThisOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent)
{
	if (TimerForBattleReady > 0) {
		return;
	}

	const bool ImplementsTrainer = OtherActor->GetClass()->ImplementsInterface(UTrainer::StaticClass());
	if (!ImplementsTrainer) {
		return;
	}

	TScriptInterface<ITrainer> Trainer = OtherActor;
	TrainerCollision(Trainer, ThisOverlappedComponent, OtherActorComponent);
}

void AOverworldPlayer::TrainerCollision_Implementation(const TScriptInterface<ITrainer>& Trainer, UPrimitiveComponent* ThisOverlappedComponent, UPrimitiveComponent* OtherActorComponent)
{
	TArray<FBattleTeamInit> Teams;
	Teams.Add(GetBattleTeamInit());
	Teams.Add(Trainer->GetBattleTeamInit());
	AImmieGameMode* GameMode = Cast<AImmieGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	checkf(IsValid(GameMode), TEXT("Overworld player attempting to start a trainer battle on an invalid ImmieGameMode"));
	GameMode->StartBattle("Singleplayer", Teams, { 0, 0, 0 });
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

void AOverworldPlayer::OnBattleEnd()
{
	TimerForBattleReady = 10;

	ITrainer::OnBattleEnd();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->Possess(this);
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