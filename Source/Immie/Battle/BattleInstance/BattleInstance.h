// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Immie/Battle/BattleStructures.h>
#include "BattleInstance.generated.h"

class ABattleTeam;
class UBattleTypeManager;
class UBattleAbilityManager;
class UBattleSpecieManager;
class AImmiePlayerController;
class AImmieCharacter;

UCLASS()
class IMMIE_API ABattleInstance : public AActor
{
	GENERATED_BODY()

private:

	/* Keeps track of which clients have successfully spawned all managers locally. */
	TArray<bool> ClientsValidSpawnedManagers;

	/* Keeps track of which clients have successfully spawned all team relevant objects locally. */
	TArray<bool> ClientsValidTeamObjects;

	/* The team of the local client. */
	int ClientTeam;

protected:

	UPROPERTY(BlueprintReadWrite)
		/* Team data for all teams to initialize with. */
		TArray<FBattleTeamInit> InitTeams;

	UPROPERTY(BlueprintReadWrite)
		/* The objects (actors technically) that own the immie characters, ability actors, etc for any given team. */
		TArray<ABattleTeam*> Teams;

	UPROPERTY(BlueprintReadWrite)
		/* Component to manage type related battle stuff. */
		UBattleTypeManager* BattleTypeManager;

	UPROPERTY(BlueprintReadWrite)
		/* Component to manage type related battle stuff. */
		UBattleAbilityManager* BattleAbilityManager;

	UPROPERTY(BlueprintReadWrite)
		/* Component to manage type related battle stuff. */
		UBattleSpecieManager* BattleSpecieManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Whether this battle instance uses networking or not. Network battles are forced to load their own data for correct replication. */
		uint8 bNetworkBattle : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Does this battle use override battle data. Automatically set to true if network battle. */
		uint8 bUseOverride : 1;

	UPROPERTY(BlueprintReadWrite)
		/* If the battle is currently paused or not. TODO implement. */
		uint8 bPaused : 1;

	UPROPERTY(BlueprintReadWrite)
		/* Client side flag for whether it has successfully executed the RPC from the server to set it's manager objects. */
		uint8 ClientExecutedStepSetClientManagers : 1;

	UPROPERTY(BlueprintReadWrite)
		/* Client side flag for whether it has successfully executed the RPC from the server to set it's team objects. */
		uint8 ClientExecuteStepSetClientTeams : 1;

	UPROPERTY(BlueprintReadWrite)
		/* The current step the battle is on. Primarily to control flow in networked battles. Automatically set to "Default" if bUseOverride is false set AND bNetworkBattle is true. */
		TEnumAsByte<EBattleStep> BattleStep;

	UPROPERTY(BlueprintReadWrite)
		/**/
		uint8 BattlePhase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* The folder that this battle instance will read the battle data from. Network battles will ALWAYS load data from the specified folder. */
		FString OverrideFolder;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		/* Increments the battle step. Also replicates to clients. */
		TEnumAsByte<EBattleStep> IncrementBattleStep(uint8 AmountToIncrement = 1);

	UFUNCTION(NetMulticast, Reliable)
		/* Sets battle step for server and all clients (or standalone). */
		void SetBattleStep(uint8 NewBattleStep);

	/* Battle instance tick for authoritative side (server / standalone). */
	void AuthorityTick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Authority Tick")
		/* Blueprint sided ticking for authoritative side (server / standalone). */
		void BP_AuthorityTick(float DeltaTime);

	/* Battle instance tick for non-authoritative side (client only). */
	void ClientTick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Client Tick")
		/* Blueprint sided ticking for non-authoritative side (client only). */
		void BP_ClientTick(float DeltaTime);

	/* Creates all battle manager objects. For blueprints, assume they were created. 
	Also calls the initialization on the managers, or allows blueprint to take over or manage initialization. */
	void CreateBattleManagers();

	UFUNCTION(BlueprintNativeEvent)
		/* Initializes the battle managers. Can be completely overridden in blueprints. Assume the battle managers are all valid. */
		void InitializeBattleManagers();

	UFUNCTION(NetMulticast, Reliable)
		/* Set all of the clients manager objects to the following. RPC instead of DOREPLIFETIME. */
		void SetClientManagers(UBattleTypeManager* NewBattleTypeManager, UBattleAbilityManager* NewBattleAbilityManager, UBattleSpecieManager* NewBattleSpecieManager);

	/* Syncronize any necessary data from all of the battle managers to the clients. */
	void SyncClientManagers();

	/* Creates the team owner actors and adds them to the Teams array.
	Also calls the initialization on the teams, or allows blueprint to take over or manage initialization. */
	void CreateTeams();

	UFUNCTION(BlueprintNativeEvent)
		/* Initializes the teams that were created. Can be completely overridden in blueprints. Assume the BattleTeam actors within Teams are all valid. */
		void InitializeTeams();

	UFUNCTION(NetMulticast, Reliable)
		/*  */
		void SetClientTeams(const TArray<ABattleTeam*>& NewTeams);

	/**/
	void SyncClientTeams();

	UFUNCTION(BlueprintNativeEvent)
		/* Currently just increments the battle step. Override for custom battle functionality. 
		Will need to manually increment battle step if overridden without calling parent implementation. */
		void FinalizeInitialization();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		/**/
		void SetPaused(bool NewPaused);

	UFUNCTION(BlueprintPure)
		/* Do all of the clients have valid battle manager objects? */
		bool AllClientsHaveValidManagers();

	UFUNCTION(BlueprintPure)
		/* Do all of the clients have valid team objects? */
		bool AllClientsHaveValidTeams();

	void SetImmieForBattle(ABattleTeam* Team, AImmieCharacter* ImmieCharacter);

	void SetFirstImmiesForBattle();

	UFUNCTION(BlueprintCallable)
		/* Battle ticking for authoritative side. Propogates to all in-battle relevant subobjects. */
		void AuthorityBattleTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		/* Battle ticking for client side. Propogates to all in-battle relevant subobjects. */
		void ClientBattleTick(float DeltaTime);

#pragma region Battle_Step

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepSpawned();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepCreateManagers();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepInitializeManagers();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepSetClientManagers();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepSyncClientManagers();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepCreateTeams();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepInitializeTeams();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepSetClientTeams();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepSyncClientTeams();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepFinalizeInitialization();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepStartBattle();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepBattleRunning(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepEndBattle();

	UFUNCTION(BlueprintNativeEvent, Category = "Authority Battle Step")
		/**/
		void AuthorityBattleStepDestroy();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepSpawned();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepCreateManagers();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepInitializeManagers();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepSetClientManagers();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepSyncClientManagers();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepCreateTeams();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepInitializeTeams();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepSetClientTeams();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepSyncClientTeams();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepFinalizeInitialization();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepStartBattle();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepBattleRunning(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepEndBattle();

	UFUNCTION(BlueprintNativeEvent, Category = "Client Battle Step")
		/**/
		void ClientBattleStepDestroy();

#pragma endregion

public:

	ABattleInstance();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		/* Owner can be null. If so, sets owner to the world's gamemode. */
		static ABattleInstance* NewBattleInstance(AActor* _Owner, FName BattleInstanceName, const FVector& Location);

	/* Initializes a battle instance with relevant team data. Battle instance will carry on to perform any steps necessary to prepare and then start a battle. */
	void BattleInit(const TArray<FBattleTeamInit>& _InitTeams);

	/* Called by a client player controller to inform the battle instance that a player has spawned a client version of the battle instance.
	Also sets the clients team index. */
	void ClientSpawned(AImmiePlayerController* Player);

	/* Called by a client player controller to inform the server that it's battle managers were set (are valid objects). */
	void ClientManagersValid(AImmiePlayerController* Player);

	/* Called by a client player controller to inform the server that the client has valid team objects. */
	void ClientTeamsValid(AImmiePlayerController* Player);

	UFUNCTION(BlueprintCallable)
		/**/
		void SetClientTeamIndex(int NewClientTeam);

	UFUNCTION(BlueprintPure)
		/**/
		UBattleTypeManager* GetBattleTypeManager() const { return BattleTypeManager; }

	UFUNCTION(BlueprintPure)
		/**/
		UBattleAbilityManager* GetBattleAbilityManager() const { return BattleAbilityManager; }

	UFUNCTION(BlueprintPure)
		/**/
		UBattleSpecieManager* GetBattleSpecieManager() const { return BattleSpecieManager; }

	UFUNCTION(BlueprintPure)
		/**/
		AImmiePlayerController* GetLocalPlayerController() const;

	UFUNCTION(BlueprintPure)
		/* Get the index of the player's or running client's team. */
		int GetPlayerTeamIndex() const { return ClientTeam; }

	UFUNCTION(BlueprintPure)
		/**/
		ABattleTeam* GetPlayerTeam() const;

	UFUNCTION(BlueprintPure)
		/**/
		bool IsPaused() const { return bPaused; }

	UFUNCTION(BlueprintPure)
		/**/
		int GetTeamCount() const { return Teams.Num(); }

	UFUNCTION(BlueprintPure)
		/**/
		ABattleTeam* GetTeam(int index) const { return Teams[index]; }



};
