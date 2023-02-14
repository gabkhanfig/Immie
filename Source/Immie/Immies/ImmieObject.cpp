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
    SpecieId = INVALID_SPECIE_ID;
    Health = 0;
    Xp = 1;
    StatLevels = { 0, 0, 0, 0 };
    AbilityIds.Reserve(MAX_ABILITY_COUNT);
    AbilityIds.Init(0, MAX_ABILITY_COUNT);
}

UImmie* UImmie::NewImmieObject(UObject* Outer, int _SpecieId)
{
    UImmie* Immie = NewObject<UImmie>(Outer, GetSpecieDataManager()->GetImmieObjectClass(_SpecieId));
    Immie->SpecieId = _SpecieId;
    return Immie;
}

void UImmie::LoadJsonData(int _SpecieId, const FJsonObjectBP& Json)
{
    SpecieId = _SpecieId;

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

        const int ElementCount = FGenericPlatformMath::Min(FoundAbilities.Num(), MAX_ABILITY_COUNT);
        for (int i = 0; i < ElementCount; i++) {
            const int FoundAbilityId = GetAbilityDataManager()->GetAbilityId(FName(FoundAbilities[i]));
            if (GetAbilityDataManager()->IsValidAbility(FoundAbilityId)) {
                // Should always be a valid index
                AbilityIds[i] = (FoundAbilityId);
            }
            else {
                iLog("Parsed ability in Immie object json " + FoundAbilities[i] + " does not have a valid ability id", LogVerbosity_Error);
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

    JsonWriter.SetStringField(JSON_FIELD_SPECIE, GetSpecieName().ToString());
    JsonWriter.SetStringField(JSON_FIELD_DISPLAY_NAME, DisplayName);
    JsonWriter.SetIntegerField(JSON_FIELD_XP, Xp);
    JsonWriter.SetFloatField(JSON_FIELD_HEALTH, Health);
    JsonWriter.SetIntegerField(JSON_FIELD_HEALTH_LEVEL, StatLevels.Health);
    JsonWriter.SetIntegerField(JSON_FIELD_ATTACK_LEVEL, StatLevels.Attack);
    JsonWriter.SetIntegerField(JSON_FIELD_DEFENSE_LEVEL, StatLevels.Defense);
    JsonWriter.SetIntegerField(JSON_FIELD_SPEED_LEVEL, StatLevels.Speed);
    
    FJsonArrayBP AbilitiesJson;
    TArray<FString> AbilityNames;

    const int AbilityCount = AbilityIds.Num();
    AbilityNames.Reserve(AbilityCount);
    for (int i = 0; i < AbilityCount; i++) {
        AbilityNames.Add(GetAbilityDataManager()->GetAbilityName(AbilityIds[i]).ToString());
    }

    AbilitiesJson.SetStringArray(JSON_FIELD_ABILITIES_ELEMENT, AbilityNames);
    JsonWriter.SetArrayField(JSON_FIELD_ABILITIES, AbilitiesJson);
    
    JsonWriter.SetObjectField(JSON_FIELD_SPECIE_SPECIFIC, BP_SaveJsonData());
    return JsonWriter;
}

UImmie* UImmie::MakeCopy(UObject* Outer)
{
  UImmie* Copy = UImmie::NewImmieObject(Outer, SpecieId);
  Copy->Health = Health;
  Copy->Xp = Xp;
  Copy->StatLevels = StatLevels;
  Copy->AbilityIds = AbilityIds;
  Copy->DisplayName = DisplayName;
  BP_MakeCopy(Copy);
  return Copy;
}

TArray<UImmie*> UImmie::MakeTeamCopy(const TArray<UImmie*>& Team, UObject* Outer)
{
  TArray<UImmie*> Copy;
  Copy.Reserve(Team.Num());
  for (int i = 0; i < Team.Num(); i++) {
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

        FName ParsedSpecieName = FName(ParsedSpecieString);
        const int SpecieId = GetSpecieDataManager()->GetSpecieId(ParsedSpecieName);
        if (!GetSpecieDataManager()->IsValidSpecie(SpecieId)) {
            iLog("Parsed specie name " + ParsedSpecieName.ToString() + " is not a valid Immie specie", LogVerbosity_Error);
            continue;
        }

        UImmie* Immie = UImmie::NewImmieObject(Outer, SpecieId);
        Immie->LoadJsonData(SpecieId, JsonTeam[i]);
        Team.Add(Immie);
    }

    return Team;
}

void UImmie::ChangeOuter(UObject* NewOuter)
{
    Rename(*GetFName().ToString(), NewOuter);
}

void UImmie::Release()
{
    ConditionalBeginDestroy();
}

void UImmie::Heal()
{
    HealFromBaseStats(GetSpecieDataManager()->GetSpecieBaseStats(SpecieId).Health);
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

void UImmie::SetDisplayName(const FString& _DisplayName)
{
    DisplayName = _DisplayName;
}

FName UImmie::GetSpecieName() const
{
    return GetSpecieDataManager()->GetSpecieName(SpecieId);
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
        BaseHealth = GetSpecieDataManager()->GetSpecieBaseStats(SpecieId).Health;
    }

    return UFormula::HealthStat(BaseHealth, GetLevel(), StatLevels.Health);
}

float UImmie::GetAttack(bool UseBaseStatOverride, uint8 BaseAttack) const
{
    if (!UseBaseStatOverride) {
        BaseAttack = GetSpecieDataManager()->GetSpecieBaseStats(SpecieId).Attack;
    }

    return UFormula::AttackStat(BaseAttack, GetLevel(), StatLevels.Attack);
}

float UImmie::GetDefense(bool UseBaseStatOverride, uint8 BaseDefense) const
{
    if (!UseBaseStatOverride) {
        BaseDefense = GetSpecieDataManager()->GetSpecieBaseStats(SpecieId).Defense;
    }

    return UFormula::DefenseStat(BaseDefense, GetLevel(), StatLevels.Defense);
}

float UImmie::GetSpeed(bool UseBaseStatOverride, uint8 BaseSpeed) const
{
    if (!UseBaseStatOverride) {
        BaseSpeed = GetSpecieDataManager()->GetSpecieBaseStats(SpecieId).Speed;
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
