// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Immie/Battle/BattleStructures.h>
#include "BattleTeam.generated.h"

class ABattleInstance;
class UImmie;
class AImmieCharacter;
class AAbilityActor;
class AImmieAIController;

UCLASS()
class IMMIE_API ABattleTeam : public AActor
{
	GENERATED_BODY()

private:

	TArray<AImmieCharacter*> Team;

	AImmieCharacter* ActiveImmie;

	ABattleInstance* BattleInstance;

	FVector DefaultSpawnLocation;

	FRotator DefaultSpawnRotation;

	AController* Controller;

	TScriptInterface<IBattler> TeamOwner;

	TEnumAsByte<EBattleTeamType> TeamType;

	TArray<AAbilityActor*> AbilityActors;

	/* If the team is current alive. */
	uint8 bTeamAlive : 1;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		TSubclassOf<AImmieAIController> AIControllerClass;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, DisplayName = "Create Team")
		/* Creates the Immie characters for the team. By default just creates Immie characters from the passed in team data. Can be fully overridden in blueprint. */
		void BP_CreateTeam(const FBattleTeamInit& TeamData);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Initialize Team")
		/* Blueprint side initialization for team. By default, Immies will be spawned in along with their subobjects, however this functionality can be overridden. See BP_CreateTeam() native event. */
		void BP_InitializeTeam(const FBattleTeamInit& TeamData);

	UFUNCTION(NetMulticast, Reliable)
		/* Synchronizes the team's subobjects (not the Immies). */
		void SyncClientSubobjects(ABattleInstance* BattleInstanceObject, AController* ControllerObject, const TArray<AImmieCharacter*>& TeamCharacterObjects);

	UFUNCTION(BlueprintCallable)
		/* Creates a team of Immie characters from an array of Immie objects. */
		void CreateTeamFromImmies(const TArray<UImmie*>& TeamImmies);

	UFUNCTION(BlueprintCallable)
		/* Perform authority battle ticking on all of the battle actors on this team. */
		void AuthorityBattleTickBattleActors(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		/* Perform client battle ticking on all of the battle actors on this team. */
		void ClientBattleTickBattleActors(float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
		/*  */
		void SetActiveImmie(AImmieCharacter* NewActiveImmie);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Sync To Clients")
		/* Blueprint sided client syncing. Check ABattleTeam::SyncToClients() to see what is already handled. */
		void BP_SyncClientTeam();

	UFUNCTION(BlueprintPure)
		/**/
		TArray<AImmieCharacter*> AllBattleReadyImmies(bool IncludeActiveImmie = false) const;

	UFUNCTION(BlueprintCallable)
		/**/
		bool SwapInNextImmie();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Battle End")
		/* Blueprint sided handling for when a battle ends. WinState is the state of this team's win condition. 
		IsLocalPlayerTeam is if this team's controller is using the local player controller, and is not running on a dedicated server. */
		void BP_OnBattleEnd(EBattleTeamWinState WinState, bool IsLocalPlayerTeam);

public:	

	ABattleTeam(); 

	virtual void Tick(float DeltaTime) override;

	/* Initialize this team given provided initial data. */
	void InitializeTeam(ABattleInstance* _BattleInstance, const FBattleTeamInit& TeamData);

	/* Set all of the subobject variables for all clients for all team relevant objects. */
	void SetClientSubobjects();

	/* Check all client-relevant subobjects and see if they are valid. */
	bool ValidSubobjects();

	/* Syncronizes team data with the clients. */
	void SyncClientTeam();

	UFUNCTION(BlueprintCallable)
		/**/
		void SetImmieForBattle(AImmieCharacter* ImmieCharacter);

	UFUNCTION(BlueprintCallable)
		/**/
		void RemoveActiveImmieFromBattle();
	
	UFUNCTION(BlueprintCallable)
		/* Spawned ability actor will be owned by this team actor. */
		AAbilityActor* SpawnAbilityActor(TSubclassOf<AAbilityActor> AbilityActorClass, UAbility* Ability, const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
		/**/
		void RemoveAbilityActor(AAbilityActor* AbilityActor);

	UFUNCTION(BlueprintNativeEvent)
		/* Authority sided battle ticking for this team and the actors and objects it owns. By default calls ticking on them. Can be overridden in blueprints. */
		void AuthorityBattleTick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
		/* Client sided battle ticking for this team and the actors and objects it owns. By default calls ticking on them. Can be overridden in blueprints. */
		void ClientBattleTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		/**/
		void DestroyBattleActors();

	UFUNCTION(NetMulticast, Reliable)
		/**/
		void OnBattleEnd(EBattleTeamWinState WinState);

	UFUNCTION(BlueprintNativeEvent, Category = "BattleEvents", DisplayName = "Player Dealt Healing")
		/* Event that executes immediately before an ability that an Immie character owns is dealing healing. The amount of healing done can be freely modified. */
		void EventPlayerDealtHealing(const TScriptInterface<IBattleActor>& Target, UPARAM(ref) float& Amount, UPARAM(ref) FBattleDamage& Healing, AImmieCharacter* ImmieCharacter);

	UFUNCTION(BlueprintNativeEvent, Category = "BattleEvents", DisplayName = "Player Dealt Damage")
		/* Event that executes immediately before an ability that an Immie character owns is dealing damage. The amount of damage done can be freely modified. */
		void EventPlayerDealtDamage(const TScriptInterface<IBattleActor>& Target, UPARAM(ref) float& Amount, UPARAM(ref) FBattleDamage& Damage, AImmieCharacter* ImmieCharacter);

	UFUNCTION(BlueprintPure)
		/* Returns the type of team this is. Override this for all blueprint child classes. */
		TEnumAsByte<EBattleTeamType> GetBattleTeamType() const { return TeamType; };

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE ABattleInstance* GetBattleInstance() const { return BattleInstance; }

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE AController* GetController() const { return Controller; }

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE TArray<AImmieCharacter*> GetTeam() const { return Team; }

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE AImmieCharacter* GetImmieCharacter(int Index) const { return Team[Index]; }

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE bool IsTeamAlive() const { return bTeamAlive; }

	UFUNCTION(BlueprintPure)
		/**/
		AImmieCharacter* GetActiveImmie() const { return ActiveImmie; }

	UFUNCTION(BlueprintPure)
		/**/
		TArray<AAbilityActor*> GetAbilityActors() const { return AbilityActors; }

};
