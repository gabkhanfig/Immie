// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieType.h"
#include <Immie/ImmieCore.h>
#include <Immie/Game/Global/Managers/TypeDataManager.h>

FTypeBitmask::FTypeBitmask() : Bitmask(0) {}

void FTypeBitmask::AddType(EImmieType Type)
{
	const int index = static_cast<int>(Type);
	Bitmask |= 1 << index;
}

void FTypeBitmask::RemoveType(EImmieType Type)
{
	const int index = static_cast<int>(Type);
	Bitmask &= ~(1 << index);
}

bool FTypeBitmask::HasType(EImmieType Type) const
{
	return Bitmask & (1 << static_cast<int>(Type));
}

TArray<EImmieType> FTypeBitmask::GetTypes() const
{
	TArray<EImmieType> Types;
	Types.Reserve(TypeCount());
	if (HasType(EImmieType::Neutral))		Types.Add(EImmieType::Neutral);
	if (HasType(EImmieType::Fire))			Types.Add(EImmieType::Fire);
	if (HasType(EImmieType::Water))			Types.Add(EImmieType::Water);
	if (HasType(EImmieType::Nature))		Types.Add(EImmieType::Nature);
	if (HasType(EImmieType::Standard))	Types.Add(EImmieType::Standard);
	if (HasType(EImmieType::Electric))	Types.Add(EImmieType::Electric);
	if (HasType(EImmieType::Air))				Types.Add(EImmieType::Air);
	if (HasType(EImmieType::Ground))		Types.Add(EImmieType::Ground);
	if (HasType(EImmieType::Metal))			Types.Add(EImmieType::Metal);
	if (HasType(EImmieType::Light))			Types.Add(EImmieType::Light);
	if (HasType(EImmieType::Dark))			Types.Add(EImmieType::Dark);
	if (HasType(EImmieType::Dragon))		Types.Add(EImmieType::Dragon);
	return Types;
}

void FTypeBitmask::AddTypes(const TArray<EImmieType>& Types)
{
	for (EImmieType Type : Types) {
		AddType(Type);
	}
}

int FTypeBitmask::TypeCount() const
{
	return FMath::CountBits(Bitmask);
}

void FTypeBitmask::Clear()
{
	Bitmask = 0;
}

FTypeBitmask FTypeBitmask::Combine(const FTypeBitmask Left, const FTypeBitmask Right)
{
	FTypeBitmask Combined;
	Combined.Bitmask = Left.Bitmask | Right.Bitmask;
	return Combined;
}

FTypeBitmask FTypeBitmask::FromJsonTypesArrayField(const FJsonArrayBP& JsonArray)
{
	FTypeBitmask Bitmask;
	const TArray<FString> TypeStrings = JsonArray.GetStringArray("Type");
	for (const FString& Type : TypeStrings) {
		if (UTypeDataManager::IsValidTypeNameString(Type)) {
			Bitmask.AddType(UTypeDataManager::GetTypeEnum(FName(Type)));
		}
		else {
			iLog("Json parsed type name of " + Type + " is not a valid type name");
			continue;
		}
	}

	return Bitmask;
}

FImmieType FImmieType::FromJson(EImmieType Type, const FJsonObjectBP& Json)
{
	FImmieType ImmieType;
	ImmieType.Type.AddType(Type);
	FJsonArrayBP WeaknessesArray = Json.GetArrayField("Weaknesses");
	FJsonArrayBP ResistancesArray = Json.GetArrayField("Resistances");

	ImmieType.Weaknesses = FTypeBitmask::FromJsonTypesArrayField(WeaknessesArray);
	ImmieType.Resistances = FTypeBitmask::FromJsonTypesArrayField(ResistancesArray);

	return ImmieType;
}

FJsonObjectBP FImmieType::ToJson()
{
	FJsonObjectBP JsonObject;
	FJsonArrayBP WeaknessesArray;
	FJsonArrayBP ResistancesArray;

	const TArray<EImmieType> WeakTypes = Weaknesses.GetTypes();
	const TArray<EImmieType> ResistTypes = Resistances.GetTypes();

	for (EImmieType Weak : WeakTypes) {
		WeaknessesArray.AddStringElement("Type", UTypeDataManager::GetTypeName(Weak).ToString());
	}
	for (EImmieType Resist : ResistTypes) {
		ResistancesArray.AddStringElement("Type", UTypeDataManager::GetTypeName(Resist).ToString());
	}

	JsonObject.SetArrayField("Weaknesses", WeaknessesArray);
	JsonObject.SetArrayField("Resistances", ResistancesArray);

	return JsonObject;
}


