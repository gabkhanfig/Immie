// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Immie/Game/ImmieGameMode.h>
#include "SingleplayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class IMMIE_API ASingleplayerGameMode : public AImmieGameMode
{
	GENERATED_BODY()

	float Timer;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
	
};
