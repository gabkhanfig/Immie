// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDataManager.h"
#include <Immie/Ability/Ability.h>
#include <Immie/Ability/AbilityDataObject.h>

void UAbilityDataManager::SetAbilityNamesAndIds()
{
	AbilityNames.Empty();

#define DefineAbilityName(AbilityName) \
	AbilityNames.Add(AbilityName)

	//DefineAbilityName("testAbility");
	DefineAbilityName("emptyAbility");
	DefineAbilityName("fireball");
	DefineAbilityName("terraSpike");
	DefineAbilityName("voltBolt");
	DefineAbilityName("sunshineBow");
	DefineAbilityName("leafVolley");
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

UAbilityDataObject* UAbilityDataManager::RegisterAbility(UObject* Outer, FName AbilityName, const FString& JsonString)
{
	TSubclassOf<UAbilityDataObject> DataObjectClass = LoadAbilityDataObjectClass(AbilityName);

	FJsonObjectBP AbilityJson;
	if (!FJsonObjectBP::LoadJsonString(JsonString, AbilityJson)) {
		iLog("Failed to load json data for ability " + AbilityName.ToString(), LogVerbosity_Error);
		return nullptr;
	}

	UAbilityDataObject* AbilityDataObject = UAbilityDataObject::CreateAbilityDataObject(Outer, DataObjectClass);
	AbilityDataObject->LoadAbilityJsonData(AbilityJson);
	return AbilityDataObject;
}

void UAbilityDataManager::RegisterAbilitiesFromDisk(UObject* Outer, const FString& FolderName, TMap<FName, UAbilityDataObject*>* AbilitiesOut)
{
	AbilitiesOut->Empty();

	for (int i = 0; i < AbilityNames.Num(); i++) {
		FString JsonString = LoadAbilityJsonFileToString(AbilityNames[i], FolderName);
		AbilitiesOut->Add(
			AbilityNames[i],
			RegisterAbility(Outer, AbilityNames[i], JsonString));
		ULogger::Log("Loaded ability data object from disk with json data [" + AbilityNames[i].ToString() + "]");
	}
}

void UAbilityDataManager::RegisterAbilitiesFromSerialized(UObject* Outer, const TArray<FSerializedAbilityData>& AbilityData, TMap<FName, UAbilityDataObject*>* AbilitiesOut)
{
	AbilitiesOut->Empty();

	for (int i = 0; i < AbilityData.Num(); i++) {
		AbilitiesOut->Add(
			AbilityData[i].AbilityName,
			RegisterAbility(Outer, AbilityData[i].AbilityName, AbilityData[i].JsonString));
		ULogger::Log("Loaded ability data object from serialized json data [" + AbilityData[i].AbilityName.ToString() + "]");
	}
}

UAbilityDataObject* UAbilityDataManager::GetAbilityDataObjectFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName)
{
	UAbilityDataObject** Found = Map.Find(AbilityName);
	if (Found) {
		return *Found;
	}

	iLog("Unable to find ability data object for ability id " + AbilityName.ToString(), LogVerbosity_Error);
	return nullptr;
}

UAbilityDataObject* UAbilityDataManager::GetAbilityDataObject(FName AbilityName)
{
	return GetAbilityDataObjectFromMap(Abilities, AbilityName);
}

bool UAbilityDataManager::IsValidAbilityFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName)
{
	return IsValid(*Map.Find(AbilityName));
}

bool UAbilityDataManager::IsValidAbility(FName AbilityName)
{
	return IsValidAbilityFromMap(Abilities, AbilityName);
}

UClass* UAbilityDataManager::GetAbilityClassFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName)
{
	UAbilityDataObject** Found = Map.Find(AbilityName);
	if (Found) {
		return (*Found)->GetAbilityClass();
	}

	iLog("Unable to find ability class for ability id " + AbilityName.ToString(), LogVerbosity_Error);
	return nullptr;
}

UClass* UAbilityDataManager::GetAbilityClass(FName AbilityName)
{
	return GetAbilityClassFromMap(Abilities, AbilityName);
}

UClass* UAbilityDataManager::GetActorClassFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName)
{
	UAbilityDataObject** Found = Map.Find(AbilityName);
	if (Found) {
		return (*Found)->GetActorClass();
	}

	iLog("Unable to find actor class for ability id " + AbilityName.ToString(), LogVerbosity_Error);
	return nullptr;
}

UClass* UAbilityDataManager::GetActorClass(FName AbilityName)
{
	return GetActorClassFromMap(Abilities, AbilityName);
}
