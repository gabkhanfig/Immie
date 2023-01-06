// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTeam.h"
#include <Immie/Immies/ImmieObject.h>
#include <Immie/Immies/ImmieCharacter.h>
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Type/ImmieType.h>
#include <Immie/Ability/Abilities/Ability.h>
#include <Immie/Controller/Player/ImmiePlayerController.h>
#include <Immie/Ability/Actor/AbilityActor.h>
#include <Kismet/GameplayStatics.h>

ABattleTeam::ABattleTeam()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;

	ActiveImmie = nullptr;
}

void ABattleTeam::BeginPlay()
{
	Super::BeginPlay();
}

void ABattleTeam::CreateTeamFromImmies(const TArray<UImmie*>& TeamImmies)
{
	FTransform SpawnTransform = FTransform(DefaultSpawnRotation, DefaultSpawnLocation, { 1, 1, 1 });
	for (int i = 0; i < TeamImmies.Num(); i++) {
		AImmieCharacter* ImmieCharacter = AImmieCharacter::NewImmieCharacter(this, SpawnTransform, TeamImmies[i], false);
		ImmieCharacter->InitializeForBattle(this, i);
		Team.Add(ImmieCharacter);
	}
}

void ABattleTeam::AuthorityBattleTickBattleActors(float DeltaTime)
{
	IBattleActor::Execute_AuthorityBattleTick(ActiveImmie, DeltaTime);

	for (int i = 0; i < AbilityActors.Num(); i++) {
		// Should be valid
		AAbilityActor* Actor = AbilityActors[i];
		checkf(IsValid(Actor), TEXT("Ability Actor should be valid for authority battle tick. See if AAbilityActor::Destroy() was called anywhere. Use AAbilityActor::DestroyAbilityActor() instead."));
		IBattleActor::Execute_AuthorityBattleTick(Actor, DeltaTime);

	}
}

void ABattleTeam::ClientBattleTickBattleActors(float DeltaTime)
{
	if (IsValid(ActiveImmie)) {
		IBattleActor::Execute_ClientBattleTick(ActiveImmie, DeltaTime);
	}
	
	// Client ability actors do not execute client battle tick. DummyAbilityActor::Tick() should handle any client relevant ticking.
}

void ABattleTeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABattleTeam::InitializeTeam(ABattleInstance* _BattleInstance, const FBattleTeamInit& TeamData)
{
	BattleInstance = _BattleInstance;
	DefaultSpawnLocation = TeamData.SpawnLocation;
	DefaultSpawnRotation = TeamData.SpawnRotation;
	Controller = TeamData.Controller;
	TeamOwner = TeamData.Actor;
	TeamType = TeamData.TeamType;

	BP_CreateTeam(TeamData);
	BP_InitializeTeam(TeamData);
}

void ABattleTeam::BP_CreateTeam_Implementation(const FBattleTeamInit& TeamData)
{
	CreateTeamFromImmies(TeamData.Team);
}

void ABattleTeam::SetClientSubobjects()
{
	SyncClientSubobjects(BattleInstance, Controller, Team);
	for (int i = 0; i < Team.Num(); i++) {
		iLog("setting client subobjects for team immie index " + FString::FromInt(i));
		Team[i]->SyncClientSubobjects();
	}
}

void ABattleTeam::SyncClientSubobjects_Implementation(ABattleInstance* BattleInstanceObject, AController* ControllerObject, const TArray<AImmieCharacter*>& TeamCharacterObjects)
{
	if (HasAuthority()) {
		return;
	}

	BattleInstance = BattleInstanceObject;
	if (ControllerObject == Cast<AController>(BattleInstance->GetLocalPlayerController())) {
		Controller = ControllerObject;
	}
	else {
		Controller = nullptr;
	}
	Team = TeamCharacterObjects;
}

bool ABattleTeam::ValidSubobjects()
{
	if (Team.Num() == 0) {
		iLog("team has no immie characters");
		return false;
	}
	if (!IsValid(BattleInstance)) {
		iLog("team has an invalid battle instance");
	}

	for (int i = 0; i < Team.Num(); i++) {
		if (!IsValid(Team[i])) {
			iLog("valid subobjects? invalid team character");
			return false;
		}

		if (!IsValid(Team[i]->GetImmieObject())) {
			iLog("valid subobjects? invalid immie object");
			return false;
		}

		TArray<UImmieType*> ImmieTypes = Team[i]->GetType();
		for (const auto& Type : ImmieTypes) {
			if (!IsValid(Type)) {
				iLog("valid subobjects? invalid type");
				return false;
			}
		}

		TArray<UAbility*> ImmieAbilities = Team[i]->GetAbilities();
		for (const auto& Ability : ImmieAbilities) {
			if (!IsValid(Ability)) {
				iLog("valid subobjects? invalid ability");
				return false;
			}
		}
	}

	return true;
}

void ABattleTeam::SyncClientTeam()
{
	for (int i = 0; i < Team.Num(); i++) {
		Team[i]->SyncClientBattleData();
	}

	BP_SyncClientTeam();
}

void ABattleTeam::SetImmieForBattle(AImmieCharacter* ImmieCharacter)
{
	RemoveActiveImmieFromBattle();

	ImmieCharacter->SetImmieEnabled(true);

	if (IsValid(Controller)) {
		ImmieCharacter->PossessForBattle(Controller);
	}


	SetActiveImmie(ImmieCharacter);
	iLog("Immie is now active " + ActiveImmie->GetFName().ToString());
}

void ABattleTeam::RemoveActiveImmieFromBattle()
{
	if (ActiveImmie == nullptr) {
		return;
	}

	ActiveImmie->UnPossessForBattle();
	ActiveImmie->SetImmieEnabled(false);
	SetActiveImmie(nullptr);
}

void ABattleTeam::SetActiveImmie_Implementation(AImmieCharacter* NewActiveImmie)
{
	ActiveImmie = NewActiveImmie;
}

AAbilityActor* ABattleTeam::SpawnAbilityActor(TSubclassOf<AAbilityActor> AbilityActorClass, UAbility* Ability, const FTransform& SpawnTransform)
{
	check(IsValid(AbilityActorClass));
	AAbilityActor* AbilityActor = Owner->GetWorld()->SpawnActorDeferred<AAbilityActor>
		(AbilityActorClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	check(IsValid(AbilityActor));
	AbilityActor->SetAbility(Ability);
	AbilityActor->SetSpawnedActiveStats(Ability->GetActiveStats());
	AbilityActors.Add(AbilityActor);

	UGameplayStatics::FinishSpawningActor(AbilityActor, SpawnTransform);
	AbilityActor->InitializeForBattle();
	return AbilityActor;
}

void ABattleTeam::RemoveAbilityActor(AAbilityActor* AbilityActor)
{
	AbilityActors.RemoveSingleSwap(AbilityActor);
	AbilityActor->OnAbilityActorDestroy();
	AbilityActor->Destroy();
}

void ABattleTeam::AuthorityBattleTick_Implementation(float DeltaTime)
{
	AuthorityBattleTickBattleActors(DeltaTime);
}

void ABattleTeam::ClientBattleTick_Implementation(float DeltaTime)
{
	ClientBattleTickBattleActors(DeltaTime);
}

EBattleTeamType ABattleTeam::GetBattleTeamType()
{
	return TeamType;
}

