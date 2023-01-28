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
	RegisterBattleInstanceClass("Singleplayer", "BP_SingleplayerBattleInstance", MapOut);
	RegisterBattleInstanceClass("Multiplayer", "BP_MultiplayerBattleInstance", MapOut);
}

void UBattleDataManager::RegisterTeamClasses(TMap<TEnumAsByte<EBattleTeamType>, UClass*>* MapOut)
{
	RegisterTeamClass(EBattleTeamType::BattleTeam_PlayerSingleplayer, "BP_PlayerSingleplayerBattleTeam", MapOut);
	RegisterTeamClass(EBattleTeamType::BattleTeam_PlayerMultiplayer, "BP_PlayerMultiplayerBattleTeam", MapOut);
	RegisterTeamClass(EBattleTeamType::BattleTeam_Wild, "BP_WildBattleTeam", MapOut);
	RegisterTeamClass(EBattleTeamType::BattleTeam_Trainer, "BP_TrainerBattleTeam", MapOut);
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

void UBattleDataManager::RegisterTeamClass(TEnumAsByte<EBattleTeamType> TeamType, const FString& ClassBlueprintName, TMap<TEnumAsByte<EBattleTeamType>, UClass*>* MapOut)
{
	const FString TeamsBlueprintFolder = "/Script/Engine.Blueprint'/Game/Battle/Team/";
	const FString TeamClassReferenceString = TeamsBlueprintFolder + ClassBlueprintName + "." + ClassBlueprintName + "_C'";
	UClass* TeamClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *TeamClassReferenceString));

	const bool ValidClass = IsValid(TeamClass);

	if (!ValidClass) {
		iLog("Invalid UClass for battle team name " + ClassBlueprintName, LogVerbosity_Error);
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

	iLog("Unable to find battle instance UClass for name " + Name.ToString(), LogVerbosity_Error);
	return nullptr;
}

UClass* UBattleDataManager::GetTeamClass(TEnumAsByte<EBattleTeamType> TeamType)
{
	UClass** Found = TeamClasses.Find(TeamType);
	if (Found) {
		return *Found;
	}

	iLog("Unable to find team UClass for battle team type " + FString::FromInt(TeamType), LogVerbosity_Error);
	return nullptr;
}
