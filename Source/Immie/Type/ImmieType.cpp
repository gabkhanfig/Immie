// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieType.h"
#include <Immie/ImmieCore.h>
#include <Immie/Game/Global/Managers/TypeDataManager.h>

// Sets default values for this component's properties
UImmieType::UImmieType()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	TypeBitmask = Type_Neutral;
	Weaknesses = Type_Neutral;
	Resistances = Type_Neutral;
	Color = FLinearColor(1.f, 1.f, 1.f, 1.f);
}

UImmieType* UImmieType::FromJson(UObject* Outer, const FName& Name, const FJsonObjectBP& Json, const FTypeConstants& _TypeConstants)
{
	UImmieType* ImmieType = NewObject<UImmieType>(Outer);

	ImmieType->TypeBitmask = GetTypeDataManager()->GetTypeBitmask(Name);
	ImmieType->Color = FLinearColor::FromSRGBColor(FColor::FromHex(Json.GetStringField("HexColor")));
	ImmieType->TypeConstants = _TypeConstants;
	ImmieType->Weaknesses = GetTypeDataManager()->GetTypeBitmaskFromJsonArray(Json.GetArrayField("Weaknesses"));
	ImmieType->Resistances = GetTypeDataManager()->GetTypeBitmaskFromJsonArray(Json.GetArrayField("Resistances"));
	
	return ImmieType;
}

float UImmieType::GetTypeEffectiveness(int AttackTypeBitmask)
{
	if (AttackTypeBitmask & Weaknesses)
		return TypeConstants.WeaknessMultiplier;
	if (AttackTypeBitmask & Resistances)
		return TypeConstants.ResistanceMultiplier;
	return 1.f;
}

float UImmieType::TotalTypeDamageMultiplier(const TArray<UImmieType*>& AttackingType, const TArray<UImmieType*>& DefendingType)
{
	float Multiplier = 1.0f;

	for (int defendIndex = 0; defendIndex < DefendingType.Num(); defendIndex++) {
		for (int attackIndex = 0; attackIndex < AttackingType.Num(); attackIndex++) {
			Multiplier *= DefendingType[defendIndex]->GetTypeEffectiveness(AttackingType[attackIndex]->TypeBitmask);
		}
	}

	return Multiplier;
}

FName UImmieType::GetTypeName() const
{
	return GetTypeDataManager()->GetTypeName(TypeBitmask);
}

void UImmieType::SyncToClients()
{
	ULogger::Log("Syncing type data with client for type " + GetTypeName().ToString());
	SetClientTypeData(TypeBitmask, Weaknesses, Resistances, TypeConstants);
}

void UImmieType::SetClientTypeData_Implementation(int _TypeBitmask, int _Weaknesses, int _Resistances, const FTypeConstants& _TypeConstants)
{
	if (IsRunningDedicatedServer()) {
		return;
	}

	TypeBitmask = _TypeBitmask;
	Weaknesses = _Weaknesses;
	Resistances = _Resistances;
	TypeConstants = _TypeConstants;

	UImmieType* ClientLocalGlobalType = GetTypeDataManager()->GetType(TypeBitmask);
	if (!IsValid(ClientLocalGlobalType)) {
		iLog("Client does not have a corresponding global type for the type with bitmask " + FString::FromInt(TypeBitmask));
		return;
	}
	Color = ClientLocalGlobalType->GetColor();
	
	ULogger::Log("Synchronized client type data for type " + GetTypeName().ToString());
}
