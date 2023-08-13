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
class ABattleAiController;
class AImmiePlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthorityBattleTickSignature, float, DeltaTime);

UCLASS()
class IMMIE_API ABattleTeam : public AActor
{
	GENERATED_BODY()

private:

	TArray<AImmieCharacter*> Team;

	AImmieCharacter* ActiveImmie;

	ABattleInstance* BattleInstance;

	AImmiePlayerController* PlayerController;

	TArray<AAbilityActor*> AbilityActors;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		bool IsPlayerControlled;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(EditCondition="!IsPlayerControlled", EditConditionHides))
		/* Only used when this team is set to explicitly not use a player controller. See FBattleTeamInit. */
		TSubclassOf<ABattleAiController> AIControllerClass;

	UPROPERTY(BlueprintReadWrite)
		/**/
		FTransform ImmieSpawnTransform;

	UPROPERTY(BlueprintReadOnly)
		/* Blueprint implementations are free to cast this to whatever valid data type. 
		For example, singleplayer teams could cast this object to AOverworldPlayer. */
		UObject* TeamOwnerAsObject;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/* If an Immie Object's outer is an immie character, use that character for battle instead of making a new one. */
		uint8 bUseOuterImmieCharacters : 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		uint8 bDestroyAllCharactersOnBattleEnd : 1;

	/* If the team is current alive. */
	uint8 bTeamAlive : 1;

public:

	UPROPERTY(BlueprintAssignable)
		/**/
		FOnAuthorityBattleTickSignature OnAuthorityBattleTickMulticastDelegate;

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
		void SyncClientSubobjects(ABattleInstance* BattleInstanceObject, AImmiePlayerController* PlayerControllerObject, const TArray<AImmieCharacter*>& TeamCharacterObjects);

	UFUNCTION(BlueprintCallable)
		/* Makes an battle immie character from an immie object. If the object's outer is an immie character,
		that character can be used instead of making a new one. See flag bUseOuterImmieCharacters. */
		AImmieCharacter* MakeBattleImmie(UImmie* ImmieObject);

	UFUNCTION(BlueprintCallable)
		/* Creates a team of Immie characters from an array of Immie objects. 
		If the Immie object's owner is a wild immie character, that character can be made into a battle immie character instead of making a new one. */
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
		void DestroyAllAbilityActors();

	UFUNCTION(BlueprintCallable)
		/**/
		void DestroyAllImmies();

	UFUNCTION(BlueprintCallable)
		/* If the Immie Character has been defeated, it can be captured by the player, otherwise it will be given to the spawner. */
		void CaptureOrReturnWildImmieToSpawner(AImmieCharacter* ImmieCharacter, AWildImmieSpawner* Spawner, bool Heal = true);

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
		/**/
		FORCEINLINE ABattleInstance* GetBattleInstance() const { return BattleInstance; }

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

	UFUNCTION(BlueprintPure)
		/**/
		AImmiePlayerController* GetPlayerController() const { return PlayerController; }

};
