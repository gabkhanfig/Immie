// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingBattleHealthbar.h"
#include <Immie/Type/ImmieType.h>
#include "ImmieBattleHud.h"

UFloatingBattleHealthbar::UFloatingBattleHealthbar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FullHealthbar = FLinearColor(0.95, 0.002, 0.01);
	HalfHealthbar = FLinearColor(0.72, 0.002, 0.015);
	QuarterHealthbar = FLinearColor(0.47, 0.002, 0.01);
	NoneHealthbar = FLinearColor(0.15, 0, 0.015);
}

void UFloatingBattleHealthbar::SetBattleActor(TScriptInterface<IBattleActor> NewBattleActor)
{
	checkf(IsValid(NewBattleActor.GetObject()), TEXT("Cannot set battle actor for floating healthbar to an invalid battle actor"));
	BattleActor = NewBattleActor;
}

float UFloatingBattleHealthbar::GetCurrentHealth() const
{
	return BattleActor->GetActiveStats().Health;
}

float UFloatingBattleHealthbar::GetMaxHealth() const
{
	return BattleActor->GetInitialStats().Health;
}

float UFloatingBattleHealthbar::GetHealthPercent() const
{
	return GetCurrentHealth() / GetMaxHealth();
}

FString UFloatingBattleHealthbar::GetDisplayName() const
{
	return BattleActor->GetDisplayName();
}

FLinearColor UFloatingBattleHealthbar::GetTypeColor(UImmieType* Type) const
{
	return Type->GetColor();
}

FLinearColor UFloatingBattleHealthbar::GetColorHealthPercent(float Percent) const
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
