// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Immie/Game/ImmieGameMode.h>
#include <Immie/Battle/BattleStructures.h>
#include "MultiplayerGameMode.generated.h"

class AImmiePlayerController;
class ABattleInstance;
class ABattleTeam;

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

	AMultiplayerGameMode();

	/* Adds a player to the battle that will happen. */
	void AddPlayerToBattle(AImmiePlayerController* Player, const FString& TeamJsonString);

	/* Is everything ready for battle? */
	bool IsReadyForBattle();

	/**/
	void ForceStartMultiplayerBattle(AImmiePlayerController* Player);

private:

	FBattleTeamInit GenerateTestAiTeam() const;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		TSubclassOf<ABattleTeam> PlayerTeamClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		TSubclassOf<ABattleTeam> AiTeamClass;

};
