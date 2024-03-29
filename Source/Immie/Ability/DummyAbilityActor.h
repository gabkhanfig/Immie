// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyAbilityActor.generated.h"

class AAbilityActor;

UCLASS()
/* Dummy class which is used for visuals, movement, etc client side. */
class IMMIE_API ADummyAbilityActor : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/**/
		FName AbilityName;

	UPROPERTY(BlueprintReadOnly)
		/* The actual ability actor that has battle relevance. */
		AAbilityActor* AbilityActor;

	UPROPERTY(BlueprintReadWrite)
		/**/
		class UMovementComponent* MovementComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		class UProjectileMovementComponent* ProjectileComponent;

public:	
	// Sets default values for this actor's properties
	ADummyAbilityActor();

	void PostLoad() override;

protected:
	
	/* AbilityActor can be assumed to be valid at this point. */
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
		/* Called when the actual ability actor is being destroyed, not necessarily this dummy actor. */
		void BP_OnAbilityActorDestroy();

public:	

	/**/
	void SetAbilityActor(AAbilityActor* _AbilityActor);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Called when the actual ability actor is being destroyed, not this one (yet). */
	void OnAbilityActorDestroy();

};
