// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleplayerGameMode.h"

void ASingleplayerGameMode::BeginPlay()
{
	Super::BeginPlay();
	Timer = 5;
}

void ASingleplayerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Timer > 0) {
		Timer -= DeltaTime;
		if (Timer <= 0) {
			StartSingleplayerBattle();
		}
	}
}
