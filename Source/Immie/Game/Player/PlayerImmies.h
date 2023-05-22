// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerImmies.generated.h"

class UImmie;

/**
 * 
 */
UCLASS()
class IMMIE_API UPlayerImmies : public UObject
{
	GENERATED_BODY()

	friend class USaveManager;
	friend class UImmieGameInstance;

private:

	UPROPERTY()
		/**/
		TArray<UImmie*> Team;

	UPROPERTY()
		/**/
		TArray<UImmie*> Chester;

private:



public:

	UPlayerImmies();
	
	bool DoesPlayerTeamHaveSpace();

	UFUNCTION(BlueprintCallable)
		/**/
		void CaptureWildImmie(UImmie* ImmieObject, bool Heal = true);

	UFUNCTION(BlueprintCallable)
		/**/
		bool AddToTeam(UImmie* ImmieObject, bool Heal = true);

	UFUNCTION(BlueprintCallable)
		/**/
		void AddToChester(UImmie* ImmieObject, bool Heal = true);

	UFUNCTION(BlueprintCallable)
		/* Add the Immie to the players team, however if the team is full, add to chester instead. Heals the Immie by default. */
		void AddImmie(UImmie* ImmieObject, bool Heal = true);

	UFUNCTION(BlueprintPure)
		/**/
		TArray<UImmie*> GetTeam() const { return Team; }



	
};
