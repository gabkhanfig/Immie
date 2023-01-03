// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ImmiePlayerController.generated.h"

class ABattleInstance;
class AImmieGameMode;

/**
 * 
 */
UCLASS()
class IMMIE_API AImmiePlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/**/
		AImmieGameMode* GameMode;

public:

	UFUNCTION(BlueprintCallable)
		/**/
		void SetGameMode(AImmieGameMode* _GameMode) { GameMode = _GameMode; }

	UFUNCTION(Client, Reliable)
		/* Called from server, executes on client.
		Sets the clients team index within a particular battle instance. */
		void SetClientBattleTeamIndex(ABattleInstance* BattleInstance, int NewClientTeam);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		/* Called from server, executes on client. 
		Requests that the client serialize and send it's team data that it will use for battle. */
		void RequestClientPlayerTeam();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
		/* Called from client, executes on server.
		Sends the serialized team data to the server. */
		void SendPlayerTeamToServer(const FString& TeamJsonString);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		/**/
		void InformBattleInstanceClientSpawned(ABattleInstance* BattleInstance);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		/**/
		void InformBattleInstanceClientValidManagers(ABattleInstance* BattleInstance);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		/**/
		void InformBattleInstanceClientValidTeams(ABattleInstance* BattleInstance);

};
