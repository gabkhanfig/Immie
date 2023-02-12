// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../Battle/BattleStructures.h"
#include "Battler.generated.h"

class UImmie;


// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UBattler : public UInterface
{
	GENERATED_BODY()
};

/* Interface for any class that can "own" a team. These include the player, trainers, and wild Immies. */
class IMMIE_API IBattler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintPure, DisplayName = "Get Battle Team Type")
		/**/
		virtual EBattleTeamType BattleActorGetBattleTeamType() const { return GetBattleTeamType(); }
	virtual EBattleTeamType GetBattleTeamType() const = 0; 

	UFUNCTION(BlueprintPure, DisplayName = "Get Team")
		/**/
		virtual TArray<UImmie*> BattleActorGetTeam() const { return GetTeam(); }
	virtual TArray<UImmie*> GetTeam() const = 0;

	UFUNCTION(BlueprintPure)
		/**/
		virtual APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, DisplayName = "Get Battle Team Init")
		/**/
		virtual FBattleTeamInit BattleActorGetBattleTeamInit() const { return GetBattleTeamInit(); }
	virtual FBattleTeamInit GetBattleTeamInit() const;

	UFUNCTION(BlueprintCallable, DisplayName = "Disable")
		/**/
		virtual void BattleActorDisable() { Disable(); };
	virtual void Disable();

	UFUNCTION(BlueprintCallable, DisplayName = "Enable")
		/**/
		virtual void BattleActorEnable() { Enable(); }
	virtual void Enable();

	UFUNCTION(BlueprintCallable)
		/**/
		virtual void BattleActorOnBattleStart() { OnBattleStart(); };
	virtual void OnBattleStart();

	UFUNCTION(BlueprintCallable)
		/**/
		virtual void BattleActorOnBattleEnd() { OnBattleEnd(); };
	virtual void OnBattleEnd();

};
