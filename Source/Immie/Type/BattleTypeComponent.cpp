// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTypeComponent.h"
#include <Immie/Game/Global/Managers/TypeDataManager.h>

UBattleTypeComponent::UBattleTypeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UBattleTypeComponent::Initialize(FTypeConstants _DamageMultipliers, const TArray<FImmieType> _TypesData)
{
	checkf(_TypesData.Num() > 0, TEXT("Cannot initialize UBattleTypeComponent with an empty array of FImmieType"));

	for (const FImmieType TypeData : _TypesData) {
		checkf(TypeData.Type.TypeCount() == 1, TEXT("FImmieType Type member variable must have exactly 1 type. Found %i types"), TypeData.Type.TypeCount());

		Types = FTypeBitmask::Combine(Types, TypeData.Type);
	}

	TypesData = _TypesData;
	DamageMultipliers = _DamageMultipliers;
}

float UBattleTypeComponent::TotalTypeDamageMultiplier(const TArray<EImmieType>& AttackingTypes) const
{
	float Multiplier = 1.0f;
	for (const FImmieType DefendTypeData : TypesData) {
		for (EImmieType AttackType : AttackingTypes) {
			if (DefendTypeData.Weaknesses.HasType(AttackType)) Multiplier *= DamageMultipliers.WeaknessMultiplier;
			if (DefendTypeData.Resistances.HasType(AttackType)) Multiplier *= DamageMultipliers.ResistanceMultiplier;
		}
	}

	return Multiplier;
}

FLinearColor UBattleTypeComponent::GetColor() const
{
	EImmieType FirstType = TypesData[0].Type.GetTypes()[0]; // The checks in UBattleTypeComponent::Initialize ensure this is valid.
	return GetTypeDataManager()->GetTypeColor(FirstType);
}

TArray<EImmieType> UBattleTypeComponent::GetTypes() const
{
	return Types.GetTypes();
}

void UBattleTypeComponent::CopyFrom(const UBattleTypeComponent* ComponentToCopy) {
	Types = ComponentToCopy->Types;
	TypesData = ComponentToCopy->TypesData;
	DamageMultipliers = ComponentToCopy->DamageMultipliers;
}

void UBattleTypeComponent::SyncToClients()
{
	//ULogger::Log("Syncing type data with client for type " + GetTypeName().ToString());
	SetClientTypeData(Types, TypesData, DamageMultipliers);
}

void UBattleTypeComponent::SetClientTypeData_Implementation(FTypeBitmask _Types, const TArray<FImmieType>& _TypesData, FTypeConstants _DamageMultipliers)
{
	if (IsRunningDedicatedServer()) {
		return;
	}

	iLog("synced client type data");

	Types = _Types;
	TypesData = _TypesData;
	DamageMultipliers = _DamageMultipliers;
}

