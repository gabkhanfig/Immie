// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataManager.h"
#include "TrainerDataManager.generated.h"

class UTrainerDataObject;

/**
 * 
 */
UCLASS()
class IMMIE_API UTrainerDataManager : public UGameDataManager
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/**/
		TMap<FName, UTrainerDataObject*> Trainers;

public:

	UFUNCTION(BlueprintPure)
		/**/
		UTrainerDataObject* GetTrainerDataObject(FName TrainerName);

private:

	virtual void LoadDefaultGameData() override;

	void RegisterTrainers(UObject* Outer, TMap<FName, UTrainerDataObject*>* MapOut);

	void RegisterTrainer(UObject* Outer, FName TrainerName, TMap<FName, UTrainerDataObject*>* MapOut);

	FString LoadTrainerJsonToString(FName TrainerName);

	TSubclassOf<UTrainerDataObject> GetTrainerDataObjectClass(FName TrainerName);

	UTrainerDataObject* LoadTrainerData(UObject* Outer, FName TrainerName);

	
	
};
