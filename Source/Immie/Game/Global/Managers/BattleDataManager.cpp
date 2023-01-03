// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleDataManager.h"

void UBattleDataManager::LoadDefaultGameData()
{
	BattleInstanceClasses.Empty();
	TeamClasses.Empty();

	RegisterBattleInstanceClasses(&BattleInstanceClasses);
	RegisterTeamClasses(&TeamClasses);
}

void UBattleDataManager::RegisterBattleInstanceClasses(TMap<FName, UClass*>* MapOut)
{
	RegisterBattleInstanceClass("Singleplayer", "SingleplayerBattleInstance_BP", MapOut);
	RegisterBattleInstanceClass("Multiplayer", "MultiplayerBattleInstance_BP", MapOut);
}

void UBattleDataManager::RegisterBattleInstanceClass(FName Name, const FString& ClassBlueprintName, TMap<FName, UClass*>* MapOut)
{
	const FString BattleInstanceBlueprintFolder = "/Script/Engine.Blueprint'/Game/Battle/BattleInstance/";
	const FString BattleInstanceClassReferenceString = BattleInstanceBlueprintFolder + ClassBlueprintName + "." + ClassBlueprintName + "_C'";
	UClass* Class = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BattleInstanceClassReferenceString));

	const bool ValidClass = IsValid(Class);

	if (!ValidClass) {
		iLog("Invalid UClass for battle instance name " + ClassBlueprintName);
		return;
	}

	//iLog(Class->GetName());
	ULogger::Log("Loaded battle instance class " + ClassBlueprintName);
	MapOut->Add(Name, Class);
}

void UBattleDataManager::RegisterTeamClasses(TMap<TEnumAsByte<EBattleTeamType>, UClass*>* MapOut)
{
	RegisterTeamClass(EBattleTeamType::BattleTeam_PlayerSingleplayer, "PlayerSingleplayerBattleTeam_BP", MapOut);
	RegisterTeamClass(EBattleTeamType::BattleTeam_PlayerMultiplayer, "PlayerMultiplayerBattleTeam_BP", MapOut);
}

void UBattleDataManager::RegisterTeamClass(TEnumAsByte<EBattleTeamType> TeamType, const FString& ClassBlueprintName, TMap<TEnumAsByte<EBattleTeamType>, UClass*>* MapOut)
{
	const FString TeamsBlueprintFolder = "/Script/Engine.Blueprint'/Game/Battle/Team/";
	const FString TeamClassReferenceString = TeamsBlueprintFolder + ClassBlueprintName + "." + ClassBlueprintName + "_C'";
	UClass* TeamClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *TeamClassReferenceString));

	const bool ValidClass = IsValid(TeamClass);

	if (!ValidClass) {
		iLog("Invalid UClass for team name " + ClassBlueprintName);
		return;
	}

	ULogger::Log("Loaded team class " + ClassBlueprintName);
	MapOut->Add(TeamType, TeamClass);
}

UClass* UBattleDataManager::GetBattleInstanceClass(FName Name)
{
	UClass** Found = BattleInstanceClasses.Find(Name);
	if (Found) {
		return *Found;
	}
	else {
		iLog("Unable to find battle instance UClass for name " + Name.ToString());
		return nullptr;
	}
}

UClass* UBattleDataManager::GetTeamClass(TEnumAsByte<EBattleTeamType> TeamType)
{
	UClass** Found = TeamClasses.Find(TeamType);
	if (Found) {
		return *Found;
	}
	else {
		iLog("Unable to find team UClass for team type " + FString::FromInt(TeamType));
		return nullptr;
	}
}
