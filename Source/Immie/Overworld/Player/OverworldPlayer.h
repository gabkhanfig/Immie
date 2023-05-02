// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Battler.h"
#include "OverworldPlayer.generated.h"

class ATrainerPawn;

UCLASS()
class IMMIE_API AOverworldPlayer : public ACharacter, public IBattler
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AOverworldPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
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

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void YawInput(float AxisValue);
	void PitchInput(float AxisValue);
	void ForwardMovement(float ScaleValue);
	void RightMovement(float ScaleValue);

	UFUNCTION(BlueprintCallable)
		/**/
		void AddNearbyBattleEligibleTrainer(ATrainerPawn* Trainer);

	UFUNCTION(BlueprintCallable)
		/**/
		void RemoveNearbyBattleEligibleTrainer(ATrainerPawn* Trainer);

	UFUNCTION(BlueprintCallable)
		/**/
		void StartBattleWithFirstNearbyBattleEligibleTrainer();

	UFUNCTION(BlueprintCallable)
		/**/
		void StartBattleWithBattler(const TScriptInterface<IBattler>& Battler);

protected:

	UPROPERTY(BlueprintReadWrite)
		/**/
		float TimerForBattleReady;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<ATrainerPawn*> NearbyBattleEligibleTrainers;



};
