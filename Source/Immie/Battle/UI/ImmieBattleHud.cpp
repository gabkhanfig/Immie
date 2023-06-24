// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieBattleHud.h"
#include <Immie/Immies/ImmieCharacter.h>
#include <Immie/Immies/ImmieObject.h>
#include <Immie/Ability/Ability.h>
#include <Immie/Type/ImmieType.h>
#include <Immie/Type/BattleTypeComponent.h>

UImmieBattleHud::UImmieBattleHud(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FullHealthbar = FLinearColor(0, 1, 0.26);
	HalfHealthbar = FLinearColor(0.95, 0.7, 0.1);
	QuarterHealthbar = FLinearColor(1, 0.04, 0.06);
	NoneHealthbar = FLinearColor(0.7, 0.01, 0.02);
}

void UImmieBattleHud::SetImmieCharacter(AImmieCharacter* _ImmieCharacter)
{
	ImmieCharacter = _ImmieCharacter;
}

float UImmieBattleHud::GetHealthPercent() const
{
	const float MaxHealth = ImmieCharacter->GetMaxHealth();
	const float CurrentHealth = ImmieCharacter->GetCurrentHealth();
	const float Percent = CurrentHealth / MaxHealth;
	//checkf(Percent >= 0.f && Percent <= 1.f, TEXT("Immie character health must percent must be between 0 and 1"));
	return Percent;
}

FString UImmieBattleHud::GetDisplayName() const
{
	return ImmieCharacter->GetImmieObject()->GetDisplayName();
}

FString UImmieBattleHud::GetCurrentHealthString()
{
	const float CurrentHealth = ImmieCharacter->GetCurrentHealth();
	return FString::FromInt(int(CurrentHealth));
}

FString UImmieBattleHud::GetMaxHealthString()
{
	const float MaxHealth = ImmieCharacter->GetMaxHealth();
	return FString::FromInt(int(MaxHealth));
}

FLinearColor UImmieBattleHud::GetTypeColor(UBattleTypeComponent* Type) const
{
	return Type->GetColor();
}

UAbility* UImmieBattleHud::GetAbility(int AbilityIndex) const
{
	TArray<UAbility*> Abilities = ImmieCharacter->GetAbilities();
	checkf(Abilities.IsValidIndex(AbilityIndex), TEXT("Immie character abilities index is not in range in battle hud"));
	return Abilities[AbilityIndex];
}

FString UImmieBattleHud::GetAbilityName(const UAbility* Ability) const
{
	return FName::NameToDisplayString(Ability->GetAbilityName().ToString(), false);
}

bool UImmieBattleHud::IsAbilityOnCooldown(const UAbility* Ability) const
{
	const bool OnCooldown = Ability->GetCurrentCooldown() > 0.f;
	return OnCooldown;
}

float UImmieBattleHud::GetAbilityCooldownPercent(const UAbility* Ability) const
{
	const float MaxCooldown = Ability->GetMaxCooldown();
	const float CurrentCooldown = Ability->GetCurrentCooldown();
	const float Percent = CurrentCooldown / MaxCooldown;
	return Percent;
}

int UImmieBattleHud::GetAbilityCurrentUses(const UAbility* Ability) const
{
	return Ability->GetCurrentUses();
}

int UImmieBattleHud::GetAbilityMaxUses(const UAbility* Ability) const
{
	return Ability->GetMaxUses();
}

FString UImmieBattleHud::GetAbilityCurrentCooldownString(const UAbility* Ability) const
{
	const float CurrentCooldown = Ability->GetCurrentCooldown();
	return FString::FromInt(int(CurrentCooldown));
}

FLinearColor UImmieBattleHud::GetColorForImmieHealth() const
{
	return GetColorForHealthPercent(GetHealthPercent());
}

FLinearColor UImmieBattleHud::GetColorForHealthPercent(float Percent) const
{
	if (Percent > 0.5) {
		return FLinearColor::LerpUsingHSV(HalfHealthbar, FullHealthbar, (Percent - 0.5) * 2);
	}
	else if (Percent > 0.25) {
		return FLinearColor::LerpUsingHSV(QuarterHealthbar, HalfHealthbar, (Percent - 0.25) * 4);
	}
	else if (Percent > 0) {
		return FLinearColor::LerpUsingHSV(NoneHealthbar, QuarterHealthbar, Percent * 4);
	}
	else {
		return NoneHealthbar;
	}
}
