// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Interfaces/Battler.h"
#include "TrainerPawn.generated.h"

class UTrainerDataObject;
class ABattleTeam;

UCLASS()
class IMMIE_API ATrainerPawn : public APawn, public IBattler
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		FName TrainerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		TEnumAsByte<EBattleTeamType> BattleTeamType;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<UImmie*> Team;

	UPROPERTY(BlueprintReadWrite)
		bool CanOverworldPlayerInitiateBattle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		TSubclassOf<ABattleTeam> BattleTeamClass;

public:
	// Sets default values for this pawn's properties
	ATrainerPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Battler interface. */
	virtual EBattleTeamType GetBattleTeamType_Implementation() const override;
	virtual TArray<UImmie*> GetTeam_Implementation() const override;
	virtual APawn* GetPawn_Implementation() const override;
	virtual FBattleTeamInit GetBattleTeamInit_Implementation() const override;
	//virtual void Disable_Implementation() override;
	//virtual void Enable_Implementation() override;
	virtual void OnBattleStart_Implementation() override;
	virtual void OnBattleEnd_Implementation(EBattleTeamWinState WinState) override;
	virtual bool CanBeBattled_Implementation() const override;

	bool GetCanOverworldPlayerInitiateBattle() const { return CanOverworldPlayerInitiateBattle; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
		/**/
		UTrainerDataObject* GetTrainerDataObject();

	void ConstructTeam();

};
