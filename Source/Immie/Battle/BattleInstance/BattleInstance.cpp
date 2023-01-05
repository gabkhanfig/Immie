// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleInstance.h"
#include <Immie/Battle/Managers/BattleTypeManager.h>
#include <Immie/Battle/Managers/BattleAbilityManager.h>
#include <Immie/Battle/Managers/BattleSpecieManager.h>
#include <Immie/Game/Global/Managers/BattleDataManager.h>
#include <Immie/Battle/Team/BattleTeam.h>
#include <Kismet/GameplayStatics.h>
#include <Immie/Game/ImmieGameMode.h>
#include <Immie/Controller/Player/ImmiePlayerController.h>

ABattleInstance::ABattleInstance()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;

	BattleTypeManager = nullptr;
	BattleAbilityManager = nullptr;
	BattleSpecieManager = nullptr;
	bNetworkBattle = false;
	bUseOverride = false;
	bPaused = true;
	BattleStep = BattleStep_Spawned;
	ClientTeam = -1;
	OverrideFolder = "Default";

	ClientExecutedStepSetClientManagers = false;
	ClientExecuteStepSetClientTeams = false;
}

void ABattleInstance::BeginPlay()
{
	Super::BeginPlay();

	if (IsRunningDedicatedServer()) {
		check(bNetworkBattle);
		if (!bUseOverride) {
			bUseOverride = true;
		}
	}

	if (!HasAuthority()) {
		ULogger::Log("Notifying server that this client loaded the Battle Instance object");
		GetLocalPlayerController()->InformBattleInstanceClientSpawned(this);
	}
}

void ABattleInstance::ClientSpawned(AImmiePlayerController* Player)
{
	iLog("Recieved that client spawned battle instance");
	int NewClientTeam = -1;

	for (int i = 0; i < InitTeams.Num(); i++) {
		if (InitTeams[i].Controller == Cast<AController>(Player)) {
			NewClientTeam = i;
			break;
		}
	}
	if (NewClientTeam == -1) {
		iLog("Could not find a team with the specified controller in the init teams", LogVerbosity_Error);
		return;
	}
	Player->SetClientBattleTeamIndex(this, NewClientTeam);
}

TEnumAsByte<EBattleStep> ABattleInstance::IncrementBattleStep(uint8 AmountToIncrement)
{
	SetBattleStep((EBattleStep)(BattleStep + AmountToIncrement));
	return BattleStep;
}

void ABattleInstance::SetBattleStep_Implementation(uint8 NewBattleStep)
{
	BattleStep = (EBattleStep)NewBattleStep;
}

void ABattleInstance::CreateBattleManagers()
{
	BattleTypeManager = NewObject<UBattleTypeManager>(this); 
	BattleAbilityManager = NewObject<UBattleAbilityManager>(this);
	BattleSpecieManager = NewObject<UBattleSpecieManager>(this);
}

void ABattleInstance::InitializeBattleManagers_Implementation()
{
	BattleTypeManager->Initialize(this, bUseOverride, OverrideFolder);
	BattleAbilityManager->Initialize(this, bUseOverride, OverrideFolder);
	BattleSpecieManager->Initialize(this, bUseOverride, OverrideFolder);
}

void ABattleInstance::SetClientManagers_Implementation(UBattleTypeManager* NewBattleTypeManager, UBattleAbilityManager* NewBattleAbilityManager, UBattleSpecieManager* NewBattleSpecieManager)
{
	BattleTypeManager = NewBattleTypeManager;
	BattleAbilityManager = NewBattleAbilityManager;
	BattleSpecieManager = NewBattleSpecieManager;
}

void ABattleInstance::ClientManagersValid(AImmiePlayerController* Player)
{
	for (int i = 0; i < InitTeams.Num(); i++) {
		if (InitTeams[i].Controller == Cast<AController>(Player)) {
			// Assume valid index.
			ClientsValidSpawnedManagers[i] = true;
		}
	}
}

void ABattleInstance::SyncClientManagers()
{
	BattleTypeManager->SyncToClients();
	BattleAbilityManager->SyncToClients();
	BattleSpecieManager->SyncToClients();
}

void ABattleInstance::CreateTeams()
{
	for (int i = 0; i < InitTeams.Num(); i++) {
		UClass* TeamClass = GetBattleDataManager()->GetTeamClass(InitTeams[i].TeamType);
		//check(TeamClass);
		FTransform SpawnTransform = FTransform(InitTeams[i].SpawnRotation, InitTeams[i].SpawnLocation, { 1, 1, 1 });
		ABattleTeam* Team = GetWorld()->SpawnActorDeferred<ABattleTeam>(TeamClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//check(Team);
		UGameplayStatics::FinishSpawningActor(Team, SpawnTransform);
		Teams.Add(Team);
	}
}

void ABattleInstance::InitializeTeams_Implementation()
{
	for (int i = 0; i < Teams.Num(); i++) {
		Teams[i]->InitializeTeam(this, InitTeams[i]);
	}
}

void ABattleInstance::SetClientTeams_Implementation(const TArray<ABattleTeam*>& NewTeams)
{
	Teams = NewTeams;
}

void ABattleInstance::ClientTeamsValid(AImmiePlayerController* Player)
{
	for (int i = 0; i < Teams.Num(); i++) {
		if (Teams[i]->GetController() == Cast<AController>(Player)) {
			// Assume valid index.
			ClientsValidTeamObjects[i] = true;
		}
	}
}

void ABattleInstance::SyncClientTeams()
{
	for (int i = 0; i < Teams.Num(); i++) {
		Teams[i]->SyncClientTeam();
	}
}

bool ABattleInstance::AllClientsHaveValidManagers()
{
	bool ValidManagerObjects = true;
	for (int i = 0; i < ClientsValidSpawnedManagers.Num(); i++) {
		if (ClientsValidSpawnedManagers[i] == false)
			ValidManagerObjects = false;
	}
	return ValidManagerObjects;
}

bool ABattleInstance::AllClientsHaveValidTeams()
{
	bool ValidTeamObjects = true;
	for (int i = 0; i < ClientsValidTeamObjects.Num(); i++) {
		if (ClientsValidTeamObjects[i] == false) {
			ValidTeamObjects = false;
		}
	}
	return ValidTeamObjects;
}

void ABattleInstance::SetImmieForBattle(ABattleTeam* Team, AImmieCharacter* ImmieCharacter)
{
	Team->SetImmieForBattle(ImmieCharacter);
}

void ABattleInstance::SetFirstImmiesForBattle()
{
	for (int i = 0; i < Teams.Num(); i++) {
		SetImmieForBattle(Teams[i], Teams[i]->GetImmieCharacter(0));
	}
}

void ABattleInstance::AuthorityBattleTick(float DeltaTime)
{
	for (int i = 0; i < Teams.Num(); i++) {
		Teams[i]->AuthorityBattleTick(DeltaTime);
	}
}

void ABattleInstance::ClientBattleTick(float DeltaTime)
{
	for (int i = 0; i < Teams.Num(); i++) {
		Teams[i]->ClientBattleTick(DeltaTime);
	}
}

void ABattleInstance::FinalizeInitialization_Implementation()
{
	IncrementBattleStep();
}

void ABattleInstance::SetPaused_Implementation(bool NewPaused)
{
	bPaused = NewPaused;
}

#pragma region Battle_Step

void ABattleInstance::AuthorityBattleStepSpawned_Implementation()
{
}

void ABattleInstance::AuthorityBattleStepCreateManagers_Implementation()
{
	iLog("creating managers");
	CreateBattleManagers();
	IncrementBattleStep();
}

void ABattleInstance::AuthorityBattleStepInitializeManagers_Implementation()
{
	iLog("initializing managers");
	InitializeBattleManagers();

	// Don't need to perform client syncing if not networked.
	if (bNetworkBattle) {
		IncrementBattleStep();
	}
	else {
		// Skip over set client managers and sync client managers steps.
		IncrementBattleStep(3);
	}
}

void ABattleInstance::AuthorityBattleStepSetClientManagers_Implementation()
{
	iLog("setting client managers");
	SetClientManagers(BattleTypeManager, BattleAbilityManager, BattleSpecieManager);
	IncrementBattleStep();
}

void ABattleInstance::AuthorityBattleStepSyncClientManagers_Implementation()
{
	if (AllClientsHaveValidManagers()) {
		iLog("Clients have valid managers. Syncing");
		SyncClientManagers();
		IncrementBattleStep();
	}
}

void ABattleInstance::AuthorityBattleStepCreateTeams_Implementation()
{
	iLog("creating teams");
	CreateTeams();
	IncrementBattleStep();
}

void ABattleInstance::AuthorityBattleStepInitializeTeams_Implementation()
{
	iLog("initializing teams");
	InitializeTeams();

	// Don't need to perform client syncing if not networked.
	if (bNetworkBattle) {
		IncrementBattleStep();
	}
	else {
		// Skip over set client managers and sync client teams steps.
		IncrementBattleStep(3);
	}
}

void ABattleInstance::AuthorityBattleStepSetClientTeams_Implementation()
{
	iLog("setting client teams");
	SetClientTeams(Teams);
	for (int i = 0; i < Teams.Num(); i++) {
		Teams[i]->SetClientSubobjects();
	}
	IncrementBattleStep();
}

void ABattleInstance::AuthorityBattleStepSyncClientTeams_Implementation()
{
	if (AllClientsHaveValidTeams()) {
		SyncClientTeams();
		IncrementBattleStep();
	}
}

void ABattleInstance::AuthorityBattleStepFinalizeInitialization_Implementation()
{
	iLog("finalizing initialization");
	FinalizeInitialization();
}

void ABattleInstance::AuthorityBattleStepStartBattle_Implementation()
{
	SetFirstImmiesForBattle();
	IncrementBattleStep();
}

void ABattleInstance::AuthorityBattleStepBattleRunning_Implementation(float DeltaTime)
{
	AuthorityBattleTick(DeltaTime);
}

void ABattleInstance::AuthorityBattleStepEndBattle_Implementation()
{
}

void ABattleInstance::AuthorityBattleStepDestroy_Implementation()
{
	Destroy();
}

void ABattleInstance::ClientBattleStepSpawned_Implementation()
{
}

void ABattleInstance::ClientBattleStepCreateManagers_Implementation()
{
}

void ABattleInstance::ClientBattleStepInitializeManagers_Implementation()
{
}

void ABattleInstance::ClientBattleStepSetClientManagers_Implementation()
{
}

void ABattleInstance::ClientBattleStepSyncClientManagers_Implementation()
{
	//iLog("on client battle step sync client managers");
	if (!ClientExecutedStepSetClientManagers) {
		if (IsValid(BattleTypeManager) && IsValid(BattleAbilityManager) && IsValid(BattleSpecieManager)) {
			GetLocalPlayerController()->InformBattleInstanceClientValidManagers(this);
			ClientExecutedStepSetClientManagers = true;
		}
	}
}

void ABattleInstance::ClientBattleStepCreateTeams_Implementation()
{
}

void ABattleInstance::ClientBattleStepInitializeTeams_Implementation()
{
}

void ABattleInstance::ClientBattleStepSetClientTeams_Implementation()
{
}

void ABattleInstance::ClientBattleStepSyncClientTeams_Implementation()
{
	if (!ClientExecuteStepSetClientTeams) {
		const bool TeamsContain = Teams.Num() != 0;
		bool ValidTeamOwners = true;
		bool ValidTeamSubobjects = true;
		for (int i = 0; i < Teams.Num(); i++) {
			if (!IsValid(Teams[i])) {
				ValidTeamOwners = false;
				break;
			}
			if (!Teams[i]->ValidSubobjects()) {
				ValidTeamSubobjects = false;
				break;
			}
		}

		if (TeamsContain && ValidTeamOwners && ValidTeamSubobjects) {
			GetLocalPlayerController()->InformBattleInstanceClientValidTeams(this);
			ClientExecuteStepSetClientTeams = true;
		}
	}
}

void ABattleInstance::ClientBattleStepFinalizeInitialization_Implementation()
{
}

void ABattleInstance::ClientBattleStepStartBattle_Implementation()
{
}

void ABattleInstance::ClientBattleStepBattleRunning_Implementation(float DeltaTime)
{
	ClientBattleTick(DeltaTime);
}

void ABattleInstance::ClientBattleStepEndBattle_Implementation()
{
}

void ABattleInstance::ClientBattleStepDestroy_Implementation()
{
}

#pragma endregion

void ABattleInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) {
		AuthorityTick(DeltaTime);
	}
	else {
		ClientTick(DeltaTime);
	}
	
}

void ABattleInstance::AuthorityTick(float DeltaTime)
{
	switch (BattleStep) {
	case BattleStep_Spawned:	
		AuthorityBattleStepSpawned();
		break;
	case BattleStep_CreateManagers:
		AuthorityBattleStepCreateManagers();
		break;
	case BattleStep_InitializeManagers:
		AuthorityBattleStepInitializeManagers();
		break;
	case BattleStep_SetClientManagers:
		AuthorityBattleStepSetClientManagers();
		break;
	case BattleStep_SyncClientManagers:
		AuthorityBattleStepSyncClientManagers();
		break;
	case BattleStep_CreateTeams:
		AuthorityBattleStepCreateTeams();
		break;
	case BattleStep_InitializeTeams:
		AuthorityBattleStepInitializeTeams();
		break;
	case BattleStep_SetClientTeams:
		AuthorityBattleStepSetClientTeams();		
		break;
	case BattleStep_SyncClientTeams:
		AuthorityBattleStepSyncClientTeams();
		break;
	case BattleStep_FinalizeInitialization:
		AuthorityBattleStepFinalizeInitialization();
		break;
	case BattleStep_StartBattle:
		AuthorityBattleStepStartBattle();
		break;
	case BattleStep_BattleRunning:
		AuthorityBattleStepBattleRunning(DeltaTime);
		break;
	case BattleStep_EndBattle:
		AuthorityBattleStepEndBattle();
		break;
	case BattleStep_Destroy:
		AuthorityBattleStepDestroy();
		break;
	}

	BP_AuthorityTick(DeltaTime);
}

void ABattleInstance::ClientTick(float DeltaTime)
{
	switch (BattleStep) {
	case BattleStep_Spawned:
		ClientBattleStepSpawned();
		break;
	case BattleStep_CreateManagers:
		ClientBattleStepCreateManagers();
		break;
	case BattleStep_InitializeManagers:
		ClientBattleStepInitializeManagers();
		break;
	case BattleStep_SetClientManagers:
		ClientBattleStepSetClientManagers();
		break;
	case BattleStep_SyncClientManagers:
		ClientBattleStepSyncClientManagers();
		break;
	case BattleStep_CreateTeams:
		ClientBattleStepCreateTeams();
		break;
	case BattleStep_InitializeTeams:
		ClientBattleStepInitializeTeams();
		break;
	case BattleStep_SetClientTeams:
		ClientBattleStepSetClientTeams();
		break;
	case BattleStep_SyncClientTeams:
		ClientBattleStepSyncClientTeams();
		break;
	case BattleStep_FinalizeInitialization:
		ClientBattleStepFinalizeInitialization();
		break;
	case BattleStep_StartBattle:
		ClientBattleStepStartBattle();
		break;
	case BattleStep_BattleRunning:
		ClientBattleStepBattleRunning(DeltaTime);
		break;
	case BattleStep_EndBattle:
		ClientBattleStepEndBattle();
		break;
	case BattleStep_Destroy:
		ClientBattleStepDestroy();
		break;
	}

	BP_ClientTick(DeltaTime);
}

ABattleInstance* ABattleInstance::NewBattleInstance(AActor* _Owner, FName BattleInstanceName, const FVector& Location)
{
	UClass* BattleInstanceClass = GetBattleDataManager()->GetBattleInstanceClass(BattleInstanceName);
	check(IsValid(BattleInstanceClass));
	FTransform SpawnTransform = FTransform({ 0, 0, 0 }, Location, { 1, 1, 1 });
	if (_Owner == nullptr) {
		_Owner = Cast<AActor>(UGameplayStatics::GetGameMode(GEngine->GetWorld()));
	}
	ABattleInstance* Spawned = _Owner->GetWorld()->SpawnActorDeferred<ABattleInstance>(BattleInstanceClass, SpawnTransform, _Owner, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	check(Spawned);
	UGameplayStatics::FinishSpawningActor(Spawned, SpawnTransform);
	return Spawned;
}

void ABattleInstance::BattleInit(const TArray<FBattleTeamInit>& _InitTeams)
{
	InitTeams = _InitTeams;

	const int TeamCount = InitTeams.Num();
	Teams.Reserve(TeamCount);

	if (bNetworkBattle) {
		ClientsValidSpawnedManagers.Init(false, TeamCount);
		ClientsValidTeamObjects.Init(false, TeamCount);
		for (int i = 0; i < TeamCount; i++) {
			const bool IsPlayerTeam =
				InitTeams[i].TeamType == EBattleTeamType::BattleTeam_PlayerMultiplayer;
			if (!IsPlayerTeam) {
				ClientsValidSpawnedManagers[i] = true;
				ClientsValidTeamObjects[i] = true;
			}
		}
	}

	IncrementBattleStep();
}

void ABattleInstance::SetClientTeamIndex(int NewClientTeam)
{
	ClientTeam = NewClientTeam;
}

AImmiePlayerController* ABattleInstance::GetLocalPlayerController() const
{
	return Cast<AImmiePlayerController>(GetWorld()->GetFirstPlayerController());
}

ABattleTeam* ABattleInstance::GetPlayerTeam() const
{
	return Teams[GetPlayerTeamIndex()];
}

