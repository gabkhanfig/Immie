// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecieDataManager.h"
#include <Immie/Immies/ImmieObject.h>
#include <Immie/Immies/ImmieCharacter.h>
#include <Immie/Immies/ImmieSpawnData.h>
#include <Immie/Immies/SpecieDataObject.h>
#include <Immie/Type/ImmieType.h>



static TArray<FName> _AllSpecieNames() {
	TArray<FName> SpecieNames;
	SpecieNames.Add("Snamdon");
	SpecieNames.Add("Arborea");
	return SpecieNames;
}

static TSet<FName> _SpecieNamesSet() {
	const TArray<FName> SpecieNames = _AllSpecieNames();
	TSet<FName> NamesSet;
	for (FName Specie : SpecieNames) {
		NamesSet.Add(Specie);
	}
	return NamesSet;
}

static TSet<FString> SetOfSpecieStringNames(const TArray<FName>& Names) {
	TSet<FString> Set;
	for (FName Name : Names) {
		Set.Add(Name.ToString());
	}
	return Set;
}

const TArray<FName> USpecieDataManager::SpecieNames = _AllSpecieNames();
const TSet<FName> USpecieDataManager::SpecieNamesSet = _SpecieNamesSet();
const TSet<FString> USpecieDataManager::SpecieStringNames = SetOfSpecieStringNames(SpecieNames);

TArray<FName> USpecieDataManager::GetAllSpecieNames() {
	return SpecieNames;
}

bool USpecieDataManager::IsValidSpecieName(FName SpecieName)
{
	return SpecieNamesSet.Contains(SpecieName);
}

bool USpecieDataManager::IsValidSpecieNameString(const FString& SpecieStringName)
{
	return SpecieStringNames.Contains(SpecieStringName);
}

TSet<FName> USpecieDataManager::GetSetOfSpecieNames()
{
	return SpecieNamesSet;
}

void USpecieDataManager::SetSpecieNamesAndIds()
{
	//SpecieNames.Empty();
	//SpecieNames = AllSpecieNames();
}

void USpecieDataManager::LoadDefaultGameData()
{
	SetSpecieNamesAndIds();

	RegisterSpeciesFromDisk(this, "Default", &Species);
}

FString USpecieDataManager::LoadSpecieJsonFileToString(FName SpecieName, const FString& FolderName)
{
	const FString JsonPath = GetGameDataDir() + FolderName + "/Species/" + SpecieName.ToString() + ".json"; 
	if (!FPaths::FileExists(JsonPath)) {
		iLog("File does not exist at path: " + JsonPath);
		return FString();
	}

	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *JsonPath);
	return JsonString;
}

TSubclassOf<USpecieDataObject> USpecieDataManager::LoadSpecieDataObjectClass(FName SpecieName)
{
	const FString SpecieString = SpecieName.ToString();
	const FString DataObjectClassReferenceString = USpecieDataObject::GetImmiesBlueprintFolder() + SpecieString + "/BP_" + SpecieString + "DataObject.BP_" + SpecieString + "DataObject_C'";
	TSubclassOf<USpecieDataObject> DataObjectClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *DataObjectClassReferenceString));

	if (!IsValid(DataObjectClass)) {
		iLog("Data object UClass for specie " + SpecieName.ToString() + " is not valid.", LogVerbosity_Error);
		return nullptr;
	}

	return DataObjectClass;
}

USpecieDataObject* USpecieDataManager::RegisterSpecie(UObject* Outer, FName SpecieName, const FString& JsonString, bool LoadLearnsets)
{
	TSubclassOf<USpecieDataObject> DataObjectClass = LoadSpecieDataObjectClass(SpecieName);

	FJsonObjectBP SpecieJson;
	if (!FJsonObjectBP::LoadJsonString(JsonString, SpecieJson)) {
		iLog("Failed to load json data for specie " + SpecieName.ToString(), LogVerbosity_Error);
		return nullptr;
	}

	USpecieDataObject* SpecieDataObject = USpecieDataObject::CreateSpecieDataObject(Outer, DataObjectClass);
	SpecieDataObject->LoadSpecieJsonData(SpecieJson, LoadLearnsets);
	return SpecieDataObject;
}

void USpecieDataManager::RegisterSpeciesFromDisk(UObject* Outer, const FString& FolderName, TMap<FName, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets)
{
	SpeciesOut->Empty();

	for (int i = 0; i < SpecieNames.Num(); i++) {
		FString JsonString = LoadSpecieJsonFileToString(SpecieNames[i], FolderName);
		SpeciesOut->Add(
			SpecieNames[i],
			RegisterSpecie(Outer, SpecieNames[i], JsonString, LoadLearnsets)
		); 
		ULogger::Log("Loaded specie data object from disk with json data [" + SpecieNames[i].ToString() + "]");
	}
}

void USpecieDataManager::RegisterSpeciesFromSerialized(UObject* Outer, const TArray<FSerializedSpecieData>& SpecieData, TMap<FName, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets)
{
	SpeciesOut->Empty();

	for (int i = 0; i < SpecieData.Num(); i++) {
		SpeciesOut->Add(
			SpecieData[i].SpecieName,
			RegisterSpecie(Outer, SpecieData[i].SpecieName, SpecieData[i].JsonString, LoadLearnsets)
		);
		ULogger::Log("Loaded specie data object from serialized json data [" + SpecieData[i].SpecieName.ToString() + "]");
	}
}

USpecieDataObject* USpecieDataManager::GetSpecieDataObjectFromMap(TMap<FName, USpecieDataObject*>& Map, FName SpecieName)
{
	USpecieDataObject** Found = Map.Find(SpecieName);
	if (Found) {
		return *Found;
	}

	iLog("Could not find specie data object for specie id " + SpecieName.ToString(), LogVerbosity_Error);
	return nullptr;
}

USpecieDataObject* USpecieDataManager::GetSpecieDataObject(FName SpecieName)
{
	return GetSpecieDataObjectFromMap(Species, SpecieName);
}

FName USpecieDataManager::SpecieNameFromBlueprintClassName(const FString ClassName, FString RightChop)
{
	const FString BlueprintIdentifier = "BP_";
	if (!ClassName.Contains(BlueprintIdentifier)) {
		iLog("[USpecieDataManager Specie Name From Blueprint Class Name]: Blueprint class identifier of BP_ not found in class name " + ClassName, LogVerbosity_Error);
		return FName();
	}

	const uint32 Start = 3;
	const uint32 End = ClassName.Find(RightChop);
	if (End == INDEX_NONE) {
		iLog("[USpecieDataManager Specie Name From Blueprint Class Name]: Unable to find right chop string of " + RightChop + " in specie blueprint class name " + ClassName, LogVerbosity_Error);
		return FName();
	}

	const FString FoundSpecieName = ClassName.Mid(Start, End - Start);
	if (!IsValidSpecieNameString(FoundSpecieName)) {
		iLog("[USpecieDataManager Specie Name From Blueprint Class Name]: Found specie name of " + FoundSpecieName + " from blueprint class name " + ClassName + " is not a valid specie", LogVerbosity_Error);
		return FName();
	}
	return FName(FoundSpecieName);
}


