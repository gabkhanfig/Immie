// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnTableManager.h"
#include "../../../Overworld/WildImmies/WildSpawnTable.h"

void USpawnTableManager::LoadDefaultGameData()
{
	ImmieSpawnTables.Empty();

#define RegisterSpawnTable(TableName) LoadSpawnTableData(TableName, &ImmieSpawnTables)

	RegisterSpawnTable("TestSpawnTable");
}

void USpawnTableManager::LoadSpawnTableData(FName TableName, TMap<FName, UWildSpawnTable*>* MapOut)
{
	const FString JsonString = LoadSpawnTableJsonToString(TableName);
	FJsonObjectBP TableJson;
	if (!FJsonObjectBP::LoadJsonString(JsonString, TableJson)) {
		iLog("Unable to parse spawn table " + TableJson.ToString(), LogVerbosity_Error);
		return;
	}

	UWildSpawnTable* SpawnTableObject = NewObject<UWildSpawnTable>(this);
	SpawnTableObject->LoadJsonData(TableJson);
	MapOut->Add(TableName, SpawnTableObject);
	ULogger::Log("Loaded immie spawn table from disk with json data [" + TableName.ToString() + ']');
}

FString USpawnTableManager::LoadSpawnTableJsonToString(FName TableName)
{
	const FString JsonPath = GetGameDataDir() + "Default/SpawnTables/" + TableName.ToString() + ".json";
	if (!FPaths::FileExists(JsonPath)) {
		iLog("File does not exist at path: " + JsonPath);
		return FString();
	}

	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *JsonPath);
	return JsonString;
}

bool USpawnTableManager::IsValidSpawnTable(FName TableName)
{
	return ImmieSpawnTables.Contains(TableName);
}

UWildSpawnTable* USpawnTableManager::GetImmieSpawnTable(FName TableName)
{
	UWildSpawnTable** Found = ImmieSpawnTables.Find(TableName);
	if (Found) {
		return *Found;
	}

	iLog("Unable to get spawn table for table name: " + TableName.ToString(), LogVerbosity_Error);
	return nullptr;
}
