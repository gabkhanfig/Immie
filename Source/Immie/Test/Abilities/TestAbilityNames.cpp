#include "../ImmieTest.h"
#include "../../Game/Global/Managers/AbilityDataManager.h"

#if WITH_IMMIE_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAbilityNamesTest, "Immie.Abilities.AbilityNames", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAbilityNamesTest::RunTest(const FString& Parameters) {
	const TArray<FName> AbilityNames = UAbilityDataManager::GetAllAbilityNames();
	if (AbilityNames.Num() == 0) {
		AddError("UAbilityDataManager::GetAllAbilityNames() must return an array with at least 1 ability name");
	}
	for (int i = 0; i < AbilityNames.Num(); i++) {
		if (!UAbilityDataManager::IsValidAbilityName(AbilityNames[i])) {
			AddError("Couldn't find ability name " + AbilityNames[i].ToString() + " in UAbilityDataManager::IsValidAbilityName()");
		}
		if (!UAbilityDataManager::IsValidAbilityNameString(AbilityNames[i].ToString())) {
			AddError("Couldn't find ability name " + AbilityNames[i].ToString() + " as an FString in UAbilityDataManager::IsValidAbilityNameString()");
		}
	}
	const TSet<FName> AbilityNamesSet;
	for (FName AbilityNameInSet : UAbilityDataManager::GetSetOfAbilityNames()) {
		if (!AbilityNames.Contains(AbilityNameInSet)) {
			AddError("Set of Ability Names contains the entry of " + AbilityNameInSet.ToString() + " that is not in array of ability Names");
		}
	}
	return true;
}

#endif