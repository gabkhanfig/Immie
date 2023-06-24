#include "../ImmieTest.h"
#include "../../Type/ImmieType.h"
#include "../../Type/BattleTypeComponent.h"
#include "../../Game/Global/Managers/TypeDataManager.h"

#if WITH_IMMIE_AUTOMATION_TESTS

// Test coverage in this file does not include networking testing
BEGIN_DEFINE_SPEC(FBattleTypeComponentTest, "Immie.Type.BattleTypeComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
END_DEFINE_SPEC(FBattleTypeComponentTest)

void FBattleTypeComponentTest::Define() 
{
	Describe("Type Damage Multiplier 1 Defend 1 Attack", [this]() {

		It("Weakness", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 25;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric });
			TestTrue("Expected type damage multiplier to be 25. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 25));
		});

		It("Resistance", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 25;
			DamageMultipliers.ResistanceMultiplier = 0.5;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric });
			TestTrue("Expected type damage multiplier to be 0.5. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 0.5));
		});

		It("No type damage multiplier match", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 25;
			DamageMultipliers.ResistanceMultiplier = 0.5;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Air });
			TestTrue("Expected type damage multiplier to be 1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 1));
		});

	}); // Type Damage Multiplier 1 Defend 1 Attack


	Describe("Type Damage Multiplier 2 Defend 1 Attack", [this]() {
		It("Same weakness", [this]() {

			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric);
			
			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Metal);
			Type2.Weaknesses.AddType(EImmieType::Electric);

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric });
			TestTrue("Expected type damage multiplier to be 100. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 100));
		});

		It("Same resistance", [this]() {

			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric);

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Metal);
			Type2.Resistances.AddType(EImmieType::Electric);

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric });
			TestTrue("Expected type damage multiplier to be 0.01. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 0.01f));
		});

		It("Only one weakness", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric);

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Metal);
			Type2.Weaknesses.AddType(EImmieType::Water);

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric });
			TestTrue("Expected type damage multiplier to be 10. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 10));
		});

		It("Only one resistance", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric);

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Metal);
			Type2.Resistances.AddType(EImmieType::Water);

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric });
			TestTrue("Expected type damage multiplier to be 0.1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 0.1));
		});

		It("Cancel out weakness and resistance", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric);

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Metal);
			Type2.Resistances.AddType(EImmieType::Electric);

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric });
			TestTrue("Expected type damage multiplier to be 1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 1));
		});
	}); // Type Damage Multiplier 2 Defend 1 Attack
	
	Describe("Type Damage Multiplier 1 Defend 2 Attack", [this]() {
		It("Single weakness", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Light });
			TestTrue("Expected type damage multiplier to be 10. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 10));
		});

		It("Single resistance", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Light });
			TestTrue("Expected type damage multiplier to be 0.1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 0.1));
			});

		It("Double weakness", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric);
			Type1.Weaknesses.AddType(EImmieType::Light);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Light });
			TestTrue("Expected type damage multiplier to be 100. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 100));
		});

		It("Double resistance", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric);
			Type1.Resistances.AddType(EImmieType::Light);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Light });
			TestTrue("Expected type damage multiplier to be 0.01. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 0.01));
		});

		It("Canceled out", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric);
			Type1.Resistances.AddType(EImmieType::Light);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Light });
			TestTrue("Expected type damage multiplier to be 1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 1));
		});
	}); // Type Damage Multiplier 1 Defend 2 Attack


	Describe("Type Damage Multiplier Many Defend 1 Attack", [this]() {

		It("Overall weak", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric); // 10

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Water);
			Type2.Weaknesses.AddType(EImmieType::Electric); // 100

			FImmieType Type3;
			Type3.Type.AddType(EImmieType::Nature);
			Type3.Resistances.AddType(EImmieType::Electric); // 1000

			FImmieType Type4;
			Type4.Type.AddType(EImmieType::Dragon);
			Type4.Weaknesses.AddType(EImmieType::Electric); // 100

			const TArray<FImmieType> Types = { Type1, Type2, Type3, Type4 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric });
			TestTrue("Expected type damage multiplier to be 100. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 100));
		});

		It("Overall resist", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 2;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Air); // 0.1

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Water);
			Type2.Resistances.AddType(EImmieType::Air); // 0.01

			FImmieType Type3;
			Type3.Type.AddType(EImmieType::Nature);
			Type3.Resistances.AddType(EImmieType::Air); // 0.001

			FImmieType Type4;
			Type4.Type.AddType(EImmieType::Dragon);
			Type4.Weaknesses.AddType(EImmieType::Air); // 0.002

			FImmieType Type5;
			Type5.Type.AddType(EImmieType::Light);
			Type5.Resistances.AddType(EImmieType::Air); // 0.0002

			const TArray<FImmieType> Types = { Type1, Type2, Type3, Type4, Type5 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Air });
			TestTrue("Expected type damage multiplier to be 0.0002. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 0.0002));
		});

		It("Overall neutral", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric); // 10

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Water);
			Type2.Resistances.AddType(EImmieType::Electric); // 1

			FImmieType Type3;
			Type3.Type.AddType(EImmieType::Nature);
			Type3.Resistances.AddType(EImmieType::Electric); // 0.1

			FImmieType Type4;
			Type4.Type.AddType(EImmieType::Dragon);
			Type4.Weaknesses.AddType(EImmieType::Electric); // 1

			const TArray<FImmieType> Types = { Type1, Type2, Type3, Type4 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric });
			TestTrue("Expected type damage multiplier to be 1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 1));
			});

	}); // Type Damage Multiplier Many Defend 1 Attack

	Describe("Type Damage Multiplier 1 Defend Many Attack", [this]() {

		It("Overall weak", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric); // 10
			Type1.Weaknesses.AddType(EImmieType::Air); // 100
			Type1.Weaknesses.AddType(EImmieType::Ground); // 1000
			Type1.Resistances.AddType(EImmieType::Metal); // 100

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Air, EImmieType::Ground, EImmieType::Metal });
			TestTrue("Expected type damage multiplier to be 100. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 100));
		});

		It("Overall resist", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric); // 0.1
			Type1.Weaknesses.AddType(EImmieType::Air); // 1
			Type1.Weaknesses.AddType(EImmieType::Ground); // 10
			Type1.Resistances.AddType(EImmieType::Metal); // 1
			Type1.Resistances.AddType(EImmieType::Dragon); // 0.1

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Air, EImmieType::Ground, EImmieType::Metal, EImmieType::Dragon });
			TestTrue("Expected type damage multiplier to be 0.1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 0.1));
		});

		It("Overall neutral", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric); // 0.1
			Type1.Weaknesses.AddType(EImmieType::Air); // 1
			Type1.Weaknesses.AddType(EImmieType::Ground); // 10
			Type1.Resistances.AddType(EImmieType::Metal); // 1

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Air, EImmieType::Ground, EImmieType::Metal, EImmieType::Dragon });
			TestTrue("Expected type damage multiplier to be 1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 1));
			});

	}); // Type Damage Multiplier 1 Defend Many Attack

	Describe("Type Damage Multiplier Many Defend Many Attack", [this]() {

		It("Overall weak", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric); // 10
			Type1.Weaknesses.AddType(EImmieType::Air); // 100
			Type1.Weaknesses.AddType(EImmieType::Ground); // 1000
			Type1.Resistances.AddType(EImmieType::Metal); // 100

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Ground);
			Type2.Weaknesses.AddType(EImmieType::Nature); // 1000
			Type2.Weaknesses.AddType(EImmieType::Air); // 10000
			Type2.Resistances.AddType(EImmieType::Ground); // 1000

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Air, EImmieType::Ground, EImmieType::Metal, EImmieType::Nature });
			TestTrue("Expected type damage multiplier to be 1000. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 1000));
			});

		It("Overall resist", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric); // 0.1
			Type1.Weaknesses.AddType(EImmieType::Air); // 1
			Type1.Weaknesses.AddType(EImmieType::Ground); // 10
			Type1.Resistances.AddType(EImmieType::Metal); // 1
			Type1.Resistances.AddType(EImmieType::Dragon); // 0.1

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Ground);
			Type2.Weaknesses.AddType(EImmieType::Nature); // 1
			Type2.Resistances.AddType(EImmieType::Air); // 0.1
			Type2.Resistances.AddType(EImmieType::Ground); // 0.01

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Air, EImmieType::Ground, EImmieType::Metal, EImmieType::Dragon, EImmieType::Nature, EImmieType::Water });
			TestTrue("Expected type damage multiplier to be 0.01. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 0.01));
			});

		It("Overall neutral", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric); // 0.1
			Type1.Weaknesses.AddType(EImmieType::Air); // 1
			Type1.Weaknesses.AddType(EImmieType::Ground); // 10
			Type1.Resistances.AddType(EImmieType::Metal); // 1

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Ground);
			Type2.Weaknesses.AddType(EImmieType::Nature); // 10
			Type2.Resistances.AddType(EImmieType::Air); // 1

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);
			const float TypeDamageMultiplier = Comp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Air, EImmieType::Ground, EImmieType::Metal, EImmieType::Dragon, EImmieType::Nature, EImmieType::Water });
			TestTrue("Expected type damage multiplier to be 1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 1));
		});

	}); // Type Damage Multiplier Many Defend Many Attack

	/*
	Describe("Colour", [this]() {

		It("1 Type", [this]() {
			FTypeConstants DamageMultipliers;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Water);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);

			UTypeDataManager* Manager = NewObject<UTypeDataManager>();

			TestTrue("Expected component type color to be the color of water", Comp->GetColor() == Manager->GetTypeColor(EImmieType::Water));
		});

		It("2 Types Order First", [this]() {
			FTypeConstants DamageMultipliers;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Water);

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Dragon);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);

			UTypeDataManager* Manager = NewObject<UTypeDataManager>();

			TestTrue("Expected component type color with two types to be the color of water", Comp->GetColor() == Manager->GetTypeColor(EImmieType::Water));
		});

		It("2 Types Order Second", [this]() {
			FTypeConstants DamageMultipliers;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Dragon);

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Water);

			const TArray<FImmieType> Types = { Type1 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);

			UTypeDataManager* Manager = NewObject<UTypeDataManager>();

			TestTrue("Expected component type color with two types to be the color of dragon", Comp->GetColor() == Manager->GetTypeColor(EImmieType::Dragon));
		});

	}); // Colour*/


	Describe("Copy", [this]() {

		It("Copy From Other Component Weakness", [this]() {

			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Weaknesses.AddType(EImmieType::Electric); // 10
			Type1.Weaknesses.AddType(EImmieType::Air); // 100
			Type1.Weaknesses.AddType(EImmieType::Ground); // 1000
			Type1.Resistances.AddType(EImmieType::Metal); // 100

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Ground);
			Type2.Weaknesses.AddType(EImmieType::Nature); // 1000
			Type2.Weaknesses.AddType(EImmieType::Air); // 10000
			Type2.Resistances.AddType(EImmieType::Ground); // 1000

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);

			UBattleTypeComponent* CopyComp = NewObject<UBattleTypeComponent>();
			CopyComp->CopyFrom(Comp);
			const float TypeDamageMultiplier = CopyComp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Air, EImmieType::Ground, EImmieType::Metal, EImmieType::Nature });
			TestTrue("Expected type damage multiplier to be 1000. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 1000));
			
		});

		It("Copy From Other Component Resistance", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric); // 0.1
			Type1.Weaknesses.AddType(EImmieType::Air); // 1
			Type1.Weaknesses.AddType(EImmieType::Ground); // 10
			Type1.Resistances.AddType(EImmieType::Metal); // 1
			Type1.Resistances.AddType(EImmieType::Dragon); // 0.1

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Ground);
			Type2.Weaknesses.AddType(EImmieType::Nature); // 1
			Type2.Resistances.AddType(EImmieType::Air); // 0.1
			Type2.Resistances.AddType(EImmieType::Ground); // 0.01

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);

			UBattleTypeComponent* CopyComp = NewObject<UBattleTypeComponent>();
			CopyComp->CopyFrom(Comp);
			const float TypeDamageMultiplier = CopyComp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Air, EImmieType::Ground, EImmieType::Metal, EImmieType::Dragon, EImmieType::Nature, EImmieType::Water });
			TestTrue("Expected type damage multiplier to be 0.01. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 0.01));
		});

		It("Copy From Other Component Neutral", [this]() {
			FTypeConstants DamageMultipliers;
			DamageMultipliers.WeaknessMultiplier = 10;
			DamageMultipliers.ResistanceMultiplier = 0.1;

			FImmieType Type1;
			Type1.Type.AddType(EImmieType::Fire);
			Type1.Resistances.AddType(EImmieType::Electric); // 0.1
			Type1.Weaknesses.AddType(EImmieType::Air); // 1
			Type1.Weaknesses.AddType(EImmieType::Ground); // 10
			Type1.Resistances.AddType(EImmieType::Metal); // 1

			FImmieType Type2;
			Type2.Type.AddType(EImmieType::Ground);
			Type2.Weaknesses.AddType(EImmieType::Nature); // 10
			Type2.Resistances.AddType(EImmieType::Air); // 1

			const TArray<FImmieType> Types = { Type1, Type2 };

			UBattleTypeComponent* Comp = NewObject<UBattleTypeComponent>();
			Comp->Initialize(DamageMultipliers, Types);

			UBattleTypeComponent* CopyComp = NewObject<UBattleTypeComponent>();
			CopyComp->CopyFrom(Comp);
			const float TypeDamageMultiplier = CopyComp->TotalTypeDamageMultiplier({ EImmieType::Electric, EImmieType::Air, EImmieType::Ground, EImmieType::Metal, EImmieType::Dragon, EImmieType::Nature, EImmieType::Water });
			TestTrue("Expected type damage multiplier to be 1. Was: " + FString::SanitizeFloat(TypeDamageMultiplier), FMath::IsNearlyEqual(TypeDamageMultiplier, 1));
		});

	}); // Copy
}

#endif