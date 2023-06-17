// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDataManager.h"
#include <Immie/Ability/Ability.h>
#include <Immie/Ability/AbilityActor.h>
#include <Immie/Ability/DummyAbilityActor.h>
#include <Immie/Ability/AbilityDataObject.h>

static TArray<FName> _AllAbilityNames() {
	TArray<FName> AbilityNames;
	AbilityNames.Add("EmptyAbility");
	AbilityNames.Add("Fireball");
	AbilityNames.Add("TerraSpike");
	AbilityNames.Add("VoltBolt");
	AbilityNames.Add("SunshineBow");
	AbilityNames.Add("LeafVolley");
	return AbilityNames;
}

static TSet<FName> _AbilityNamesSet() {
	const TArray<FName> AbilityNames = _AllAbilityNames();
	TSet<FName> NamesSet;
	for (FName Ability : AbilityNames) {
		NamesSet.Add(Ability);
	}
	return NamesSet;
}

const TArray<FName> UAbilityDataManager::AbilityNames = _AllAbilityNames();
const TSet<FName> UAbilityDataManager::AbilityNamesSet = _AbilityNamesSet();

TArray<FName> UAbilityDataManager::GetAllAbilityNames() {
	return AbilityNames;
}

bool UAbilityDataManager::IsValidAbilityName(FName AbilityName)
{
	return AbilityNamesSet.Contains(AbilityName);
}

TSet<FName> UAbilityDataManager::GetSetOfAbilityNames()
{
	return AbilityNamesSet;
}

void UAbilityDataManager::SetAbilityNamesAndIds()
{
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
	const FString AbilityString = AbilityName.ToString(); 
	const FString DataObjectClassReferenceString = UAbilityDataObject::GetAbilitiesBlueprintFolder() + AbilityString + "/BP_" + AbilityString + "DataObject.BP_" + AbilityString + "DataObject_C'";
	TSubclassOf<UAbilityDataObject> DataObjectClass = StaticLoadClass(UAbilityDataObject::StaticClass(), NULL, *DataObjectClassReferenceString);

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

FName UAbilityDataManager::AbilityNameFromBlueprintClassName(const FString ClassName, FString RightChop)
{
	const FString BlueprintIdentifier = "BP_";
	if (!ClassName.Contains(BlueprintIdentifier)) {
		iLog("[UAbilityDataManager Ability Name From Blueprint Class Name]: Blueprint class identifier of BP_ not found in class name " + ClassName, LogVerbosity_Error);
		return FName();
	}

	const uint32 Start = 3;
	const uint32 End = ClassName.Find(RightChop);
	if (End == INDEX_NONE) {
		iLog("[UAbilityDataManager Ability  Name From Blueprint Class Name]: Unable to find right chop string of " + RightChop + " in ability blueprint class name " + ClassName, LogVerbosity_Error);
		return FName();
	}
	const FName FoundAbilityName = FName(ClassName.Mid(Start, End - Start));
	if (!IsValidAbilityName(FoundAbilityName)) {
		iLog("[UAbilityDataManager Ability  Name From Blueprint Class Name]: Found ability name of " + FoundAbilityName.ToString() + " from blueprint class name " + ClassName + " is not a valid ability", LogVerbosity_Error);
		return FName();
	}
	return FoundAbilityName;
}
