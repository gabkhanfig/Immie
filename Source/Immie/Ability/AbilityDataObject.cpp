// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDataObject.h"
#include <Immie/ImmieCore.h>
#include <Immie/Game/Global/Managers/TypeDataManager.h>
#include <Immie/Game/Global/Managers/AbilityDataManager.h>
#include <Immie/Game/Global/Managers/SpecieDataManager.h>
#include <Immie/Game/Global/Managers/ConfigDataManager.h>
#include "Ability.h"
#include "AbilityActor.h"
#include "DummyAbilityActor.h"

#define ABILITY_DO_JSON_FIELD_TYPES "Types"
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
#define ABILITY_DO_JSON_FIELD_GRAVITY "Gravity"
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
	//AbilityClass = nullptr;
	//ActorClass = nullptr;
	//DummyActorClass = nullptr;

	bUseActorClass = false;
	bUseDummyActorClass = false;
	ActorClass = nullptr;
	DummyActorClass = nullptr;
}

void UAbilityDataObject::PostLoad()
{
	const FName ClassName = GetClass()->GetFName();
	if (ClassName == "AbilityDataObject") {
		Super::PostLoad();
		return;
	}

	AbilityName = UAbilityDataManager::AbilityNameFromBlueprintClassName(ClassName.ToString(), "DataObject_C"); 
	AbilityClass = FetchAbilityComponentClass(); 
	if (bUseActorClass) {
		ActorClass = FetchAbiltyActorClass();
	}
	if (bUseDummyActorClass) {
		DummyActorClass = FetchDummyAbilityActorClass();
	}

	Super::PostLoad();
}

#if WITH_EDITOR
void UAbilityDataObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr) {
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());
		if (PropertyName == GET_MEMBER_NAME_CHECKED(UAbilityDataObject, bUseActorClass)) {
			if (bUseActorClass) {
				ActorClass = FetchAbiltyActorClass();
			}
			else {
				ActorClass = nullptr;
			}
		}
		else if (PropertyName == GET_MEMBER_NAME_CHECKED(UAbilityDataObject, bUseDummyActorClass)) {
			if (bUseDummyActorClass) {
				DummyActorClass = FetchDummyAbilityActorClass();
			}
			else {
				DummyActorClass = nullptr;
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

UClass* UAbilityDataObject::FetchAbilityComponentClass() const
{
	if (AbilityName == "None") {
		iLog("Cannot fetch ability component class for an ability with a name of None", LogVerbosity_Error);
		return nullptr;
	}
	const FString AbilityString = AbilityName.ToString(); 
	const FString AbilityClassReferenceString = GetAbilitiesBlueprintFolder() + AbilityString + "/BP_" + AbilityString + ".BP_" + AbilityString + "_C'";
	TSubclassOf<UAbility> AbilityClassReference = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *AbilityClassReferenceString)); 
	if (!IsValid(AbilityClassReference)) {
		iLog("Unable to find ability component class for ability " + AbilityName.ToString(), LogVerbosity_Error);
		return nullptr;
	}
	return AbilityClassReference;
}

UClass* UAbilityDataObject::FetchAbiltyActorClass() const
{
	// No logging required. Not all abilities require a valid actor class.
	if (AbilityName == "None") {
		return nullptr;
	}
	const FString AbilityString = AbilityName.ToString();
	const FString ActorClassReferenceString = GetAbilitiesBlueprintFolder() + AbilityString + "/BP_" + AbilityString + "Actor.BP_" + AbilityString + "Actor_C'";
	TSubclassOf<AAbilityActor> ActorClassReference = StaticLoadClass(AAbilityActor::StaticClass(), NULL, *ActorClassReferenceString);
	if (!IsValid(ActorClassReference)) {
		return nullptr;
	}
	return ActorClassReference;
}

UClass* UAbilityDataObject::FetchDummyAbilityActorClass() const
{
	// No logging required. Not all abilities require a valid dummy actor class.
	if (AbilityName == "None") {
		return nullptr;
	}
	const FString AbilityString = AbilityName.ToString();
	const FString DummyActorClassReferenceString = GetAbilitiesBlueprintFolder() + AbilityString + "/BP_" + AbilityString + "DummyActor.BP_" + AbilityString + "DummyActor_C'";
	TSubclassOf<ADummyAbilityActor> DummyActorClassReference = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *DummyActorClassReferenceString));
	if (!IsValid(DummyActorClassReference)) {
		return nullptr;
	}
	return DummyActorClassReference;
}

const FString& UAbilityDataObject::GetAbilitiesBlueprintFolder()
{
	static const FString AbilitiesBlueprintFolder = "/Script/Engine.Blueprint'/Game/Abilities/";
	return AbilitiesBlueprintFolder;
}

UAbilityDataObject* UAbilityDataObject::CreateAbilityDataObject(UObject* Outer, UClass* DataObjectClass)
{
	UAbilityDataObject* AbilityDataObject = NewObject<UAbilityDataObject>(Outer, DataObjectClass);
	return AbilityDataObject;
}

void UAbilityDataObject::LoadAbilityJsonData(const FJsonObjectBP& Json)
{
	if (Json.HasField(ABILITY_DO_JSON_FIELD_TYPES)) {
		TypeBitmask = GetTypeDataManager()->GetTypeBitmaskFromJsonArray(Json.GetArrayField("Types"));
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
	Json.TryGetFloatField(ABILITY_DO_JSON_FIELD_GRAVITY, Gravity);

	const uint8 MaxBaseStat = GetConfigDataManager()->GetMaxBaseStat();

	int _BaseHealth = 0;
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_BASE_HEALTH, _BaseHealth); 
	if (_BaseHealth > MaxBaseStat) {
		ULogger::Log("Base health of " + FString::FromInt(_BaseHealth) + " is greater than the maximum allowed base stat of " + FString::FromInt(MaxBaseStat), LogVerbosity_Error);
		BaseStats.Health = MaxBaseStat;
	}
	else {
		BaseStats.Health = _BaseHealth;
	}

	int _BaseAttack = 0;
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_BASE_ATTACK, _BaseAttack);
	if (_BaseAttack > MaxBaseStat) {
		ULogger::Log("Base attack of " + FString::FromInt(_BaseAttack) + " is greater than the maximum allowed base stat of " + FString::FromInt(MaxBaseStat), LogVerbosity_Error);
		BaseStats.Attack = MaxBaseStat;
	}
	else {
		BaseStats.Attack = _BaseAttack;
	}

	int _BaseDefense = 0;
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_BASE_DEFENSE, _BaseDefense);
	if (_BaseDefense > MaxBaseStat) {
		ULogger::Log("Base defense of " + FString::FromInt(_BaseDefense) + " is greater than the maximum allowed base stat of " + FString::FromInt(MaxBaseStat), LogVerbosity_Error);
		BaseStats.Defense = MaxBaseStat;
	}
	else {
		BaseStats.Defense = _BaseDefense;
	}

	int _BaseSpeed = 0;
	Json.TryGetIntegerField(ABILITY_DO_JSON_FIELD_BASE_SPEED, _BaseSpeed);
	if (_BaseSpeed > MaxBaseStat) {
		ULogger::Log("Base speed of " + FString::FromInt(_BaseSpeed) + " is greater than the maximum allowed base stat of " + FString::FromInt(MaxBaseStat), LogVerbosity_Error);
		BaseStats.Speed = MaxBaseStat;
	}
	else {
		BaseStats.Speed = _BaseSpeed;
	}

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
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_GRAVITY, Gravity);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_BASE_HEALTH, BaseStats.Health);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_BASE_ATTACK, BaseStats.Attack);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_BASE_DEFENSE, BaseStats.Defense);
	Json.SetIntegerField(ABILITY_DO_JSON_FIELD_BASE_SPEED, BaseStats.Speed);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_HEALTH, RelativeStats.Health);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_ATTACK, RelativeStats.Attack);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_DEFENSE, RelativeStats.Defense);
	Json.SetFloatField(ABILITY_DO_JSON_FIELD_RELATIVE_SPEED, RelativeStats.Speed);

	Json.SetObjectField(ABILITY_DO_JSON_FIELD_ABILITY_SPECIFIC, BP_AbilityDataToJson());

	return Json;
}

FJsonObjectBP UAbilityDataObject::BP_AbilityDataToJson_Implementation()
{
	return FJsonObjectBP();
}