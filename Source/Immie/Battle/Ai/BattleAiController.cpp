// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAiController.h"
#include "../../Immies/ImmieCharacter.h"
#include "../Team/BattleTeam.h"
#include "../BattleInstance/BattleInstance.h"
#include "../../Ability/Ability.h"
#include <Kismet/KismetMathLibrary.h>

ABattleAiController::ABattleAiController() :
	_ImmieCharacter(nullptr), _Target(nullptr)
{
}

void ABattleAiController::Tick(float DeltaTime)
{
	if (!HasAuthority() || !_ImmieCharacter->IsImmieEnabled()) return;

	if (!IsValid(_Target) || !_Target->IsImmieEnabled()) {
		_Target = FindNearestTarget();
		check(IsValid(_Target));
	}

	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(_ImmieCharacter->GetActorLocation(), _Target->GetActorLocation());
	_ImmieCharacter->SetActorRotation(DesiredRotation);

	for (UAbility* Ability : _Abilities) {
		if (!Ability->CanAbilityBeUsed()) continue;
		Ability->InputPress();
	}
}

void ABattleAiController::Initialize(AImmieCharacter* ImmieCharacter)
{
	_ImmieCharacter = ImmieCharacter;

	const FName EmptyAbilityName = FName("EmptyAbility");

	TArray<UAbility*> Abilities = _ImmieCharacter->GetAbilities();
	for (UAbility* Ability : Abilities) {
		check(IsValid(Ability));
		if (Ability->GetAbilityName() == EmptyAbilityName) continue;
		
		_Abilities.Add(Ability);
	}
}

AImmieCharacter* ABattleAiController::FindNearestTarget() const
{
	const ABattleTeam* ThisTeam = GetBattleTeam();
	ABattleInstance* BattleInstance = GetBattleInstance();
	TArray<ABattleTeam*> Teams = BattleInstance->GetTeams();

	TArray<AImmieCharacter*> PossibleTargets;
	for (ABattleTeam* Team : Teams) {
		if (Team == ThisTeam) continue;
		PossibleTargets.Add(Team->GetActiveImmie());
	}

	checkf(PossibleTargets.Num() > 0, TEXT("Cannot find any targets for a battle ai controller"));

	if (PossibleTargets.Num() == 1) {
		return PossibleTargets[0];
	}

	PossibleTargets.Sort([this](const AImmieCharacter& A, const AImmieCharacter& B) {
		float DistanceA = A.GetDistanceTo(_ImmieCharacter);
		float DistanceB = B.GetDistanceTo(_ImmieCharacter);
		return DistanceA < DistanceB;
	});

	return PossibleTargets[0];
}

ABattleTeam* ABattleAiController::GetBattleTeam() const
{
	return IBattleActor::Execute_GetTeam(_ImmieCharacter);
}

ABattleInstance* ABattleAiController::GetBattleInstance() const
{
	return _ImmieCharacter->GetBattleInstance();
}
