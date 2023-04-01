// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDataManager.h"
#include <Immie/Ability/Ability.h>
#include <Immie/Ability/AbilityDataObject.h>

void UAbilityDataManager::SetAbilityNamesAndIds()
{
	AbilityNames.Empty();
	AbilityIds.Empty();

	int Index = -1;

#define DefineAbilityName(AbilityName) \
	Index = AbilityNames.Add(AbilityName);\
	AbilityIds.Add(AbilityName, Index)

	//DefineAbilityName("testAbility");
	DefineAbilityName("emptyAbility");
	DefineAbilityName("fireball");
	DefineAbilityName("terraSpike");
	DefineAbilityName("voltBolt");
	DefineAbilityName("sunshineBow");
	//DefineAbilityName("");
}

void UAbilityDataManager::LoadDefaultGameData()
{
	SetAbilityNamesAndIds();

	RegisterAbilitiesFromDisk(this, "Default", &Abilities);
}

FString UAbilityDataManager::LoadAbilityJsonFileToString(FName AbilityName, const FString& FolderName)
{
	const FString JsonPath = GetGameDataDir() + FolderName + "/Abilities/" + AbilityName.ToString() + ".json";
	if (!FPaths::FileExists(JsonPath)) {
		iLog("File does not exist at path: " + JsonPath);
		return FString();
	}

	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *JsonPath);
	return JsonString;
}

TSubclassOf<UAbilityDataObject> UAbilityDataManager::LoadAbilityDataObjectClass(FName AbilityName)
{
	const FString AbilityCapitalizedName = UStringUtils::ToUpperFirstLetter(AbilityName.ToString());
	const FString DataObjectClassReferenceString = UAbilityDataObject::GetAbilitiesBlueprintFolder() + AbilityCapitalizedName + "/BP_" + AbilityCapitalizedName + "DataObject.BP_" + AbilityCapitalizedName + "DataObject_C'";
	TSubclassOf<UAbilityDataObject> DataObjectClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *DataObjectClassReferenceString));

	if (!IsValid(DataObjectClass)) {
		iLog("Data object UClass for ability " + AbilityName.ToString() + " is not valid.", LogVerbosity_Error);
		return nullptr;
	}

	return DataObjectClass;
}

UAbilityDataObject* UAbilityDataManager::RegisterAbility(UObject* Outer, FName AbilityName, int AbilityId, const FString& JsonString)
{
	TSubclassOf<UAbilityDataObject> DataObjectClass = LoadAbilityDataObjectClass(AbilityName);

	FJsonObjectBP AbilityJson;
	if (!FJsonObjectBP::LoadJsonString(JsonString, AbilityJson)) {
		iLog("Failed to load json data for ability " + AbilityName.ToString(), LogVerbosity_Error);
		return nullptr;
	}

	UAbilityDataObject* AbilityDataObject = UAbilityDataObject::CreateAbilityDataObject(Outer, AbilityId, DataObjectClass);
	AbilityDataObject->LoadAbilityJsonData(AbilityJson);
	return AbilityDataObject;
}

void UAbilityDataManager::RegisterAbilitiesFromDisk(UObject* Outer, const FString& FolderName, TMap<int, UAbilityDataObject*>* AbilitiesOut)
{
	AbilitiesOut->Empty();

	for (int i = 0; i < AbilityNames.Num(); i++) {
		FString JsonString = LoadAbilityJsonFileToString(AbilityNames[i], FolderName);
		const int AbilityId = *AbilityIds.Find(AbilityNames[i]);
		AbilitiesOut->Add(
			AbilityId,
			RegisterAbility(Outer, AbilityNames[i], AbilityId, JsonString));
#ifdef DEVELOPMENT
		ULogger::Log("Loaded ability data object from disk with json data [" + AbilityNames[i].ToString() + "] using internal ability id: " + FString::FromInt(AbilityId));
#else
		ULogger::Log("Loaded ability data object from disk with json data [" + AbilityNames[i].ToString() + "]");
#endif
	}
}

void UAbilityDataManager::RegisterAbilitiesFromSerialized(UObject* Outer, const TArray<FSerializedAbilityData>& AbilityData, TMap<int, UAbilityDataObject*>* AbilitiesOut)
{
	AbilitiesOut->Empty();

	for (int i = 0; i < AbilityData.Num(); i++) {
		AbilitiesOut->Add(
			AbilityData[i].AbilityId,
			RegisterAbility(Outer, AbilityData[i].AbilityName, AbilityData[i].AbilityId, AbilityData[i].JsonString));
#ifdef DEVELOPMENT
		ULogger::Log("Loaded ability data object from serialized json data [" + AbilityData[i].AbilityName.ToString() + "] using internal ability id: " + FString::FromInt(AbilityData[i].AbilityId));
#else
		ULogger::Log("Loaded ability data object from serialized json data [" + AbilityData[i].AbilityName.ToString() + "]");
#endif
	}
}

UAbilityDataObject* UAbilityDataManager::GetAbilityDataObjectFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId)
{
	UAbilityDataObject** Found = Map.Find(AbilityId);
	if (Found) {
		return *Found;
	}

	iLog("Unable to find ability data object for ability id " + FString::FromInt(AbilityId), LogVerbosity_Error);
	return nullptr;
}

UAbilityDataObject* UAbilityDataManager::GetAbilityDataObject(int AbilityId)
{
	return GetAbilityDataObjectFromMap(Abilities, AbilityId);
}

int UAbilityDataManager::GetAbilityIdFromMap(TMap<FName, int>& Map, FName AbilityName)
{
	int* Found = Map.Find(AbilityName);
	if (Found) {
		return *Found;
	}

	iLog("Unable to find ability id for " + AbilityName.ToString(), LogVerbosity_Error);
	return INVALID_ABILITY_ID;
}

int UAbilityDataManager::GetAbilityId(FName AbilityName)
{
	return GetAbilityIdFromMap(AbilityIds, AbilityName);
}

FName UAbilityDataManager::GetAbilityNameFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId)
{
	UAbilityDataObject** Found = Map.Find(AbilityId);
	if (Found) {
		return (*Found)->GetAbilityName();
	}

	iLog("Unable to find name for ability id " + FString::FromInt(AbilityId), LogVerbosity_Error);
	return FName();
}

FName UAbilityDataManager::GetAbilityName(int AbilityId)
{
	return GetAbilityNameFromMap(Abilities, AbilityId);
}

bool UAbilityDataManager::IsValidAbilityFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId)
{
	return IsValid(*Map.Find(AbilityId));
}

bool UAbilityDataManager::IsValidAbility(int AbilityId)
{
	return IsValidAbilityFromMap(Abilities, AbilityId);
}

UClass* UAbilityDataManager::GetAbilityClassFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId)
{
	UAbilityDataObject** Found = Map.Find(AbilityId);
	if (Found) {
		return (*Found)->GetAbilityClass();
	}

	iLog("Unable to find ability class for ability id " + FString::FromInt(AbilityId), LogVerbosity_Error);
	return nullptr;
}

UClass* UAbilityDataManager::GetAbilityClass(int AbilityId)
{
	return GetAbilityClassFromMap(Abilities, AbilityId);
}

UClass* UAbilityDataManager::GetActorClassFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId)
{
	UAbilityDataObject** Found = Map.Find(AbilityId);
	if (Found) {
		return (*Found)->GetActorClass();
	}

	iLog("Unable to find actor class for ability id " + FString::FromInt(AbilityId), LogVerbosity_Error);
	return nullptr;
}

UClass* UAbilityDataManager::GetActorClass(int AbilityId)
{
	return GetActorClassFromMap(Abilities, AbilityId);
}
