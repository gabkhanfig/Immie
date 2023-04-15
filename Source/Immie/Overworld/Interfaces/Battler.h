// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../Battle/BattleStructures.h"
#include "Battler.generated.h"

class UImmie;


// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UBattler : public UInterface
{
	GENERATED_BODY()
};

/* Interface for any class that can "own" a team. These include the player, trainers, and wild Immies. 
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
		EBattleTeamType GetBattleTeamType() const;// { return GetBattleTeamType(); }
	//virtual EBattleTeamType GetBattleTeamType() const = 0; 

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		TArray<UImmie*> GetTeam() const;// { return GetTeam(); }
	//virtual TArray<UImmie*> GetTeam() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		APawn* GetPawn() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		/**/
		FBattleTeamInit GetBattleTeamInit() const;// { return GetBattleTeamInit(); }
	//virtual FBattleTeamInit GetBattleTeamInit() const;

	//UFUNCTION(BlueprintCallable)
	//	/**/
	//	void Disable();// { Disable(); };
	////virtual void Disable();

	//UFUNCTION(BlueprintCallable)
	//	/**/
	//	void Enable();// { Enable(); }
	//virtual void Enable();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/**/
		void OnBattleStart();// { OnBattleStart(); };
	//virtual void OnBattleStart();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/**/
		void OnBattleEnd();// { OnBattleEnd(); };
	//virtual void OnBattleEnd();

};
