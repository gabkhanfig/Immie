// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleStructures.generated.h"

class UImmie;
class UAbility;
class IBattler;
class ABattleTeam;

UENUM(BlueprintType)
enum EBattleTeamType
{
	BattleTeam_None					UMETA(DisplayName = "None"),
	BattleTeam_PlayerSingleplayer	UMETA(DisplayName = "Player Singleplayer"),
	BattleTeam_PlayerMultiplayer	UMETA(DisplayName = "Player Multiplayer"),
	BattleTeam_Wild					UMETA(DisplayName = "Wild"),
	BattleTeam_Trainer				UMETA(DisplayName = "Trainer")
};

UENUM(BlueprintType)
enum EBattleStep
{
	BattleStep_Spawned = 0							UMETA(DisplayName = "Spawned"),
	BattleStep_CreateManagers = 1					UMETA(DisplayName = "Create Managers"),
	BattleStep_InitializeManagers = 2				UMETA(DisplayName = "Initialize Managers"),
	BattleStep_SetClientManagers = 3				UMETA(DisplayName = "Set Client Managers"),
	BattleStep_SyncClientManagers = 4				UMETA(DisplayName = "Sync Client Managers"),
	BattleStep_CreateTeams = 5						UMETA(DisplayName = "Create Teams"),
	BattleStep_InitializeTeams = 6					UMETA(DisplayName = "Initialize Teams"),
	BattleStep_SetClientTeams = 7					UMETA(DisplayName = "Set Client Teams"),
	BattleStep_SyncClientTeams = 8					UMETA(DisplayName = "Sync Client Teams"),
	BattleStep_FinalizeInitialization = 9			UMETA(DisplayName = "Finalize Initialization"),
	BattleStep_StartBattle = 10						UMETA(DisplayName = "Start Battle"),
	BattleStep_BattleRunning = 11					UMETA(DisplayName = "Battle Running"),
	BattleStep_EndBattle = 12						UMETA(DisplayName = "End Battle"),
	BattleStep_Destroy = 13							UMETA(DisplayName = "Destroy")
};

UENUM(BlueprintType)
enum EBattleTeamWinState
{
	BattleTeamWinState_Win UMETA(DisplayName = "Win"),
	BattleTeamWinState_Lose UMETA(DisplayName = "Lose"),
	BattleTeamWinState_Draw UMETA(DisplayName = "Draw"),
};

USTRUCT(BlueprintType)
struct FBattleTeamInit 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		TArray<UImmie*> Team;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		FTransform SpawnTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Can be null. If set to nullptr, an AI controller will be created for this battle team. */
		AController* Controller;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* The actor that "owns" the team. */
		TScriptInterface<IBattler> Trainer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		TSubclassOf<ABattleTeam> BattleTeamClass;

	FBattleTeamInit();

};

USTRUCT(BlueprintType)
struct FBattleSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Set to 0 for Immies to gain no Xp from the battle. */
		float BaseXpMultiplier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		float ParticipationXpMultiplier;
};

USTRUCT(BlueprintType)
/* Contains damage OR healing values and their ownership. */
struct FBattleDamage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* The ability that is responsible for this damage / healing. Setting this to null will force a damage multiplier of 1 always for any time duration. */
		UAbility* Ability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Total amount remaining to damage or heal. */
		float Amount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Amount of time in seconds left until this is deleted. */
		float RemainingTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* The total amount of time in seconds that this damage / healing occurs for. */
		float TotalTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Whether this instance should be deleted. */
		bool Garbage;
};