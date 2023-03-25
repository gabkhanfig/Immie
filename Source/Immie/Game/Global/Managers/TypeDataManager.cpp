// Fill out your copyright notice in the Description page of Project Settings.


#include "TypeDataManager.h"
#include <Immie/Type/ImmieType.h>

void UTypeDataManager::MapTypeBitmasks()
{
	TypeNames.Empty();
	TypeBitmasks.Empty();

#define MAP_TYPE_BITMASK(TypeEnum, TypeName) \
TypeNames.Add(TypeEnum, TypeName); \
TypeBitmasks.Add(TypeName, TypeEnum);

	MAP_TYPE_BITMASK(Type_Neutral, "neutral");
	MAP_TYPE_BITMASK(Type_Fire, "fire");
	MAP_TYPE_BITMASK(Type_Water, "water");
	MAP_TYPE_BITMASK(Type_Nature, "nature");
	MAP_TYPE_BITMASK(Type_Standard, "standard");
	MAP_TYPE_BITMASK(Type_Electric, "electric");
	MAP_TYPE_BITMASK(Type_Air, "air");
	MAP_TYPE_BITMASK(Type_Ground, "ground");
	MAP_TYPE_BITMASK(Type_Metal, "metal");
	MAP_TYPE_BITMASK(Type_Light, "light");
	MAP_TYPE_BITMASK(Type_Dark, "dark");
	MAP_TYPE_BITMASK(Type_Dragon, "dragon");
}

void UTypeDataManager::LoadDefaultGameData()
{
	ULogger::Log("Mapping Battle Type Bitmasks", LogVerbosity_Warning);
	MapTypeBitmasks();
	
	ULogger::Log("Loading type constants", LogVerbosity_Warning);
	RegisterTypeConstants("Default", TypeConstants);
	
	ULogger::Log("Loading all type data", LogVerbosity_Warning);
	RegisterTypes(this, "Default", &Types, TypeConstants);
}

void UTypeDataManager::RegisterTypeConstants(const FString& FolderName, FTypeConstants& TypeConstantsOut)
{
	const FString JsonPath = GetGameDataDir() + FolderName + "/Types/_TypeConstants.json";

	FJsonObjectBP TypeConstantsJson;
	if (FJsonObjectBP::LoadJsonFile(JsonPath, TypeConstantsJson)) {
		TypeConstantsOut.WeaknessMultiplier = TypeConstantsJson.GetFloatField("WeaknessMultiplier");
		TypeConstantsOut.ResistanceMultiplier = TypeConstantsJson.GetFloatField("ResistanceMultiplier");
	}
	else {
		iLog("Unable to load type constants specified path " + JsonPath);
	}
}

void UTypeDataManager::RegisterTypes(UObject* Outer, const FString& FolderName, TMap<int, UImmieType*>* TypesOut, const FTypeConstants& TypeConsts)
{
	for (const auto& TypeName : TypeNames) {
		RegisterType(TypeName.Value, Outer, FolderName, TypesOut, TypeConsts);
	}
}

void UTypeDataManager::RegisterType(const FName& Name, UObject* Outer, const FString& FolderName, TMap<int, UImmieType*>* TypesOut, const FTypeConstants& TypeConsts)
{
	const FString JsonPath = GetGameDataDir() + FolderName + "/Types/" + Name.ToString() + ".json";

	FJsonObjectBP TypeJson;
	if (FJsonObjectBP::LoadJsonFile(JsonPath, TypeJson)) {
		const int TypeBitmask = GetTypeBitmask(Name);
		UImmieType* Type = UImmieType::FromJson(Outer, Name, TypeJson, TypeConsts);
#ifdef DEVELOPMENT
		ULogger::Log("Loaded type data for " + Name.ToString() + " with type bitmask of " + FString::FromInt(TypeBitmask));
#else
		ULogger::Log("Loaded type data for " + Name.ToString());
#endif
		TypesOut->Add(TypeBitmask, Type);
	}
	else {
		iLog("Unable to load Immie type " + Name.ToString());
	}
}

int UTypeDataManager::GetTypeBitmask(FName Name)
{
	int* FoundBitmask = TypeBitmasks.Find(Name);
	if (FoundBitmask) {
		return *FoundBitmask;
	}
	else {
		iLog("Unable to find type bitmask for " + Name.ToString() + ". Sending neutral instead", LogVerbosity_Error);
		return Type_Neutral;
	}
}

FName UTypeDataManager::GetTypeName(int TypeBitmask)
{
	FName* FoundName = TypeNames.Find(TypeBitmask);
	if (FoundName) {
		return *FoundName;
	}
	else {
		iLog("Unable to find type bitmask for " + FString::FromInt(TypeBitmask) + ". Sending neutral instead", LogVerbosity_Error);
		return "neutral";
	}
}

bool UTypeDataManager::IsValidType(int TypeBitmask)
{
	return Types.Contains(TypeBitmask);
}

UImmieType* UTypeDataManager::GetType(int TypeBitmask)
{
	UImmieType** TypeFound = Types.Find(TypeBitmask);
	if (TypeFound) {
		return *TypeFound;
	}
	
	else {
		iLog("Unable to find Immie type with bitmask " + FString::FromInt(TypeBitmask), LogVerbosity_Error);
		return nullptr;
	}
}

UImmieType* UTypeDataManager::GetTypeFromName(FName Name)
{
	const int TypeBitmask = GetTypeBitmask(Name);
	UImmieType** TypeFound = Types.Find(TypeBitmask);
	if (TypeFound) {
		return *TypeFound;
	}

	else {
		iLog("Unable to find Immie type  " + Name.ToString(), LogVerbosity_Error);
		return nullptr;
	}
}

TArray<UImmieType*> UTypeDataManager::GetTypes(int TypeBitmask)
{
	return GetMultipleTypesFromMap(TypeBitmask, Types);
}

TArray<UImmieType*> UTypeDataManager::GetMultipleTypesFromMap(int TypesBitmask, TMap<int, UImmieType*>& Map)
{
	TArray<UImmieType*> TypesOut;
	for (int i = 0; i < (sizeof(int) * 8) + 1; i++) {
		int SingleBitmask = 0;
		if (i > 0) {
			SingleBitmask = 1 << (i - 1);
		}

		const bool BitmaskHasType = TypesBitmask & SingleBitmask;
		if (!BitmaskHasType) {
			continue;
		}

		UImmieType** Found = Map.Find(TypesBitmask & SingleBitmask);
		if (!Found) {
			iLog("Unable to get type for multiple bitmask types with specific bit " + FString::FromInt(TypesBitmask & SingleBitmask), LogVerbosity_Error);
			continue;
		}

		TypesOut.Add(*Found);
	}

	return TypesOut;
}

int UTypeDataManager::GetTypeBitmaskFromJsonArray(const FJsonArrayBP& JsonArray)
{
	TArray<FString> JsonAsFString = JsonArray.GetStringArray("Type");
	int Bitmask = 0;
	for (int i = 0; i < JsonAsFString.Num(); i++) {
		Bitmask |= GetTypeBitmask(FName(JsonAsFString[i]));
	}
	return Bitmask;
}

TArray<FName> UTypeDataManager::GetTypeNames(int TypeBitmask)
{
	TArray<FName> Names;
	for (const auto& MaskPair : TypeBitmasks) {
		const int Bitmask = MaskPair.Value;
		if (TypeBitmask & Bitmask) {
			Names.Add(MaskPair.Key);
		}
	}
	return Names;
}
