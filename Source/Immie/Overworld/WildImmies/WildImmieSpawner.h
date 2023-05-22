// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildImmieSpawner.generated.h"

class UWildSpawnTable;
class UImmieSpawnData;
class USpawnTableManager;
class AImmieCharacter;
class UImmie;

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

	UFUNCTION(BlueprintCallable)
		/**/
		void StartTrackingWildImmie(AImmieCharacter* ImmieCharacter);

	UFUNCTION(BlueprintCallable)
		/**/
		void StopTrackingWildImmie(AImmieCharacter* ImmieCharacter);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		/* Already added to the SpawnedImmies array. */
		AImmieCharacter* SpawnWildImmie(UImmieSpawnData* SpawnData, FVector Location);


protected:	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (NoResetToDefault))
		/* Cannot be "None". If it is, the program will be aborted. */
		FName SpawnTableName;

	UPROPERTY(BlueprintReadOnly)
		/* Set before blueprint begin play. */
		UWildSpawnTable* SpawnTable;

	UPROPERTY(BlueprintReadWrite)
		/**/
		TArray<AImmieCharacter*> SpawnedImmies;

};
