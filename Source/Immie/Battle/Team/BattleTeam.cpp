// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTeam.h"
#include <Immie/Immies/ImmieObject.h>
#include <Immie/Immies/ImmieCharacter.h>
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Type/ImmieType.h>
#include <Immie/Ability/Ability.h>
#include <Immie/Controller/Player/ImmiePlayerController.h>
#include <Immie/Ability/AbilityActor.h>
#include <Kismet/GameplayStatics.h>
#include <Immie/Battle/Components/DamageComponent.h>
#include "../../Overworld/Interfaces/Battler.h"

ABattleTeam::ABattleTeam()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	ActiveImmie = nullptr;
	bTeamAlive = true;
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

TArray<AImmieCharacter*> ABattleTeam::AllBattleReadyImmies(bool IncludeActiveImmie) const
{
	TArray<AImmieCharacter*> Immies;
	for (int i = 0; i < Team.Num(); i++) {
		AImmieCharacter* Immie = Team[i];
		if (IncludeActiveImmie) {
			if (Immie == ActiveImmie) continue;
		}

		if (Immie->GetCurrentHealth() > 0) {
			Immies.Add(Immie);
		}
	}
	return Immies;
}

bool ABattleTeam::SwapInNextImmie()
{
	RemoveActiveImmieFromBattle();
	const TArray<AImmieCharacter*> BattleReadyImmies = AllBattleReadyImmies(false);
	if (BattleReadyImmies.Num() == 0) return false;
	SetImmieForBattle(BattleReadyImmies[0]);
	return true;
}

void ABattleTeam::AuthorityBattleTickBattleActors(float DeltaTime)
{
	const bool ActiveImmieAlive = ActiveImmie->GetCurrentHealth() > 0;
	if (ActiveImmieAlive) {
		IBattleActor::Execute_AuthorityBattleTick(ActiveImmie, DeltaTime);
		//ActiveImmie->AuthorityBattleTick(DeltaTime);
	}
	else {
		if (!SwapInNextImmie()) {
			bTeamAlive = false;
		}
	}

	for (int i = 0; i < AbilityActors.Num(); i++) {
		// Should be valid
		AAbilityActor* Actor = AbilityActors[i];
		checkf(IsValid(Actor), TEXT("Ability Actor should be valid for authority battle tick. See if AAbilityActor::Destroy() was called anywhere. Use AAbilityActor::DestroyAbilityActor() instead."));
		IBattleActor::Execute_AuthorityBattleTick(Actor, DeltaTime);
		//Actor->AuthorityBattleTick(DeltaTime);
	}
}

void ABattleTeam::ClientBattleTickBattleActors(float DeltaTime)
{
	if (IsValid(ActiveImmie)) {
		IBattleActor::Execute_ClientBattleTick(ActiveImmie, DeltaTime);
		//ActiveImmie->ClientBattleTick(DeltaTime);
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
	TeamOwner = TeamData.Trainer;
	TeamType = TeamData.TeamType;

	BP_CreateTeam(TeamData);
	BP_InitializeTeam(TeamData);

	if (TeamOwner != nullptr) {
		TeamOwner->OnBattleStart();
	}
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

		TArray<UImmieType*> ImmieTypes = IBattleActor::Execute_GetType(Team[i]);//->GetType();
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
	ImmieCharacter->OnBeginBattle();
}

void ABattleTeam::RemoveActiveImmieFromBattle()
{
	if (ActiveImmie == nullptr) {
		return;
	}

	ActiveImmie->OnRemoveFromBattle();
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
	UDamageComponent* DamageComponent = UDamageComponent::NewDamageComponent(AbilityActor);

	AbilityActor->SetAbility(Ability);
	AbilityActor->SetSpawnedActiveStats(Ability->GetActiveStats());
	AbilityActor->SetDamageComponent(DamageComponent);

	AbilityActors.Add(AbilityActor);

	AbilityActor->InitializeForBattle();
	UGameplayStatics::FinishSpawningActor(AbilityActor, SpawnTransform); // Calls BeginPlay()

	return AbilityActor;
}

void ABattleTeam::RemoveAbilityActor(AAbilityActor* AbilityActor)
{
	AbilityActors.RemoveSingle(AbilityActor);
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

void ABattleTeam::DestroyBattleActors()
{
	RemoveActiveImmieFromBattle();
	for (int i = 0; i < Team.Num(); i++) {
		Team[i]->Destroy();
	}

	for (int i = 0; i < AbilityActors.Num(); i++) {
		AAbilityActor* AbilityActor = AbilityActors[i];
		if (!IsValid(AbilityActor)) {
			continue;
		}
		RemoveAbilityActor(AbilityActor);
	}
}

void ABattleTeam::EventPlayerDealtHealing_Implementation(const TScriptInterface<IBattleActor>& Target, UPARAM(ref) float& Amount, UPARAM(ref)FBattleDamage& Healing, AImmieCharacter* ImmieCharacter)
{
}

void ABattleTeam::EventPlayerDealtDamage_Implementation(const TScriptInterface<IBattleActor>& Target, UPARAM(ref) float& Amount, UPARAM(ref)FBattleDamage& Damage, AImmieCharacter* ImmieCharacter)
{
}

void ABattleTeam::OnBattleEnd_Implementation(EBattleTeamWinState WinState)
{
	if (HasAuthority()) {
		DestroyBattleActors();
		if (TeamOwner != nullptr) { // Don't use IsValid() due to being unable to get the UObject if its null
			TeamOwner->OnBattleEnd();
		}
	}

	AImmiePlayerController* AsPlayerController = Cast<AImmiePlayerController>(Controller);
	const bool IsLocalPlayer = AsPlayerController && AsPlayerController == GetBattleInstance()->GetLocalPlayerController();
	const bool IsLocalPlayerTeam = IsLocalPlayer && !IsRunningDedicatedServer();
	BP_OnBattleEnd(WinState, IsLocalPlayerTeam);	
}
