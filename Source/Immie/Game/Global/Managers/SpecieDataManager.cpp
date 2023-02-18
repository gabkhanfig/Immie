// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecieDataManager.h"
#include <Immie/Immies/SpecieDataObject.h>
#include <Immie/Type/ImmieType.h>

void USpecieDataManager::SetSpecieNamesAndIds()
{
	SpecieNames.Empty();
	SpecieIds.Empty();

	int Index = -1;

#define DefineSpecieName(SpecieName) \
	Index = SpecieNames.Add(SpecieName);\
	SpecieIds.Add(SpecieName, Index);

	DefineSpecieName("snamdon");
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

UClass* USpecieDataManager::LoadSpecieDataObjectClass(FName SpecieName)
{
	const FString SpecieString = UStringUtils::ToUpperFirstLetter(SpecieName.ToString());
	const FString DataObjectClassReferenceString = USpecieDataObject::GetImmiesBlueprintFolder() + SpecieString + "/BP_" + SpecieString + "DataObject.BP_" + SpecieString + "DataObject_C'";
	UClass* DataObjectClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *DataObjectClassReferenceString));

	if (!IsValid(DataObjectClass)) {
		iLog("Data object UClass for specie " + SpecieName.ToString() + " is not valid.", LogVerbosity_Error);
		return nullptr;
	}

	return DataObjectClass;
}

USpecieDataObject* USpecieDataManager::RegisterSpecie(UObject* Outer, FName SpecieName, int SpecieId, const FString& JsonString, bool LoadLearnsets)
{
	UClass* DataObjectClass = LoadSpecieDataObjectClass(SpecieName);

	FJsonObjectBP SpecieJson;
	if (!FJsonObjectBP::LoadJsonString(JsonString, SpecieJson)) {
		iLog("Failed to load json data for specie " + SpecieName.ToString(), LogVerbosity_Error);
		return nullptr;
	}

	USpecieDataObject* SpecieDataObject = USpecieDataObject::CreateSpecieDataObject(Outer, SpecieId, DataObjectClass);
	SpecieDataObject->LoadSpecieJsonData(SpecieJson, LoadLearnsets);
	return SpecieDataObject;
}

void USpecieDataManager::RegisterSpeciesFromDisk(UObject* Outer, const FString& FolderName, TMap<int, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets)
{
	SpeciesOut->Empty();

	for (int i = 0; i < SpecieNames.Num(); i++) {
		FString JsonString = LoadSpecieJsonFileToString(SpecieNames[i], FolderName);
		const int SpecieId = *SpecieIds.Find(SpecieNames[i]);
		SpeciesOut->Add(
			SpecieId,
			RegisterSpecie(Outer, SpecieNames[i], SpecieId, JsonString, LoadLearnsets)
		); 
#ifdef DEVELOPMENT
			ULogger::Log("Loaded specie data object from disk with json data [" + SpecieNames[i].ToString() + "] using internal specie id of: " + FString::FromInt(SpecieId));
#else
			ULogger::Log("Loaded ability data object from disk with json data [" + SpecieNames[i].ToString() + "]");
#endif
	}
}

void USpecieDataManager::RegisterSpeciesFromSerialized(UObject* Outer, const TArray<FSerializedSpecieData>& SpecieData, TMap<int, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets)
{
	SpeciesOut->Empty();

	for (int i = 0; i < SpecieData.Num(); i++) {
		SpeciesOut->Add(
			SpecieData[i].SpecieId,
			RegisterSpecie(Outer, SpecieData[i].SpecieName, SpecieData[i].SpecieId, SpecieData[i].JsonString, LoadLearnsets)
		);
#ifdef DEVELOPMENT
		ULogger::Log("Loaded specie data object from serialized json data [" + SpecieData[i].SpecieName.ToString() + "] using internal specie id of: " + FString::FromInt(SpecieData[i].SpecieId));
#else
		ULogger::Log("Loaded ability data object from serialized json data [" + SpecieData[i].SpecieName.ToString() + "]");
#endif
	}
}

USpecieDataObject* USpecieDataManager::GetSpecieDataObjectFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId)
{
	USpecieDataObject** Found = Map.Find(SpecieId);
	if (Found) {
		return *Found;
	}

	iLog("Could not find specie data object for specie id " + FString::FromInt(SpecieId), LogVerbosity_Error);
	return nullptr;
}

USpecieDataObject* USpecieDataManager::GetSpecieDataObject(int SpecieId)
{
	return GetSpecieDataObjectFromMap(Species, SpecieId);
}

int USpecieDataManager::GetSpecieIdFromMap(TMap<FName, int>& Map, FName SpecieName)
{
	int* Found = Map.Find(SpecieName);
	if (Found) {
		return *Found;
	}

	iLog("Could not find specie id for " + SpecieName.ToString(), LogVerbosity_Error);
	return INVALID_SPECIE_ID;
}

int USpecieDataManager::GetSpecieId(FName Name)
{
	return GetSpecieIdFromMap(SpecieIds, Name);
}

FName USpecieDataManager::GetSpecieNameFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId)
{
	USpecieDataObject** Found = Map.Find(SpecieId);
	if (Found) {
		return (*Found)->GetSpecieName();
	}

	iLog("Could not find name for specie id " + FString::FromInt(SpecieId));
	return FName();
}

FName USpecieDataManager::GetSpecieName(int SpecieId)
{
	return GetSpecieNameFromMap(Species, SpecieId);
}

bool USpecieDataManager::IsValidSpecieFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId)
{
	return IsValid(*Map.Find(SpecieId));
}

bool USpecieDataManager::IsValidSpecie(int SpecieId)
{
	return IsValidSpecieFromMap(Species, SpecieId);
}

int USpecieDataManager::GetSpecieTypeBitmaskFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId)
{
	USpecieDataObject** Found = Map.Find(SpecieId);
	if (Found) {
		return (*Found)->GetTypeBitmask();
	}

	iLog("Unable to find type bitmask for specie id " + FString::FromInt(SpecieId), LogVerbosity_Error);
	return Type_Neutral;
}

int USpecieDataManager::GetSpecieTypeBitmask(int SpecieId)
{
	return GetSpecieTypeBitmaskFromMap(Species, SpecieId);
}

FBaseStats USpecieDataManager::GetSpecieBaseStatsFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId)
{
	USpecieDataObject** Found = Map.Find(SpecieId);
	if (Found) {
		return (*Found)->GetBaseStats();
	}

	iLog("Unable to find base stats for specie id " + FString::FromInt(SpecieId), LogVerbosity_Error);
	return FBaseStats();
}

FBaseStats USpecieDataManager::GetSpecieBaseStats(int SpecieId)
{
	return GetSpecieBaseStatsFromMap(Species, SpecieId);
}

UClass* USpecieDataManager::GetImmieObjectClassFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId)
{
	USpecieDataObject** Found = Map.Find(SpecieId);
	if (Found) {
		return (*Found)->GetImmieObjectClass();
	}

	iLog("Unable to find Immie object UClass for specie id " + FString::FromInt(SpecieId), LogVerbosity_Error);
	return nullptr;
}

UClass* USpecieDataManager::GetImmieObjectClass(int SpecieId)
{
	return GetImmieObjectClassFromMap(Species, SpecieId);
}

UClass* USpecieDataManager::GetImmieCharacterClassFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId)
{
	USpecieDataObject** Found = Map.Find(SpecieId);
	if (Found) {
		return (*Found)->GetImmieCharacterClass();
	}

	iLog("Unable to find Immie character UClass for specie id " + FString::FromInt(SpecieId), LogVerbosity_Error);
	return nullptr;
}

UClass* USpecieDataManager::GetImmieCharacterClass(int SpecieId)
{
	return GetImmieCharacterClassFromMap(Species, SpecieId);
}

FSpecieLearnset USpecieDataManager::GetSpecieLearnsetsFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId)
{
	USpecieDataObject** Found = Map.Find(SpecieId);
	if (Found) {
		return (*Found)->GetLearnset();
	}

	iLog("Could not find specie learnset for Id number " + FString::FromInt(SpecieId), LogVerbosity_Error);
	return FSpecieLearnset();
}

FSpecieLearnset USpecieDataManager::GetSpecieLearnsets(int SpecieId)
{
	return GetSpecieLearnsetsFromMap(Species, SpecieId);
}


