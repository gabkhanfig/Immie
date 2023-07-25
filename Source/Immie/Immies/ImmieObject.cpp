// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieObject.h"
#include <Immie/Game/Global/Managers/TypeDataManager.h>
#include <Immie/Game/Global/Managers/AbilityDataManager.h>
#include <Immie/Game/Global/Managers/SpecieDataManager.h>
#include <Immie/Game/Global/Managers/ConfigDataManager.h>
#include <Immie/Immies/SpecieDataObject.h>
#include <Immie/ImmieCore.h>

#define JSON_FIELD_SPECIE "Specie"
#define JSON_FIELD_DISPLAY_NAME "DisplayName"
#define JSON_FIELD_XP "Xp"
#define JSON_FIELD_HEALTH "Health"
#define JSON_FIELD_HEALTH_LEVEL "HealthLevel"
#define JSON_FIELD_ATTACK_LEVEL "AttackLevel"
#define JSON_FIELD_DEFENSE_LEVEL "DefenseLevel"
#define JSON_FIELD_SPEED_LEVEL "SpeedLevel"
#define JSON_FIELD_ABILITIES "Abilities"
#define JSON_FIELD_ABILITIES_ELEMENT "Ability"
#define JSON_FIELD_SPECIE_SPECIFIC "SpecieSpecific"

UImmie::UImmie()
{
    Health = 0;
    Xp = 1;
    StatLevels = { 0, 0, 0, 0 };
    Abilities.Reserve(MAX_ABILITY_COUNT);
}

void UImmie::PostLoad()
{
  const FName ClassName = GetClass()->GetFName();
  if (ClassName == "Immie" || ClassName == "ImmieObject_BP_C") {
    Super::PostLoad();
    return;
  }

  SpecieName = USpecieDataManager::SpecieNameFromBlueprintClassName(ClassName.ToString(), "Object_C");

  Super::PostLoad();
}

UImmie* UImmie::NewImmieObject(UObject* Outer, FName _SpecieName)
{
  check(Outer);
  UClass* ImmieObjectClass = GetSpecieDataManager()->GetSpecieDataObject(_SpecieName)->GetImmieObjectClass();
  check(ImmieObjectClass);
  UImmie* Immie = NewObject<UImmie>(Outer, ImmieObjectClass);
  return Immie;
}

void UImmie::LoadJsonData(const FJsonObjectBP& Json)
{
    bool SetDisplayNameToSpecie = false;

    FString _DisplayName;
    if (Json.TryGetStringField(JSON_FIELD_DISPLAY_NAME, _DisplayName, false)) {
        if (_DisplayName == "") {
            SetDisplayNameToSpecie = true;
        }
        else {
            DisplayName = std::move(_DisplayName);
        }
    }
    else {
        SetDisplayNameToSpecie = true;
    }
    if (SetDisplayNameToSpecie) {
        DisplayName = Json.GetStringField(JSON_FIELD_SPECIE);
    }

    const int MaxXp = GetConfigDataManager()->GetMaxXp();
    Json.TryGetIntegerField(JSON_FIELD_XP, Xp, true, 0);
    if (Xp < 1) {
      ULogger::Log("Immie object xp in json is lower than 1", LogVerbosity_Error);
      Xp = 1;
    }
    if (Xp > MaxXp) {
      ULogger::Log("Immie object xp in json is greater than " + FString::FromInt(MaxXp), LogVerbosity_Error);
      Xp = MaxXp;
    }
        

    const uint8 MaxStatLevel = GetConfigDataManager()->GetMaxStatLevel();

    int _StatLevelHealth;
    Json.TryGetIntegerField(JSON_FIELD_HEALTH_LEVEL, _StatLevelHealth, true, 0);
    if (_StatLevelHealth > MaxStatLevel) {
      ULogger::Log("Immie object json health stat level is greater than " + FString::FromInt(MaxStatLevel), LogVerbosity_Error);
      StatLevels.Health = MaxStatLevel;
    }
    else if (_StatLevelHealth < 0) {
      ULogger::Log("Immie object json health stat level is not allowed to be negative", LogVerbosity_Error);
      StatLevels.Health = 0;
    }
    else {
      StatLevels.Health = _StatLevelHealth;
    }

    int _StatLevelAttack;
    Json.TryGetIntegerField(JSON_FIELD_ATTACK_LEVEL, _StatLevelAttack, true, 0);
    if (_StatLevelAttack > MaxStatLevel) {
      ULogger::Log("Immie object json attack stat level is greater than " + FString::FromInt(MaxStatLevel), LogVerbosity_Error);
      StatLevels.Attack = MaxStatLevel;
    }
    else if (_StatLevelAttack < 0) {
      ULogger::Log("Immie object json attack stat level is not allowed to be negative", LogVerbosity_Error);
      StatLevels.Attack = 0;
    }
    else {
      StatLevels.Attack = _StatLevelAttack;
    }

    int _StatLevelDefense;
    Json.TryGetIntegerField(JSON_FIELD_DEFENSE_LEVEL, _StatLevelDefense, true, 0);
    if (_StatLevelDefense > MaxStatLevel) {
      ULogger::Log("Immie object json defense stat level is greater than " + FString::FromInt(MaxStatLevel), LogVerbosity_Error);
      StatLevels.Defense = MaxStatLevel;
    }
    else if (_StatLevelDefense < 0) {
      ULogger::Log("Immie object json defense stat level is not allowed to be negative", LogVerbosity_Error);
      StatLevels.Defense = 0;
    }
    else {
      StatLevels.Defense = _StatLevelDefense;
    }

    int _StatLevelSpeed;
    Json.TryGetIntegerField(JSON_FIELD_SPEED_LEVEL, _StatLevelSpeed, true, 0);
    if (_StatLevelHealth > MaxStatLevel) {
      ULogger::Log("Immie object json speed stat level is greater than " + FString::FromInt(MaxStatLevel), LogVerbosity_Error);
      StatLevels.Speed = MaxStatLevel;
    }
    else if (_StatLevelSpeed < 0) {
      ULogger::Log("Immie object json speed stat level is not allowed to be negative", LogVerbosity_Error);
      StatLevels.Speed = 0;
    }
    else {
      StatLevels.Speed = _StatLevelSpeed;
    }

    // Below the other stat stuff due to the max health calculation requiring them.
    const float MaxHealth = GetMaxHealth();
    Json.TryGetFloatField(JSON_FIELD_HEALTH, Health, true, -1);
    if (Health < 0)
        Health = MaxHealth;
    if (Health > MaxHealth)
        Health = MaxHealth;

    FJsonArrayBP AbilityJsonArray;
    if (Json.TryGetArrayField(JSON_FIELD_ABILITIES, AbilityJsonArray)) {
        TArray<FString> FoundAbilities = AbilityJsonArray.GetStringArray(JSON_FIELD_ABILITIES_ELEMENT);

        for (int i = 0; i < MAX_ABILITY_COUNT; i++) {
            const FString AbilityName = FoundAbilities.IsValidIndex(i) ? FoundAbilities[i] : "emptyAbility";
            if (UAbilityDataManager::IsValidAbilityNameString(AbilityName)) {
                Abilities.Add(FName(AbilityName));
            }
            else {
                iLog("Parsed ability in Immie object json " + AbilityName + " does not have a valid ability id", LogVerbosity_Error);
            }
        }
    }

    FJsonObjectBP SpecieSpecific;
    if (Json.TryGetObjectField(JSON_FIELD_SPECIE_SPECIFIC, SpecieSpecific)) {
        BP_LoadJsonData(SpecieSpecific);
    }
}

FJsonObjectBP UImmie::SaveJsonData()
{
    FJsonObjectBP JsonWriter;

    JsonWriter.SetStringField(JSON_FIELD_SPECIE, SpecieName.ToString());
    JsonWriter.SetStringField(JSON_FIELD_DISPLAY_NAME, DisplayName);
    JsonWriter.SetIntegerField(JSON_FIELD_XP, Xp);
    JsonWriter.SetFloatField(JSON_FIELD_HEALTH, Health);
    JsonWriter.SetIntegerField(JSON_FIELD_HEALTH_LEVEL, StatLevels.Health);
    JsonWriter.SetIntegerField(JSON_FIELD_ATTACK_LEVEL, StatLevels.Attack);
    JsonWriter.SetIntegerField(JSON_FIELD_DEFENSE_LEVEL, StatLevels.Defense);
    JsonWriter.SetIntegerField(JSON_FIELD_SPEED_LEVEL, StatLevels.Speed);
    
    FJsonArrayBP AbilitiesJson;
    TArray<FString> AbilityNames;

    const int AbilityCount = Abilities.Num();
    AbilityNames.Reserve(AbilityCount);
    for (int i = 0; i < AbilityCount; i++) {
        AbilityNames.Add(Abilities[i].ToString());
    }

    AbilitiesJson.SetStringArray(JSON_FIELD_ABILITIES_ELEMENT, AbilityNames);
    JsonWriter.SetArrayField(JSON_FIELD_ABILITIES, AbilitiesJson);
    
    JsonWriter.SetObjectField(JSON_FIELD_SPECIE_SPECIFIC, BP_SaveJsonData());
    return JsonWriter;
}

UImmie* UImmie::MakeCopy(UObject* Outer)
{
  UImmie* Copy = UImmie::NewImmieObject(Outer, SpecieName);
  Copy->Health = Health;
  Copy->Xp = Xp;
  Copy->StatLevels = StatLevels;
  Copy->Abilities = Abilities;
  Copy->DisplayName = DisplayName;
  BP_MakeCopy(Copy);
  return Copy;
}

TArray<UImmie*> UImmie::MakeTeamCopy(const TArray<UImmie*>& Team, UObject* Outer)
{
  ULogger::Log("making team copy");
  TArray<UImmie*> Copy;
  Copy.Reserve(Team.Num());
  for (int i = 0; i < Team.Num(); i++) {
    ULogger::Log("making copy number: " + FString::FromInt(i) + " " + Team[i]->GetSpecieName().ToString());
    Copy.Add(Team[i]->MakeCopy(Outer));
  }
  return Copy;
}

FJsonObjectBP UImmie::BP_SaveJsonData_Implementation()
{
    return FJsonObjectBP();
}

FJsonObjectBP UImmie::TeamToJson(const TArray<UImmie*>& Team, const FString& ArrayFieldName)
{
    FJsonObjectBP Root;

    const int Count = Team.Num();
    TArray<FJsonObjectBP> TeamMembers;
    TeamMembers.Reserve(Count);
    for (int i = 0; i < Count; i++) {
        TeamMembers.Add(Team[i]->SaveJsonData());
    }
    FJsonArrayBP JsonArrayTeam = FJsonObjectBP::JsonArrayFromObjects(TeamMembers);
    Root.SetArrayField(ArrayFieldName, JsonArrayTeam);

    return Root;
}

TArray<UImmie*> UImmie::JsonToTeam(const FJsonObjectBP& TeamJsonObject, const FString& ArrayFieldName, UObject* Outer)
{
    TArray<UImmie*> Team;
    TArray<FJsonObjectBP> JsonTeam = FJsonObjectBP::ArrayFromObjects(TeamJsonObject.GetArrayField(ArrayFieldName).GetObjectArray());
    for (int i = 0; i < JsonTeam.Num(); i++) {

        FString ParsedSpecieString;
        if (!JsonTeam[i].TryGetStringField("Specie", ParsedSpecieString, false)) {
            iLog("Immie json must have a specie string field!", LogVerbosity_Error);
            continue;
        }

        if (!USpecieDataManager::IsValidSpecieNameString(ParsedSpecieString)) {
            iLog("Parsed specie name " + ParsedSpecieString + " is not a valid Immie specie", LogVerbosity_Error);
            continue;
        }

        UImmie* Immie = UImmie::NewImmieObject(Outer, FName(ParsedSpecieString));
        Immie->LoadJsonData(JsonTeam[i]);
        Team.Add(Immie);
    }

    return Team;
}

void UImmie::ChangeOuter(UObject* NewOuter)
{
    Rename(nullptr, NewOuter);
}

void UImmie::Release()
{
    ConditionalBeginDestroy();
}

void UImmie::Heal()
{
    HealFromBaseStats(GetSpecieDataManager()->GetSpecieDataObject(SpecieName)->GetBaseStats().Health);
}

void UImmie::HealFromBaseStats(uint8 HealthBaseStat)
{
    Health = UFormula::HealthStat(HealthBaseStat, GetLevel(), StatLevels.Health);
}

void UImmie::SetHealth(float NewHealth)
{
    const float MaxHealth = GetMaxHealth();
    if (NewHealth > MaxHealth)
        Health = MaxHealth;
    else if (NewHealth < 0) {
        Health = 0;
    }
    else {
        Health = NewHealth;
    }
}

void UImmie::SetAbilityNames(const TArray<FName>& NewAbilities)
{
  for (FName Name : NewAbilities) {
    checkf(UAbilityDataManager::IsValidAbilityName(Name), TEXT("Ability name %s is not a valid ability name"), *Name.ToString());
  }
  Abilities = NewAbilities;
}

void UImmie::SetDisplayName(const FString& _DisplayName)
{
    DisplayName = _DisplayName;
}

void UImmie::SetXp(int NewXp)
{
    // TODO check max xp amount
    Xp = NewXp;
    if (Xp < 0) {
        Xp = 0;
    }
}

uint8 UImmie::GetLevel() const
{
    return UFormula::LevelFromXp(GetXp());
}

void UImmie::SetLevel(uint8 NewLevel)
{
    if (NewLevel < 1) {
        Xp = 0;
        return;
    }
    // TODO check max level
    Xp = UFormula::MinimumXpForLevel(NewLevel);
    
}

float UImmie::GetMaxHealth(bool UseBaseStatOverride, uint8 BaseHealth) const
{
    if (!UseBaseStatOverride) {
        BaseHealth = GetSpecieDataManager()->GetSpecieDataObject(SpecieName)->GetBaseStats().Health;
    }

    return UFormula::HealthStat(BaseHealth, GetLevel(), StatLevels.Health);
}

float UImmie::GetAttack(bool UseBaseStatOverride, uint8 BaseAttack) const
{
    if (!UseBaseStatOverride) {
        BaseAttack = GetSpecieDataManager()->GetSpecieDataObject(SpecieName)->GetBaseStats().Attack;
    }

    return UFormula::AttackStat(BaseAttack, GetLevel(), StatLevels.Attack);
}

float UImmie::GetDefense(bool UseBaseStatOverride, uint8 BaseDefense) const
{
    if (!UseBaseStatOverride) {
        BaseDefense = GetSpecieDataManager()->GetSpecieDataObject(SpecieName)->GetBaseStats().Defense;
    }

    return UFormula::DefenseStat(BaseDefense, GetLevel(), StatLevels.Defense);
}

float UImmie::GetSpeed(bool UseBaseStatOverride, uint8 BaseSpeed) const
{
    if (!UseBaseStatOverride) {
        BaseSpeed = GetSpecieDataManager()->GetSpecieDataObject(SpecieName)->GetBaseStats().Speed;
    }

    return UFormula::SpeedStat(BaseSpeed, StatLevels.Speed);
}

FBattleStats UImmie::GetStats(bool UseBaseStatOverride, uint8 BaseHealth, uint8 BaseAttack, uint8 BaseDefense, uint8 BaseSpeed) const
{
    FBattleStats Stats;
    Stats.Health = GetMaxHealth(UseBaseStatOverride, BaseHealth);
    Stats.Attack = GetAttack(UseBaseStatOverride, BaseAttack);
    Stats.Defense = GetDefense(UseBaseStatOverride, BaseDefense);
    Stats.Speed = GetSpeed(UseBaseStatOverride, BaseSpeed);
    return Stats;
}
