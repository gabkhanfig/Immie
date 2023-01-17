// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDataTypes.h"

/* Simple load of a boolean json flag using the flag name. */
#define LOAD_JSON_FLAG(FlagName) \
Flags.##FlagName = GetJsonAbilityFlag(JsonObject, #FlagName)

/* Simple set of a boolean json field using the flag name. */
#define SET_JSON_FLAG(FlagName) \
Json.SetBoolField(#FlagName, FlagName)

FAbilityFlags FAbilityFlags::LoadJsonAbilityFlags(const FJsonObjectBP& JsonObject)
{
    FAbilityFlags Flags;

    LOAD_JSON_FLAG(DoesDamage);
    LOAD_JSON_FLAG(DoesHealing);
    LOAD_JSON_FLAG(SameTypeImmie);
    LOAD_JSON_FLAG(BaseStats);
    LOAD_JSON_FLAG(RelativeStats);
    LOAD_JSON_FLAG(VisualDummy);
    LOAD_JSON_FLAG(Projectile);

    return Flags;
}

FJsonObjectBP FAbilityFlags::ToJson()
{
    FJsonObjectBP Json;

    SET_JSON_FLAG(DoesDamage);
    SET_JSON_FLAG(DoesHealing);
    SET_JSON_FLAG(SameTypeImmie);
    SET_JSON_FLAG(BaseStats);
    SET_JSON_FLAG(RelativeStats);
    SET_JSON_FLAG(VisualDummy);
    SET_JSON_FLAG(Projectile);

    return Json;
}

bool FAbilityFlags::GetJsonAbilityFlag(const FJsonObjectBP& JsonObject, const FString& FlagName)
{
    bool Flag = false;
    JsonObject.TryGetBoolField(FlagName, Flag);
    return Flag;
}
