// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Battle/BattleStructures.h"
#include "ImmieGameMode.generated.h"

class ABattleInstance;

/**
 * 
 */
UCLASS()
class IMMIE_API AImmieGameMode : public AGameModeBase
{
	GENERATED_BODY()

		

protected:

	virtual void BeginPlay() override;

public:

	AImmieGameMode();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		/**/
		void StartSingleplayerBattle();

	UFUNCTION(BlueprintCallable)
		/* Creates a new battle instance and calls it's BattleInit(). */
		ABattleInstance* StartBattle(FName BattleInstanceName, const TArray<FBattleTeamInit>& Teams, FVector BattleLocation);
	
};
