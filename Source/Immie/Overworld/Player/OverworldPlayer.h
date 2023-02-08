// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Trainer.h"
#include "OverworldPlayer.generated.h"

UCLASS()
class IMMIE_API AOverworldPlayer : public ACharacter, public ITrainer
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
		void TrainerCollision(const TScriptInterface<ITrainer>& Trainer, UPrimitiveComponent* ThisOverlappedComponent, UPrimitiveComponent* OtherActorComponent);
	
	/* Trainer interface. */

	virtual EBattleTeamType GetBattleTeamType() const override;
	virtual TArray<UImmie*> GetTeam() const override;
	virtual FBattleTeamInit GetBattleTeamInit() const override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void YawInput(float AxisValue);
	void PitchInput(float AxisValue);
	void ForwardMovement(float ScaleValue);
	void RightMovement(float ScaleValue);

};
