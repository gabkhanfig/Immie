// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include <Immie/Game/Global/Managers/TypeDataManager.h>
#include <Immie/Game/Global/Managers/AbilityDataManager.h>
#include <Immie/Game/Global/Managers/SpecieDataManager.h>
#include <Immie/Game/Global/Managers/BattleDataManager.h>
#include <Immie/Game/Global/Managers/ConfigDataManager.h>
#include <Immie/Game/Global/Managers/TrainerDataManager.h>
#include <Immie/Game/Global/Managers/SpawnTableManager.h>
#include <Immie/Game/Player/PlayerImmies.h>
#include <Immie/Game/Save/SaveManager.h>
#include <Immie/Type/ImmieType.h>
#include <Immie/Immies/ImmieObject.h>
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Game/ImmieGameMode.h>

void UImmieGameInstance::InitializeGameDataManagers()
{
	TypeDataManager = NewObject<UTypeDataManager>(this);
	AbilityDataManager = NewObject<UAbilityDataManager>(this);
	SpecieDataManager = NewObject<USpecieDataManager>(this);
	BattleDataManager = NewObject<UBattleDataManager>(this);
	PlayerImmies = NewObject<UPlayerImmies>(this);
	SaveManager = NewObject<USaveManager>(this);
	ConfigDataManager = NewObject<UConfigDataManager>(this);
	TrainerDataManager = NewObject<UTrainerDataManager>(this);
	SpawnTableManager = NewObject<USpawnTableManager>(this);

	ConfigDataManager->Load();
	TypeDataManager->Load();
	AbilityDataManager->Load();
	SpecieDataManager->Load();
	BattleDataManager->Load();
	TrainerDataManager->Load();
	SpawnTableManager->Load();
}

void UImmieGameInstance::Init()
{
	Super::Init();

	_ImmieGameInstance = this;
	InitializeGameDataManagers();

	SaveManager->LoadSave("TestSave");


	BP_ImmieGameInit();
}
