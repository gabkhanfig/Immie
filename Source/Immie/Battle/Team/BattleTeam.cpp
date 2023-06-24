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
#include "../../Controller/Ai/ImmieAIController.h"
#include "../../Overworld/WildImmies/WildImmieSpawner.h"
#include "../../Game/Player/PlayerImmies.h"
#include <Immie/Type/BattleTypeComponent.h>

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
	bUseOuterImmieCharacters = true;
	bDestroyAllCharactersOnBattleEnd = true;
}

void ABattleTeam::BeginPlay()
{
	Super::BeginPlay();
}

AImmieCharacter* ABattleTeam::MakeBattleImmie(UImmie* ImmieObject)
{
	AImmieCharacter* ObjectOuterAsImmie = Cast<AImmieCharacter>(ImmieObject->GetOuter());
	// If the Immie Object's that's not an immie character, make a new battle immie character,
	// OR if this battle team is flagged to not use an outer immie character
	if (!bUseOuterImmieCharacters || ObjectOuterAsImmie == nullptr) {
		return AImmieCharacter::SpawnBattleImmieCharacter(this, ImmieSpawnTransform, ImmieObject);
	}
	// Otherwise, use the existing character.
	ObjectOuterAsImmie->MakeBattle(this);
	return ObjectOuterAsImmie;
}

void ABattleTeam::CreateTeamFromImmies(const TArray<UImmie*>& TeamImmies)
{
	for (int i = 0; i < TeamImmies.Num(); i++) {
		AImmieCharacter* ImmieCharacter = MakeBattleImmie(TeamImmies[i]);
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
	if (ActiveImmie->IsAlive()) {
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
	ImmieSpawnTransform = TeamData.SpawnTransform;
	Controller = TeamData.Controller;
	TeamOwnerAsObject = TeamData.TeamOwner;

	if (Controller == nullptr) {
		if (IsValid(AIControllerClass)) {
			//iLog("null controller but valid ai controller class");
		}
		else {
			//iLog("null controller");
		}
	}

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

	for (AImmieCharacter* ImmieCharacter : Team) {
		if (!IsValid(ImmieCharacter)) {
			iLog("valid subobjects? invalid team character");
			return false;
		}

		if (!IsValid(ImmieCharacter->GetImmieObject())) {
			iLog("valid subobjects? invalid immie object");
			return false;
		}

		if (!IsValid(IBattleActor::Execute_GetTypeComponent(ImmieCharacter))) {
			iLog("valid subobjects? invalid immie object");
			return false;
		}

		TArray<UAbility*> ImmieAbilities = ImmieCharacter->GetAbilities();
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
	AActor* _Owner = Ability->GetImmieCharacter(); //this
	AAbilityActor* AbilityActor = Owner->GetWorld()->SpawnActorDeferred<AAbilityActor>
		(AbilityActorClass, SpawnTransform, _Owner, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

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

void ABattleTeam::DestroyAllAbilityActors()
{
	for (int i = 0; i < AbilityActors.Num(); i++) {
		AAbilityActor* AbilityActor = AbilityActors[i];
		if (!IsValid(AbilityActor)) {
			continue;
		}
		RemoveAbilityActor(AbilityActor);
		i--;
	}
}

void ABattleTeam::DestroyAllImmies()
{
	for (int i = 0; i < Team.Num(); i++) {
		Team[i]->Destroy();
	}
	Team.Empty();
}

void ABattleTeam::CaptureOrReturnWildImmieToSpawner(AImmieCharacter* ImmieCharacter, AWildImmieSpawner* Spawner, bool Heal)
{
	if (ImmieCharacter->IsAlive()) {
		ImmieCharacter->MakeWild(Spawner);
	}
	else {
		GetPlayerImmies()->CaptureWildImmie(ImmieCharacter->GetImmieObject(), Heal);
		checkf(ImmieCharacter->GetImmieObject()->GetOuter() != ImmieCharacter, TEXT("Captured Immie Object's owner is still an Immie Character. Cannot destroy character"));
		ImmieCharacter->Destroy();
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
		DestroyAllAbilityActors();
		RemoveActiveImmieFromBattle();
		if (bDestroyAllCharactersOnBattleEnd) {
			DestroyAllImmies();
		}
		//DestroyBattleActors();
	}

	AImmiePlayerController* AsPlayerController = Cast<AImmiePlayerController>(Controller);
	const bool IsLocalPlayer = AsPlayerController && AsPlayerController == GetBattleInstance()->GetLocalPlayerController();
	const bool IsLocalPlayerTeam = IsLocalPlayer && !IsRunningDedicatedServer();
	BP_OnBattleEnd(WinState, IsLocalPlayerTeam);	
}
