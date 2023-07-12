// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BattleAiController.generated.h"

class AImmieCharacter;
class ABattleTeam;
class ABattleInstance;

/**
 * 
 */
UCLASS()
class IMMIE_API ABattleAiController : public AAIController
{
	GENERATED_BODY()

public:

	ABattleAiController();

	virtual void Tick(float DeltaTime) override;

	void Initialize(AImmieCharacter* ImmieCharacter);

	UFUNCTION(BlueprintPure)
		AImmieCharacter* GetImmieCharacter() const { return _ImmieCharacter; }

	UFUNCTION(BlueprintPure)
		ABattleTeam* GetBattleTeam() const;

	UFUNCTION(BlueprintPure)
		ABattleInstance* GetBattleInstance() const;

private:

	UPROPERTY()
		/**/
		AImmieCharacter* _ImmieCharacter;

};
