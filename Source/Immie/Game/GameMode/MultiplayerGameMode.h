// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Immie/Game/ImmieGameMode.h>
#include "MultiplayerGameMode.generated.h"

class AImmiePlayerController;
class ABattleInstance;

USTRUCT(BlueprintType)
struct FMultiplayerTeamContainer 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		AImmiePlayerController* Controller;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		FString TeamJsonString;
};

/**
 * 
 */
UCLASS()
class IMMIE_API AMultiplayerGameMode : public AImmieGameMode
{
	GENERATED_BODY()

private:

	/* Storing the players team for battle */
	TArray<FMultiplayerTeamContainer> PlayerTeams;

	ABattleInstance* Battle;

protected:

	virtual void PostLogin(APlayerController* NewPlayer) override;

public:

	/* Adds a player to the battle that will happen. */
	void AddPlayerToBattle(AImmiePlayerController* Player, const FString& TeamJsonString);

	/* Is everything ready for battle? */
	bool IsReadyForBattle();

	/* Starts the multiplayer battle, including subsequently validating the data. */
	void StartMultiplayerBattle();



};
