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

	TMap<FName, UTrainerDataObject*> Trainers;

public:

private:

	virtual void LoadDefaultGameData() override;
	
};
