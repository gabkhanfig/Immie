// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ImmieGameMode.generated.h"

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
	
};
