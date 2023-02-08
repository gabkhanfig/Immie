// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../Battle/BattleStructures.h"
#include "Trainer.generated.h"

class UImmie;


// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UTrainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IMMIE_API ITrainer
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

	UFUNCTION(BlueprintPure, DisplayName = "Get Battle Team Init")
		/**/
		virtual FBattleTeamInit BattleActorGetBattleTeamInit() const { return GetBattleTeamInit(); }
	virtual FBattleTeamInit GetBattleTeamInit() const;

};
