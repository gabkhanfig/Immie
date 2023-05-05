// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildImmieSpawner.generated.h"

class UWildSpawnTable;
class UImmieSpawnData;

/* Actor that spawns in wild immies around it. */
UCLASS()
class IMMIE_API AWildImmieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWildImmieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		/**/
		FName SpawnTable;

};
