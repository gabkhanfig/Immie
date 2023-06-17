#include "../ImmieTest.h"
#include "../../Game/Global/Managers/SpecieDataManager.h"

#if WITH_IMMIE_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecieNamesTest, "Immie.Immies.SpecieNames", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecieNamesTest::RunTest(const FString& Parameters) {
	const TArray<FName> SpecieNames = USpecieDataManager::GetAllSpecieNames();
	if (SpecieNames.Num() == 0) {
		AddError("USpecieDataManager::GetAllSpecieNames() must return an array with at least 1 specie name");
	}
	for (int i = 0; i < SpecieNames.Num(); i++) {
		if (!USpecieDataManager::IsValidSpecieName(SpecieNames[i])) {
			AddError("Couldn't find specie name " + SpecieNames[i].ToString() + " in USpecieDataManager::IsValidSpecieName()");
		}
	}
	const TSet<FName> SpecieNamesSet;
	for (FName SpecieNameInSet : USpecieDataManager::GetSetOfSpecieNames()) {
		if (!SpecieNames.Contains(SpecieNameInSet)) {
			AddError("Set of Specie Names contains the entry of " + SpecieNameInSet.ToString() + " that is not in array of Specie Names");
		}
	}

	return true;
}

#endif