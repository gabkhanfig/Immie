// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildImmieSpawner.generated.h"

class UWildSpawnTable;
class UImmieSpawnData;
class USpawnTableManager;

/* Actor that spawns in wild immies around it. */
UCLASS()
class IMMIE_API AWildImmieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWildImmieSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (NoResetToDefault))
		/* Cannot be "None". */
		FName SpawnTableName;

	UPROPERTY(BlueprintReadOnly)
		/* Set before blueprint begin play. */
		UWildSpawnTable* SpawnTable;

};
