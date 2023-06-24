#include "../ImmieTest.h"
#include "../../Type/ImmieType.h"

#if WITH_IMMIE_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FTypeBitmaskTest, "Immie.Type.Bitmasks", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
END_DEFINE_SPEC(FTypeBitmaskTest)

#define TEST_SINGULAR_TYPE(Enum, String) \
Describe(String, [this]() {\
	It(String " Add Type", [this]() {\
		FTypeBitmask Bitmask;\
		Bitmask.AddType(Enum);\
		TestTrue("Bitmask does not contain type: " String, Bitmask.Bitmask & (1 << static_cast<int>(Enum)));\
		});\
It(String " Remove Type", [this]() {\
	FTypeBitmask Bitmask;\
	Bitmask.Bitmask = (1 << static_cast<int>(Enum));\
	Bitmask.RemoveType(Enum);\
	TestTrue("Type was not removed from bitmask: " String, Bitmask.Bitmask == 0);\
	});\
It(String " Has Type", [this]() {\
	FTypeBitmask Bitmask;\
	Bitmask.AddType(Enum);\
	TestTrue("Bitmask does not contain type: " String, Bitmask.HasType(Enum));\
	});\
It(String " Get Array Containing Type", [this]() {\
	FTypeBitmask Bitmask;\
	Bitmask.AddType(Enum);\
	const TArray<EImmieType> Types = Bitmask.GetTypes();\
	TestTrue("FTypeBitmask::GetTypes() returned an array of size 0 when it should have been 1", Types.Num() > 0);\
	TestTrue("Array index 0 must be type: " String, Types[0] == Enum);\
	});\
It(String " Set Type Bitmask From Array", [this]() {\
	FTypeBitmask Bitmask;\
	Bitmask.AddTypes({ Enum });\
	TestTrue("Bitmask does not contain type from array: " String, Bitmask.HasType(Enum));\
	});\
It(String " Type Count", [this]() {\
		FTypeBitmask Bitmask;\
		Bitmask.AddType(Enum);\
		TestTrue("Bitmask must have a type count of 1: " String, Bitmask.TypeCount() == 1);\
		});\
	}); // Describe Type

void FTypeBitmaskTest::Define() {
	TEST_SINGULAR_TYPE(EImmieType::Neutral, "Neutral");
	TEST_SINGULAR_TYPE(EImmieType::Fire, "Fire");
	TEST_SINGULAR_TYPE(EImmieType::Water, "Water");
	TEST_SINGULAR_TYPE(EImmieType::Nature, "Nature");
	TEST_SINGULAR_TYPE(EImmieType::Standard, "Standard");
	TEST_SINGULAR_TYPE(EImmieType::Electric, "Electric");
	TEST_SINGULAR_TYPE(EImmieType::Air, "Air");
	TEST_SINGULAR_TYPE(EImmieType::Ground, "Ground");
	TEST_SINGULAR_TYPE(EImmieType::Metal, "Metal");
	TEST_SINGULAR_TYPE(EImmieType::Light, "Light");
	TEST_SINGULAR_TYPE(EImmieType::Dark, "Dark");
	TEST_SINGULAR_TYPE(EImmieType::Dragon, "Dragon");

	Describe("Sanity Tests", [this]() {
		It("Clear Type", [this]() {
			FTypeBitmask Bitmask;
			Bitmask.AddType(EImmieType::Light);
			Bitmask.AddType(EImmieType::Standard);
			Bitmask.Clear();
			TestTrue("Clearing bitmask must make it have 0 bits set", Bitmask.TypeCount() == 0);
		});
	});

	Describe("Multiple Types", [this]() {
		It("Add Two Types", [this]() {
			FTypeBitmask Bitmask;
			Bitmask.AddType(EImmieType::Fire);
			Bitmask.AddType(EImmieType::Ground);
			TestTrue("Bitmask must contain fire type", Bitmask.HasType(EImmieType::Fire));
			TestTrue("Bitmask must contain ground type", Bitmask.HasType(EImmieType::Ground));
		});
		It("Add Two Types, Remove One", [this]() {
			FTypeBitmask Bitmask;
			Bitmask.AddType(EImmieType::Fire);
			Bitmask.AddType(EImmieType::Ground);
			Bitmask.RemoveType(EImmieType::Fire);
			TestTrue("Bitmask must NOT contain fire type", !Bitmask.HasType(EImmieType::Fire));
			TestTrue("Bitmask must contain ground type", Bitmask.HasType(EImmieType::Ground));
		});
		It("Add Two Types, Remove Two", [this]() {
			FTypeBitmask Bitmask;
			Bitmask.AddType(EImmieType::Fire);
			Bitmask.AddType(EImmieType::Ground);
			Bitmask.RemoveType(EImmieType::Fire);
			Bitmask.RemoveType(EImmieType::Ground);
			TestTrue("Bitmask must NOT contain fire type", !Bitmask.HasType(EImmieType::Fire));
			TestTrue("Bitmask must NOT contain ground type", !Bitmask.HasType(EImmieType::Ground));
		});
		It("Array Containing 3 Types", [this]() {
			FTypeBitmask Bitmask;
			Bitmask.AddType(EImmieType::Water);
			Bitmask.AddType(EImmieType::Air);
			Bitmask.AddType(EImmieType::Dragon);
			const TArray<EImmieType> Types = Bitmask.GetTypes();
			TestTrue("Array must contain 3 elements", Types.Num() == 3);
			TestTrue("Array index 0 must be water", Types[0] == EImmieType::Water);
			TestTrue("Array index 1 must be air", Types[1] == EImmieType::Air);
			TestTrue("Array index 2 must be dragon", Types[2] == EImmieType::Dragon);
		});
		It("Set Bitmask From Array Of 3 Types", [this]() {
			FTypeBitmask Bitmask;
			const TArray<EImmieType> TypesToAdd = { EImmieType::Nature, EImmieType::Electric, EImmieType::Dark };
			Bitmask.AddTypes(TypesToAdd);
			TestTrue("Bitmask from array must have nature type", Bitmask.HasType(EImmieType::Nature));
			TestTrue("Bitmask from array must have electric type", Bitmask.HasType(EImmieType::Electric));
			TestTrue("Bitmask from array must have dark type", Bitmask.HasType(EImmieType::Dark));
		});
		It("Combine Types", [this]() {
			FTypeBitmask First;
			FTypeBitmask Second;
			First.AddTypes({ EImmieType::Air, EImmieType::Electric });
			Second.AddType(EImmieType::Dragon);
			FTypeBitmask Combined = FTypeBitmask::Combine(First, Second);
			TestTrue("Expected combined types to have a type count of 3", Combined.TypeCount() == 3);
			const TArray<EImmieType> Types = Combined.GetTypes();
			TestTrue("Expected array to contain 3 elements", Types.Num() == 3);
			TestTrue("Expected array to contain air", Types.Contains(EImmieType::Air));
			TestTrue("Expected array to contain electric ", Types.Contains(EImmieType::Electric));
			TestTrue("Expected array to contain dragon", Types.Contains(EImmieType::Dragon));
		});
	});
}
#endif