// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../Battle/BattleStructures.h"
#include "Battler.generated.h"

class UImmie;
class ABattleTeam;

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UBattler : public UInterface
{
	GENERATED_BODY()
};

/* Interface for any class that can "own" a team. These include the overworld player, trainers, and wild Immies. 
Only used by APawns. */
class IMMIE_API IBattler
{
	GENERATED_BODY()

protected:

	void DisablePawn();

	void EnablePawn();

	FBattleTeamInit DefaultBattleTeamInit() const;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		TArray<UImmie*> GetBattlerTeam() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		APawn* GetPawn() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		FBattleTeamInit GetBattleTeamInit() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		TSubclassOf<ABattleTeam> GetBattleTeamClass() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/**/
		void OnBattleStart();// { OnBattleStart(); };
	//virtual void OnBattleStart();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/**/
		void OnBattleEnd(EBattleTeamWinState WinState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/**/
		bool CanBeBattled() const;

};
