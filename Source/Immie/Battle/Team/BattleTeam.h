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

	AActor* TeamOwner;

	TEnumAsByte<EBattleTeamType> TeamType;

	TArray<AAbilityActor*> AbilityActors;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		/* Creates a team of Immie characters from an array of Immie objects. */
		void CreateTeamFromImmies(const TArray<UImmie*>& TeamImmies);

	UFUNCTION(BlueprintCallable)
		/* Perform authority battle ticking on all of the battle actors on this team. */
		void AuthorityBattleTickBattleActors(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		/* Perform client battle ticking on all of the battle actors on this team. */
		void ClientBattleTickBattleActors(float DeltaTime);

public:	

	ABattleTeam(); 

	virtual void Tick(float DeltaTime) override;

	/* Initialize this team given provided initial data. */
	void InitializeTeam(ABattleInstance* _BattleInstance, const FBattleTeamInit& TeamData);

	UFUNCTION(BlueprintNativeEvent, DisplayName = "Create Team")
		/* Creates the Immie characters for the team. By default just creates Immie characters from the passed in team data. Can be fully overridden in blueprint. */
		void BP_CreateTeam(const FBattleTeamInit& TeamData);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Initialize Team")
		/* Blueprint side initialization for team. By default, Immies will be spawned in along with their subobjects, however this functionality can be overridden. See BP_CreateTeam() native event. */
		void BP_InitializeTeam(const FBattleTeamInit& TeamData);

	/* Set all of the subobject variables for all clients for all team relevant objects. */
	void SetClientSubobjects();

	UFUNCTION(NetMulticast, Reliable)
		/* Synchronizes the team's subobjects (not the Immies). */
		void SyncClientSubobjects(ABattleInstance* BattleInstanceObject, AController* ControllerObject, const TArray<AImmieCharacter*>& TeamCharacterObjects);

	/* Check all client-relevant subobjects and see if they are valid. */
	bool ValidSubobjects();

	/* Syncronizes team data with the clients. */
	void SyncClientTeam();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Sync To Clients")
		/* Blueprint sided client syncing. Check ABattleTeam::SyncToClients() to see what is already handled. */
		void BP_SyncClientTeam();

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

	UFUNCTION(BlueprintPure)
		/* Returns the type of team this is. Override this for all blueprint child classes. */
		EBattleTeamType GetBattleTeamType();

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



};
