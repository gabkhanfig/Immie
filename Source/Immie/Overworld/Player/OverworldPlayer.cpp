// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldPlayer.h"
#include <Immie/ImmieCore.h>
#include "../../Game/Player/PlayerImmies.h"
#include <Kismet/KismetMathLibrary.h>
#include <Immie/Game/ImmieGameMode.h>
#include <Kismet/GameplayStatics.h>
#include "../Trainer/TrainerPawn.h"
#include "../../Battle/Team/BattleTeam.h"
#include "../../Immies/ImmieObject.h"
#include "../../Controller/Player/ImmiePlayerController.h"

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

	for (int i = 0; i < NearbyBattleEligibleBattlers.Num(); i++) {
		TScriptInterface<IBattler> Battler = NearbyBattleEligibleBattlers[i];
		if (!IsValid(Battler.GetObject())) {
			NearbyBattleEligibleBattlers.RemoveSingle(Battler);
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

TArray<UImmie*> AOverworldPlayer::GetBattlerTeam_Implementation() const
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
	TeamInit.IsPlayerControlled = true;
	AImmiePlayerController* PlayerController = Cast<AImmiePlayerController>(GetController());
	checkf(IsValid(PlayerController), TEXT("Cannot use an invalid player controller when starting a battle"));
	TeamInit.PlayerController = PlayerController;
	return TeamInit;
}

void AOverworldPlayer::OnBattleStart_Implementation()
{
	DisablePawn();
}

void AOverworldPlayer::OnBattleEnd_Implementation(EBattleTeamWinState WinState)
{
	switch (WinState) {
	case BattleTeamWinState_Win:
		iLog("you won!", LogVerbosity_Display, 7.5, FLinearColor(0.1, 0.65, 0.15));
		break;
	case BattleTeamWinState_Lose:
		iLog("you lost :<", LogVerbosity_Display, 7.5, FLinearColor(1, 0.3, 0.3));
		break;
	case BattleTeamWinState_Draw:
		iLog("you drew", LogVerbosity_Display, 7.5, FLinearColor(0.9, 0.55, 0.2));
		break;
	}

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

TSubclassOf<ABattleTeam> AOverworldPlayer::GetBattleTeamClass_Implementation() const
{
	return BattleTeamClass;
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

void AOverworldPlayer::AddNearbyBattleEligibleBattler(const TScriptInterface<IBattler>& Battler)
{
	NearbyBattleEligibleBattlers.Add(Battler);
}

void AOverworldPlayer::RemoveNearbyBattleEligibleBattler(const TScriptInterface<IBattler>& Battler)
{
	NearbyBattleEligibleBattlers.Add(Battler);
}

void AOverworldPlayer::StartBattleWithFirstNearbyBattleEligibleTrainer()
{
	if (NearbyBattleEligibleBattlers.Num() == 0) return;
	
	TScriptInterface<IBattler> Battler = nullptr;
	for (int i = 0; i < NearbyBattleEligibleBattlers.Num(); i++) {
		if (IsValid(NearbyBattleEligibleBattlers[i].GetObject())) {
			Battler = NearbyBattleEligibleBattlers[i];
			break;
		}
	}
	if (Battler == nullptr) return;

	StartBattleWithBattler(Battler);
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

void AOverworldPlayer::DisplayPlayerTeam()
{
	TArray<UImmie*> Team = GetPlayerImmies()->GetTeam();
	iLog("Player has " + FString::FromInt(Team.Num()) + " Immies");
	for (UImmie* Immie : Team) {
		iLog("Team member: " + Immie->GetSpecieName().ToString());
	}
	iLog("");
}
