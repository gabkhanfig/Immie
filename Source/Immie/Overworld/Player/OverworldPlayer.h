// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Battler.h"
#include "OverworldPlayer.generated.h"

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

	UFUNCTION(BlueprintCallable)
		/* Called when any of this Overworld Player's colliders have begun collision with another actor. */
		void OnCollision(UPrimitiveComponent* ThisOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent);

	UFUNCTION(BlueprintNativeEvent)
		/**/
		void TrainerCollision(const TScriptInterface<IBattler>& Trainer, UPrimitiveComponent* ThisOverlappedComponent, UPrimitiveComponent* OtherActorComponent);
	
	/* Battler interface. */
	virtual EBattleTeamType GetBattleTeamType_Implementation() const override;
	virtual TArray<UImmie*> GetTeam_Implementation() const override;
	virtual APawn* GetPawn_Implementation() const override;
	virtual FBattleTeamInit GetBattleTeamInit_Implementation() const override;
	//virtual void Disable_Implementation() override;
	//virtual void Enable_Implementation() override;
	virtual void OnBattleStart_Implementation() override;
	virtual void OnBattleEnd_Implementation() override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void YawInput(float AxisValue);
	void PitchInput(float AxisValue);
	void ForwardMovement(float ScaleValue);
	void RightMovement(float ScaleValue);

protected:

	UPROPERTY(BlueprintReadWrite)
		/**/
		float TimerForBattleReady;

};
