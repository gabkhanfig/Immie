// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDataObject.h"
#include <Immie/ImmieCore.h>
#include <Immie/Game/Global/Managers/TypeDataManager.h>
#include <Immie/Game/Global/Managers/AbilityDataManager.h>
#include <Immie/Game/Global/Managers/SpecieDataManager.h>

#define ABILITY_DO_JSON_FIELD_TYPES "Types"
#define ABILITY_DO_JSON_FIELD_ABILITY_FLAGS "AbilityFlags"
#define ABILITY_DO_JSON_FIELD_INITIAL_COOLDOWN "InitialCooldown"
#define ABILITY_DO_JSON_FIELD_MAX_COOLDOWN "MaxCooldown"
#define ABILITY_DO_JSON_FIELD_INITIAL_USES "InitialUses"
#define ABILITY_DO_JSON_FIELD_MAX_USES "MaxUses"
#define ABILITY_DO_JSON_FIELD_DAMAGE_POWER "DamagePower"
#define ABILITY_DO_JSON_FIELD_DAMAGE_DURATION "DamageDuration"
#define ABILITY_DO_JSON_FIELD_HEALING_POWER "HealingPower"
#define ABILITY_DO_JSON_FIELD_HEALING_DURATION "HealingDuration"
#define ABILITY_DO_JSON_FIELD_SPEED "Speed"
#define ABILITY_DO_JSON_FIELD_RANGE "Range"
#define ABILITY_DO_JSON_FIELD_MAX_DELAY "MaxDelay"
#define ABILITY_DO_JSON_FIELD_BASE_HEALTH "BaseHealth"
#define ABILITY_DO_JSON_FIELD_BASE_ATTACK "BaseAttack"
#define ABILITY_DO_JSON_FIELD_BASE_DEFENSE "BaseDefense"
#define ABILITY_DO_JSON_FIELD_BASE_SPEED "BaseSpeed"
#define ABILITY_DO_JSON_FIELD_RELATIVE_HEALTH "RelativeHealth"
#define ABILITY_DO_JSON_FIELD_RELATIVE_ATTACK "RelativeAttack"
#define ABILITY_DO_JSON_FIELD_RELATIVE_DEFENSE "RelativeDefense"
#define ABILITY_DO_JSON_FIELD_RELATIVE_SPEED "RelativeSpeed"
#define ABILITY_DO_JSON_FIELD_ABILITY_SPECIFIC "AbilitySpecific"

#define DEFAULT_MAX_DELAY 0.25

class UAbility;
class AAbilityActor;
class ADummyAbilityActor;

UAbilityDataObject::UAbilityDataObject()
{
}

void UAbilityDataObject::LoadClasses()
{
	if (AbilityName == "None") {
		iLog("Ability name is \"None\" for ability data object " + GetFName().ToString(), LogVerbosity_Error);
		return;
	}

	const FString AbilityCapitalizedName = UStringUtils::ToUpperFirstLetter(AbilityName.ToString());

	const FString AbilityClassReferenceString = GetAbilitiesBlueprintFolder() + AbilityCapitalizedName + "/" + AbilityCapitalizedName + "_BP." + AbilityCapitalizedName + "_BP_C'";
	UClass* AbilityClassReference = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *AbilityClassReferenceString));
	AbilityClass = AbilityClassReference;

	const FString ActorClassReferenceString = GetAbilitiesBlueprintFolder() + AbilityCapitalizedName + "/" + AbilityCapitalizedName + "Actor_BP." + AbilityCapitalizedName + "Actor_BP_C'";
	UClass* ActorClassReference = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *ActorClassReferenceString));
	if (IsValid(ActorClassReference)) {
		ActorClass = ActorClassReference;
	}

	const FString DummyActorClassReferenceString = GetAbilitiesBlueprintFolder() + AbilityCapitalizedName + "/Dummy" + AbilityCapitalizedName + "Actor_BP.Dummy" + AbilityCapitalizedName + "Actor_BP_C'";
	UClass* DummyActorClassReference = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *DummyActorClassReferenceString));
	if (IsValid(DummyActorClassReference)) {
		DummyActorClass = DummyActorClassReference;
	}
}

const FString& UAbilityDataObject::GetAbilitiesBlueprintFolder()
{
	static const FString AbilitiesBlueprintFolder = "/Script/Engine.Blueprint'/Game/Abilities/";
	return AbilitiesBlueprintFolder;
}

UAbilityDataObject* UAbilityDataObject::CreateAbilityDataObject(UObject* Outer, int _AbilityId, UClass* DataObjectClass)
{
	UAbilityDataObject* AbilityDataObject = NewObject<UAbilityDataObject>(Outer, DataObjectClass);
	AbilityDataObject->AbilityId = _AbilityId;
	AbilityDataObject->LoadClasses();
	return AbilityDataObject;
}

void UAbilityDataObject::CheckClassesValid()
{
	iLog(IsValid(AbilityClass) ? "ability class is valid" : "ability class no valid :(");
	iLog(IsValid(ActorClass) ? "ability actor class is valid" : "ability actor class not valid");
}

void UAbilityDataObject::LoadAbilityJsonData(const FJsonObjectBP& Json)
{
	if (Json.HasField(ABILITY_DO_JSON_FIELD_TYPES)) {
		TypeBitmask = GetTypeDataManager()->GetTypeBitmaskFromJsonArray(Json.GetArrayField("Types"));
	}

	FJsonObjectBP AbilityFlagsJson;
	if (Json.TryGetObjectField(ABILITY_DO_JSON_FIELD_ABILITY_FLAGS, AbilityFlagsJson)) {
		AbilityFlags = FAbilityFlags::LoadJsonAbilityFlags(AbilityFlagsJson);
	}

	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_INITIAL_COOLDOWN, InitialCooldown);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_MAX_COOLDOWN, MaxCooldown);
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_INITIAL_USES, InitialUses);
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_MAX_USES, MaxUses);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_DAMAGE_POWER, DamagePower);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_DAMAGE_DURATION, DamageDuration);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_HEALING_POWER, HealingPower);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_HEALING_DURATION, HealingDuration);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_SPEED, Speed);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_RANGE, Range);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_MAX_DELAY, MaxDelay, true, DEFAULT_MAX_DELAY);

#define CHECK_BASE_STAT_RANGE(TempVariable, BaseStatVariable) \
if (TempVariable > BASE_STAT_MAX || TempVariable < 0) {	\
	iLog("Base stat of " + FString::FromInt(TempVariable) + " is out of base stat range.", LogVerbosity_Error);	\
	BaseStatVariable = 0;	\
}	\
else {	\
	BaseStatVariable = TempVariable;	\
}

	int _BaseHealth = 0;
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_BASE_HEALTH, _BaseHealth);
	CHECK_BASE_STAT_RANGE(_BaseHealth, BaseStats.Health);

	int _BaseAttack = 0;
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_BASE_ATTACK, _BaseAttack);
	CHECK_BASE_STAT_RANGE(_BaseAttack, BaseStats.Attack);

	int _BaseDefense = 0;
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_BASE_DEFENSE, _BaseDefense);
	CHECK_BASE_STAT_RANGE(_BaseDefense, BaseStats.Defense);

	int _BaseSpeed = 0;
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_BASE_SPEED, _BaseSpeed);
	CHECK_BASE_STAT_RANGE(_BaseSpeed, BaseStats.Speed);

	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_HEALTH, RelativeStats.Health);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_ATTACK, RelativeStats.Attack);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_DEFENSE, RelativeStats.Defense);
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_SPEED, RelativeStats.Speed);

	FJsonObjectBP AbilitySpecific;
	if (Json.TryGetObjectField(ABILITY_DO_JSON_FIELD_ABILITY_SPECIFIC, AbilitySpecific)) {
		BP_LoadAbilityJsonData(AbilitySpecific);
	}
}

FJsonObjectBP UAbilityDataObject::AbilityDataToJson()
{
	FJsonObjectBP Json;

	TArray<FName> TypeNames = GetTypeDataManager()->GetTypeNames(TypeBitmask);
	const int TypesCount = TypeNames.Num();
	TArray<FString> TypeStrings;
	TypeStrings.Reserve(TypesCount);
	for (int i = 0; i < TypesCount; i++) {
		TypeStrings.Add(TypeNames[i].ToString());
	}
	FJsonArrayBP TypesArray;
	TypesArray.SetStringArray("Type", TypeStrings);

	Json.SetArrayField(ABILITY_DO_JSON_FIELD_TYPES, TypesArray);

	Json.SetFloatField(ABILITY_DO_JSON_FIELD_INITIAL_COOLDOWN, InitialCooldown);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_MAX_COOLDOWN, MaxCooldown);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_INITIAL_USES, InitialUses);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_MAX_USES, MaxUses);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_DAMAGE_POWER, DamagePower);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_DAMAGE_DURATION, DamageDuration);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_HEALING_POWER, HealingPower);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_HEALING_DURATION, HealingDuration);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_SPEED, Speed);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_RANGE, Range);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_MAX_DELAY, MaxDelay);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_BASE_HEALTH, BaseStats.Health);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_BASE_ATTACK, BaseStats.Attack);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_BASE_DEFENSE, BaseStats.Defense);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_BASE_SPEED, BaseStats.Speed);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_HEALTH, RelativeStats.Health);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_ATTACK, RelativeStats.Attack);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_DEFENSE, RelativeStats.Defense);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_SPEED, RelativeStats.Speed);

	Json.SetObjectField(ABILITY_DO_JSON_FIELD_ABILITY_FLAGS, AbilityFlags.ToJson());
	Json.SetObjectField(ABILITY_DO_JSON_FIELD_ABILITY_SPECIFIC, BP_AbilityDataToJson());

	return Json;
}

FJsonObjectBP UAbilityDataObject::BP_AbilityDataToJson_Implementation()
{
	return FJsonObjectBP();
}