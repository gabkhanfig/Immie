// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Interfaces/Trainer.h"
#include "TrainerPawn.generated.h"

UCLASS()
class IMMIE_API ATrainerPawn : public APawn, public ITrainer
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual EBattleTeamType GetBattleTeamType() const override;
	virtual TArray<UImmie*> GetTeam() const override;

};
