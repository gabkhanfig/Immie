// Fill out your copyright notice in the Description page of Project Settings.


#include "TypeDataManager.h"
#include <Immie/Type/ImmieType.h>
#include <Immie/Type/BattleTypeComponent.h>

UTypeDataManager::UTypeDataManager()
{
	NeutralColor = FLinearColor::FromSRGBColor(FColor::FromHex("#ede5df"));
	FireColor = FLinearColor::FromSRGBColor(FColor::FromHex("#d1480d"));
	WaterColor = FLinearColor::FromSRGBColor(FColor::FromHex("#066abd"));
	NatureColor = FLinearColor::FromSRGBColor(FColor::FromHex("#5ec916"));
	StandardColor = FLinearColor::FromSRGBColor(FColor::FromHex("#a3a2a3"));
	ElectricColor = FLinearColor::FromSRGBColor(FColor::FromHex("#e3dd66"));
	AirColor = FLinearColor::FromSRGBColor(FColor::FromHex("#bff2e3"));
	GroundColor = FLinearColor::FromSRGBColor(FColor::FromHex("#9c730b"));
	MetalColor = FLinearColor::FromSRGBColor(FColor::FromHex("#bfb8b9"));
	LightColor = FLinearColor::FromSRGBColor(FColor::FromHex("#e9f7cb"));
	DarkColor = FLinearColor::FromSRGBColor(FColor::FromHex("#28033d"));
	DragonColor = FLinearColor::FromSRGBColor(FColor::FromHex("#5c4cc7"));
}

static TMap<FName, EImmieType> MappedTypeNamesToEnums() {
	TMap<FName, EImmieType> Map;
	Map.Add("Neutral", EImmieType::Neutral);
	Map.Add("Fire", EImmieType::Fire);
	Map.Add("Water", EImmieType::Water);
	Map.Add("Nature", EImmieType::Nature);
	Map.Add("Standard", EImmieType::Standard);
	Map.Add("Electric", EImmieType::Electric);
	Map.Add("Air", EImmieType::Air);
	Map.Add("Ground", EImmieType::Ground);
	Map.Add("Metal", EImmieType::Metal);
	Map.Add("Light", EImmieType::Light);
	Map.Add("Dark", EImmieType::Dark);
	Map.Add("Dragon", EImmieType::Dragon);
	return Map;
}

static TArray<FName> AllTypeNames(const TMap<FName, EImmieType>& Map) {
	TArray<FName> Keys;
	Map.GetKeys(Keys);

	TArray<FName> Names;
	Names.Init(FName(), Keys.Num());
	for (FName Key : Keys) {
		const EImmieType* Type = Map.Find(Key);
		checkf(Type != nullptr, TEXT("Mapped type must be in the map"));
		const int Index = static_cast<int>(*Type);
		Names[Index] = Key;
	}
	return Names;
}

static TSet<FString> SetOfTypeStringNames(const TArray<FName>& Names) {
	TSet<FString> Set;
	for (FName Name : Names) {
		Set.Add(Name.ToString());
	}
	return Set;
}

const TMap<FName, EImmieType> UTypeDataManager::TypeEnums = MappedTypeNamesToEnums();
const TArray<FName> UTypeDataManager::TypeNames = AllTypeNames(UTypeDataManager::TypeEnums);
const TSet<FString> UTypeDataManager::TypeStringNames = SetOfTypeStringNames(UTypeDataManager::TypeNames);

void UTypeDataManager::LoadDefaultGameData()
{
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

void UTypeDataManager::RegisterTypes(UObject* Outer, const FString& FolderName, TMap<EImmieType, FImmieType>* TypesOut, const FTypeConstants& TypeConsts)
{
	for (const auto& TypeName : TypeNames) {
		RegisterType(TypeName, Outer, FolderName, TypesOut, TypeConsts);
	}
}

void UTypeDataManager::RegisterType(const FName& Name, UObject* Outer, const FString& FolderName, TMap<EImmieType, FImmieType>* TypesOut, const FTypeConstants& TypeConsts)
{
	const FString JsonPath = GetGameDataDir() + FolderName + "/Types/" + Name.ToString() + ".json";

	FJsonObjectBP TypeJson;
	if (FJsonObjectBP::LoadJsonFile(JsonPath, TypeJson)) {
		const EImmieType TypeEnum = GetTypeEnum(Name);
		FImmieType Type = FImmieType::FromJson(TypeEnum, TypeJson);
		
		//UBattleTypeComponent* Type = UBattleTypeComponent::FromJson(Outer, Name, TypeJson, TypeConsts);
		ULogger::Log("Loaded type data for " + Name.ToString());
		TypesOut->Add(TypeEnum, Type);
	}
	else {
		iLog("Unable to load Immie type " + Name.ToString());
	}
}

bool UTypeDataManager::IsValidTypeName(FName TypeName)
{
	return TypeEnums.Contains(TypeName);
}

bool UTypeDataManager::IsValidTypeNameString(FString TypeNameString)
{
	return TypeStringNames.Contains(TypeNameString);
}

EImmieType UTypeDataManager::GetTypeEnum(FName TypeName)
{
	const bool ValidTypeName = IsValidTypeName(TypeName);
	checkf(ValidTypeName, TEXT("Invalid type name of %s. Cannot get valid type enum value."), *TypeName.ToString());
	return *TypeEnums.Find(TypeName);
}

FName UTypeDataManager::GetTypeName(EImmieType Type)
{
	return TypeNames[static_cast<int>(Type)];
}

FLinearColor UTypeDataManager::GetTypeColor(EImmieType Type)
{
	switch (Type) {
		case EImmieType::Neutral: return NeutralColor;
		case EImmieType::Fire: return FireColor;
		case EImmieType::Water: return WaterColor;
		case EImmieType::Nature: return NatureColor;
		case EImmieType::Standard: return StandardColor;
		case EImmieType::Electric: return ElectricColor;
		case EImmieType::Air: return AirColor;
		case EImmieType::Ground: return GroundColor;
		case EImmieType::Metal: return MetalColor;
		case EImmieType::Light: return LightColor;
		case EImmieType::Dark: return DarkColor;
		case EImmieType::Dragon: return DragonColor;
		default: return FLinearColor();
	}
}

FImmieType UTypeDataManager::GetType(EImmieType Type)
{
	FImmieType* TypeFound = Types.Find(Type);
	checkf(TypeFound, TEXT("Cannot find FImmieType entry for type %s"), *GetTypeName(Type).ToString())
	return *TypeFound;
}
