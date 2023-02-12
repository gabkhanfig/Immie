// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Interfaces/Battler.h"
#include "TrainerPawn.generated.h"

UCLASS()
class IMMIE_API ATrainerPawn : public APawn, public IBattler
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		TEnumAsByte<EBattleTeamType> BattleTeamType;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<UImmie*> Team;

public:
	// Sets default values for this pawn's properties
	ATrainerPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Battler interface. */

	virtual EBattleTeamType GetBattleTeamType() const override;
	virtual TArray<UImmie*> GetTeam() const override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
