// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Immie/Game/Global/Managers/TypeDataManager.h>
#include <Immie/Game/Global/Managers/AbilityDataManager.h>
#include <Immie/Game/Global/Managers/SpecieDataManager.h>
#include "ImmieObject.h"
#include <Kismet/GameplayStatics.h>
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Battle/Managers/BattleTypeManager.h>
#include <Immie/Battle/Managers/BattleAbilityManager.h>
#include <Immie/Battle/Managers/BattleSpecieManager.h>
#include <Immie/Battle/Team/BattleTeam.h>
#include <Immie/Ability/Ability.h>
#include <Immie/Battle/Components/DamageComponent.h>
#include <Immie/Immies/SpecieDataObject.h>
#include <Kismet/KismetMathLibrary.h>
#include <Immie/Movement/ImmieMovementComponent.h>
#include <Immie/Controller/Player/ImmiePlayerController.h>
#include <Immie/Type/ImmieType.h>
#include <Immie/Battle/UI/ImmieBattleHud.h>
#include <Immie/Battle/UI/FloatingBattleHealthbar.h>
#include <Immie/Overworld/WildImmies/WildImmieSpawner.h>
#include "Components/SphereComponent.h"

AImmieCharacter::AImmieCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UImmieMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = true;

	FloatingBattleHealthbarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingBattleHealthbarComponent"));
	FloatingBattleHealthbarComponent->SetupAttachment(RootComponent);
	FloatingBattleHealthbarComponent->SetIsReplicated(false);
	FloatingBattleHealthbarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingBattleHealthbarComponent->SetVisibility(false, true);
	FloatingBattleHealthbarComponent->SetWidget(nullptr);

	WildBattlerCollider = CreateDefaultSubobject<USphereComponent>(TEXT("WildBattlerCollider"));
	WildBattlerCollider->SetupAttachment(RootComponent);
	WildBattlerCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Ignore);
	WildBattlerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ImmieObject = nullptr;
	bEnabled = true;
	Abilities.Reserve(MAX_ABILITY_COUNT);
	Team = nullptr;
	BattleHud = nullptr;
	ImmieCharacterMode = EImmieCharacterMode::None;

	WildSpawner = nullptr;

	// TODO remove this later
	AbilityColliders.Add(GetCapsuleComponent());

	static ConstructorHelpers::FClassFinder<ABattleTeam> BattleTeamFoundClass(TEXT("/Game/Battle/Team/BP_WildBattleTeam"));
	BattleTeamClass = BattleTeamFoundClass.Class;

}

void AImmieCharacter::PostLoad()
{
	const FName ClassName = GetClass()->GetFName();
	if (ClassName == "ImmieCharacter" || ClassName == "ImmieCharacter_BP_C") {
		Super::PostLoad();
		return;
	}

	SpecieName = USpecieDataManager::SpecieNameFromBlueprintClassName(ClassName.ToString(), "Character_C");
	
	Super::PostLoad();
}

void AImmieCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AImmieCharacter::PressAbility(int Index)
{
	UAbility* Ability = Abilities[Index];
	Ability->InputPress();
}

void AImmieCharacter::ReleaseAbility(int Index)
{
	UAbility* Ability = Abilities[Index];
	Ability->InputRelease();
}

void AImmieCharacter::ImmieYawInput(float AxisValue)
{
	AddControllerYawInput(AxisValue * 0.5);
}

void AImmieCharacter::ImmiePitchInput(float AxisValue)
{
	AddControllerPitchInput(AxisValue * 0.5);
}

void AImmieCharacter::AddForwardMovement(float ScaleValue)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), ScaleValue, false);
}

void AImmieCharacter::AddRightMovement(float ScaleValue)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), ScaleValue, false);
}

void AImmieCharacter::Jump()
{
	Super::Jump();
}

void AImmieCharacter::StopJumping()
{
	Super::StopJumping();
}

void AImmieCharacter::Crouch()
{
}

void AImmieCharacter::StopCrouching()
{
}

void AImmieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AImmieCharacter::AuthorityBattleTick_Implementation(float DeltaTime)
{
	FBattleStats PreviousActiveStats = ActiveStats;

	// Is already checked for if is alive by battle team. 

	AuthorityBattleTickComponents(DeltaTime);
	if (!IsRunningDedicatedServer()) {
		IBattleActor::Execute_UpdateVisuals(this);
		//UpdateVisuals();
	}

	if (ActiveStats != PreviousActiveStats) {
		UpdateActiveStats(ActiveStats);
	}
}

void AImmieCharacter::ClientBattleTick_Implementation(float DeltaTime)
{
	ClientBattleTickComponents(DeltaTime);
	IBattleActor::Execute_UpdateVisuals(this);
	//UpdateVisuals();
}

bool AImmieCharacter::IsValidAbilityCollider_Implementation(UPrimitiveComponent* Collider) const
{
	return AbilityColliders.Contains(Collider);
}

bool AImmieCharacter::CanBeHealedByAbilityActor_Implementation(AAbilityActor* AbilityActor) const
{
	return true;
}

bool AImmieCharacter::CanBeDamagedByAbilityActor_Implementation(AAbilityActor* AbilityActor) const
{
	return true;
}

bool AImmieCharacter::IsEnemy_Implementation(const TScriptInterface<IBattleActor>& OtherBattleActor) const
{
	if (!IsValid(Team)) return true;
	return Team != IBattleActor::Execute_GetTeam(OtherBattleActor.GetObject());
}

FBattleStats AImmieCharacter::GetInitialStats_Implementation() const
{
	return InitialStats;
}

ABattleTeam* AImmieCharacter::GetTeam_Implementation() const
{
	return Team;
}

TArray<UImmieType*> AImmieCharacter::GetType_Implementation() const
{
	return Type;
}

void AImmieCharacter::IncreaseHealth_Implementation(float Amount)
{
	checkf(Amount >= 0, TEXT("Increasing battle actor health by negative value is not allowed"));
	ActiveStats.Health += Amount;
	if (ActiveStats.Health > InitialStats.Health) {
		ActiveStats.Health = InitialStats.Health;
	}
}

void AImmieCharacter::DecreaseHealth_Implementation(float Amount)
{
	checkf(Amount >= 0, TEXT("Decreasing battle actor health by negative value is not allowed"));
	ActiveStats.Health -= Amount;
	if (ActiveStats.Health < 0) {
		ActiveStats.Health = 0;
	}
}

void AImmieCharacter::UpdateVisuals_Implementation()
{
}

void AImmieCharacter::AuthorityBattleTickComponents(float DeltaTime)
{
	for (int i = 0; i < Abilities.Num(); i++) {
		Abilities[i]->AuthorityBattleTick(DeltaTime);
	}
	DamageComponent->AuthorityBattleTick(DeltaTime);
}

void AImmieCharacter::ClientBattleTickComponents(float DeltaTime)
{
	for (int i = 0; i < Abilities.Num(); i++) {
		Abilities[i]->ClientBattleTick(DeltaTime);
	}
	DamageComponent->ClientBattleTick(DeltaTime);
}

void AImmieCharacter::SetImmieObjectFromJsonString(const FString& JsonString)
{
	FJsonObjectBP ImmieJson;
	if (FJsonObjectBP::LoadJsonString(JsonString, ImmieJson)) {
		FName ParsedSpecieName = FName(ImmieJson.GetStringField("Specie"));
		ImmieObject = UImmie::NewImmieObject(this, ParsedSpecieName);
		ImmieObject->LoadJsonData(ImmieJson);
	}
}

void AImmieCharacter::AddAbilityCollider(UPrimitiveComponent* AbilityCollider)
{
	AbilityColliders.Add(AbilityCollider);
}

void AImmieCharacter::CreateBattleHud()
{
	if (!IsValid(BattleHudClass)) {
		iLog("Battle hud class for Immie must be valid! Specie: " + GetSpecieName().ToString(), LogVerbosity_Error);
		return;
	}

	BattleHud = CreateWidget<UImmieBattleHud>(Cast<AImmiePlayerController>(GetController()), BattleHudClass);
	BattleHud->SetImmieCharacter(this);
	BattleHud->BP_Initialize();
	BattleHud->AddToViewport(100);
}

void AImmieCharacter::CreateFloatingHealthBar()
{
	if (!IsValid(FloatingBattleHealthbarClass)) {
		iLog("Floating battle healthbar class for Immie must be valid! Specie: " + GetSpecieName().ToString(), LogVerbosity_Error);
		return;
	}

	FloatingBattleHealthbar = CreateWidget<UFloatingBattleHealthbar>(Cast<APlayerController>(GetWorld()->GetFirstPlayerController()), FloatingBattleHealthbarClass);
	FloatingBattleHealthbar->SetBattleActor(this);
	FloatingBattleHealthbar->BP_Initialize();

	FloatingBattleHealthbarComponent->SetWidget(FloatingBattleHealthbar);
	FloatingBattleHealthbarComponent->SetVisibility(true, true);
}

void AImmieCharacter::UpdateActiveStats_Implementation(FBattleStats NewActiveStats)
{
	ActiveStats = NewActiveStats;
}

void AImmieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	/* Rotation. */
	PlayerInputComponent->BindAxis("Turn", this, &AImmieCharacter::ImmieYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AImmieCharacter::ImmiePitchInput);

	/* WASD. */
	PlayerInputComponent->BindAxis("MoveForward", this, &AImmieCharacter::AddForwardMovement);
	PlayerInputComponent->BindAxis("MoveRight", this, &AImmieCharacter::AddRightMovement);

	/* Jump. */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AImmieCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AImmieCharacter::StopJumping);

	/* Crouch. */
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AImmieCharacter::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AImmieCharacter::StopCrouching);

	/* Immie ability inputs. */
	PlayerInputComponent->BindAction("InputAbility0", IE_Pressed, this, &AImmieCharacter::PressAbility0);
	PlayerInputComponent->BindAction("InputAbility1", IE_Pressed, this, &AImmieCharacter::PressAbility1);
	PlayerInputComponent->BindAction("InputAbility2", IE_Pressed, this, &AImmieCharacter::PressAbility2);
	PlayerInputComponent->BindAction("InputAbility3", IE_Pressed, this, &AImmieCharacter::PressAbility3);

	PlayerInputComponent->BindAction("InputAbility0", IE_Released, this, &AImmieCharacter::ReleaseAbility0);
	PlayerInputComponent->BindAction("InputAbility1", IE_Released, this, &AImmieCharacter::ReleaseAbility1);
	PlayerInputComponent->BindAction("InputAbility2", IE_Released, this, &AImmieCharacter::ReleaseAbility2);
	PlayerInputComponent->BindAction("InputAbility3", IE_Released, this, &AImmieCharacter::ReleaseAbility3);
}

void AImmieCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AImmieCharacter::UnPossessed()
{
	Super::UnPossessed();
}

AImmieCharacter* AImmieCharacter::NewImmieCharacter(AActor* _Owner, const FTransform& Transform, UImmie* _ImmieObject, bool EnabledOnSpawn, ESpawnActorCollisionHandlingMethod SpawnCollisionHandling)
{
	UClass* ImmieCharacterClass = GetSpecieDataManager()->GetImmieCharacterClass(_ImmieObject->GetSpecieName());
	checkf(IsValid(ImmieCharacterClass), TEXT("UClass for Immie character must be valid for spawning"));
	AImmieCharacter* SpawnedImmie = 
		_Owner->GetWorld()->SpawnActorDeferred<AImmieCharacter>(ImmieCharacterClass, Transform, _Owner, nullptr, SpawnCollisionHandling);

	checkf(IsValid(SpawnedImmie), TEXT("Spawned Immie character must be valid"));
	SpawnedImmie->ImmieObject = _ImmieObject;

	if (!EnabledOnSpawn) {
		SpawnedImmie->SetImmieEnabled(false);
	}

	UGameplayStatics::FinishSpawningActor(SpawnedImmie, Transform);
	return SpawnedImmie;
}

AImmieCharacter* AImmieCharacter::SpawnBattleImmieCharacter(ABattleTeam* BattleTeam, const FTransform& Transform, UImmie* _ImmieObject)
{
	AImmieCharacter* SpawnedImmie = NewImmieCharacter(BattleTeam, Transform, _ImmieObject, false, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	SpawnedImmie->ImmieCharacterMode = EImmieCharacterMode::Battle;
	SpawnedImmie->WildBattlerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	return SpawnedImmie;
}

AImmieCharacter* AImmieCharacter::SpawnWildImmieCharacter(AWildImmieSpawner* Spawner, const FTransform& Transform, UImmie* _ImmieObject)
{
	AImmieCharacter* SpawnedImmie = NewImmieCharacter(Spawner, Transform, _ImmieObject, true, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	_ImmieObject->ChangeOuter(SpawnedImmie);
	SpawnedImmie->ImmieCharacterMode = EImmieCharacterMode::Wild;
	SpawnedImmie->WildBattlerCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SpawnedImmie->WildSpawner = Spawner;
	return SpawnedImmie;
}

void AImmieCharacter::MakeBattle(ABattleTeam* BattleTeam)
{
	if (ImmieCharacterMode == EImmieCharacterMode::Wild && IsValid(WildSpawner)) {
		WildSpawner->StopTrackingWildImmie(this);
	}
	ImmieCharacterMode = EImmieCharacterMode::Battle;
	WildBattlerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetImmieEnabled(false);
	SetOwner(BattleTeam);
}

void AImmieCharacter::MakeWild(AWildImmieSpawner* Spawner)
{
	ImmieCharacterMode = EImmieCharacterMode::Wild;
	WildSpawner = Spawner;
	WildSpawner->StartTrackingWildImmie(this);
	WildBattlerCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetImmieEnabled(true);
	SetOwner(WildSpawner);
}

void AImmieCharacter::PossessForBattle(AController* NewController)
{
	NewController->Possess(this);
	
	const bool IsPlayerController = NewController->IsPlayerController();
	if (IsPlayerController) {
		AImmiePlayerController* ImmiePlayerController = Cast<AImmiePlayerController>(NewController);
		ClientPossessedByPlayerController(ImmiePlayerController);
	}
}

void AImmieCharacter::ClientPossessedByPlayerController_Implementation(AImmiePlayerController* PlayerController)
{
	BP_ClientPossessedByPlayerController(PlayerController);
}

void AImmieCharacter::UnPossessForBattle()
{
	if (GetController() == nullptr) {
		return;
	}

	AImmiePlayerController* ImmiePlayerController = Cast<AImmiePlayerController>(GetController());
	if (IsValid(ImmiePlayerController)) {
	}
	GetController()->UnPossess();
}

void AImmieCharacter::ClientUnPossessedByPlayerController_Implementation(AImmiePlayerController* PlayerController)
{
	BP_ClientUnPossessedByPlayerController(PlayerController);
}

void AImmieCharacter::InitializeForBattle(ABattleTeam* OwningTeam, int SlotOnTeam)
{
	Team = OwningTeam;
	TeamSlot = SlotOnTeam;

	USpecieDataObject* SpecieDataObject = GetBattleInstance()->GetBattleSpecieManager()->GetSpecieDataObject(ImmieObject->GetSpecieName());

	const int SpecieTypeBitmask = SpecieDataObject->GetTypeBitmask();
	Type = GetBattleInstance()->GetBattleTypeManager()->GetTypes(SpecieTypeBitmask);

	Level = UFormula::LevelFromXp(ImmieObject->GetXp());

	const FBaseStats BaseStats = SpecieDataObject->GetBaseStats();
	InitialStats = ImmieObject->GetStats(true, BaseStats.Health, BaseStats.Attack, BaseStats.Defense, BaseStats.Speed);

	ActiveStats.Health = ImmieObject->GetHealth();
	ActiveStats.Attack = InitialStats.Attack;
	ActiveStats.Defense = InitialStats.Defense;
	ActiveStats.Speed = InitialStats.Speed;

	DamageComponent = UDamageComponent::NewDamageComponent(this);
	DamageComponent->RegisterComponent();

	const TArray<FName> AbilityIds = ImmieObject->GetAbilityNames();
	const int AbilityCount = AbilityIds.Num();
	for (int i = 0; i < AbilityCount; i++) {
		UAbility* BattleAbility = UAbility::NewAbility(this, AbilityIds[i]);
		BattleAbility->InitializeForBattle();
		BattleAbility->RegisterComponent();
		Abilities.Add(BattleAbility);
	}

	BP_InitializeForBattle(SpecieDataObject);
}

void AImmieCharacter::SyncClientSubobjects()
{
	FString ImmieObjectString = ImmieObject->SaveJsonData().ToString();
	SetClientSubobjects(ImmieObjectString, Type, Abilities, Team, DamageComponent);
	BP_SyncClientSubobjects();
}

void AImmieCharacter::SyncClientBattleData()
{
	SetClientBattleData(
		TeamSlot,
		Level,
		InitialStats,
		ActiveStats
	);

	for (int i = 0; i < Abilities.Num(); i++) {
		Abilities[i]->SyncToClients();
	}

	BP_SyncClientBattleData();
}

void AImmieCharacter::SetClientBattleData_Implementation(int _TeamSlot, uint8 _Level, FBattleStats _InitialStats, FBattleStats _ActiveStats)
{
	if (Team->HasAuthority()) {
		return;
	}

	TeamSlot = _TeamSlot;
	Level = _Level;
	InitialStats = _InitialStats;
	ActiveStats = _ActiveStats;
}

void AImmieCharacter::SetClientSubobjects_Implementation(const FString& ImmieObjectString, const TArray<UImmieType*>& TypesObjects, const TArray<UAbility*>& AbilityObjects, ABattleTeam* BattleTeamObject, UDamageComponent* DamageComponentObject)
{
	if (BattleTeamObject->HasAuthority()) {
		return;
	}

	Type = TypesObjects;
	Abilities = AbilityObjects;
	Team = BattleTeamObject;
	DamageComponent = DamageComponentObject;

	SetImmieObjectFromJsonString(ImmieObjectString);
}

bool AImmieCharacter::AllClientBattleSubobjectsValid()
{
	const int TypeCount = Type.Num();
	const int AbilityCount = Abilities.Num();

	if (!IsValid(ImmieObject))
		return false;

	if (TypeCount == 0)
		return false;

	if (AbilityCount == 0)
		return false;

	if (!IsValid(Team))
		return false;

	if (!IsValid(DamageComponent))
		return false;

	for (int i = 0; i < TypeCount; i++) {
		if (!IsValid(Type[i])) {
			return false;
		}
	}

	for (int i = 0; i < AbilityCount; i++) {
		if (!IsValid(Abilities[i])) {
			return false;
		}
	}
	
	const bool IsValidBlueprintSubobjects = BP_AllClientBattleSubobjectsValid();
	return IsValidBlueprintSubobjects;
}

void AImmieCharacter::OnBeginBattle_Implementation()
{
	if (IsRunningDedicatedServer()) return;

	if (IsControlledByLocalPlayer()) {
		CreateBattleHud();
	}
	else {
		CreateFloatingHealthBar();
	}
}

void AImmieCharacter::OnRemoveFromBattle_Implementation()
{
	if (IsValid(BattleHud)) {
		BattleHud->RemoveFromParent();
	}
	if (IsValid(FloatingBattleHealthbar)) {
		FloatingBattleHealthbar->RemoveFromParent();
		FloatingBattleHealthbarComponent->SetVisibility(false, true);
		FloatingBattleHealthbarComponent->SetWidget(nullptr);
	}

	BattleHud = nullptr;
	FloatingBattleHealthbar = nullptr;
}

void AImmieCharacter::EventPlayerDealtHealing_Implementation(const TScriptInterface<IBattleActor>& Target, float& Amount, FBattleDamage& Healing)
{
	Team->EventPlayerDealtHealing(Target, Amount, Healing, this);
	for (int i = 0; i < Abilities.Num(); i++) {
		UAbility* Ability = Abilities[i];
		const bool IsHealingAbility = Healing.Ability == Ability;
		Ability->EventPlayerDealtDamage(Target, Amount, Healing, IsHealingAbility);
	}
}

void AImmieCharacter::EventPlayerDealtDamage_Implementation(const TScriptInterface<IBattleActor>& Target, float& Amount, FBattleDamage& Damage)
{
	Team->EventPlayerDealtDamage(Target, Amount, Damage, this);
	for (int i = 0; i < Abilities.Num(); i++) {
		UAbility* Ability = Abilities[i];
		const bool IsDamagingAbility = Damage.Ability == Ability;
		Ability->EventPlayerDealtDamage(Target, Amount, Damage, IsDamagingAbility);
	}
}

void AImmieCharacter::WildTick(float DeltaTime)
{
	iLog("Wild tick immie character", LogVerbosity_Display, 0);

	BP_WildTick(DeltaTime);
}

bool AImmieCharacter::BP_AllClientBattleSubobjectsValid_Implementation()
{
	return true;
}

ABattleInstance* AImmieCharacter::GetBattleInstance() const
{
	return Team->GetBattleInstance();
}

bool AImmieCharacter::IsControlledByLocalPlayer() const
{
	AImmiePlayerController* ImmieController = Cast<AImmiePlayerController>(GetController());
	return ImmieController != nullptr && ImmieController == GetBattleInstance()->GetLocalPlayerController();
}

UDamageComponent* AImmieCharacter::GetDamageComponent_Implementation() const
{
	return DamageComponent;
}

float AImmieCharacter::TotalHealingFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityHealing) const
{
	// TODO properly implement healing thats not just the opposite of damage.
	const float UnmodifiedDamageMultiplier = UFormula::Damage(AbilityHealing.Power, AbilityHealing.AttackerStat, AbilityHealing.DefenderStat, AbilityHealing.InstigatorLevel);
	const float TypeDamageMultiplier = UImmieType::TotalTypeDamageMultiplier(AbilityHealing.InstigatorType, AbilityHealing.DefenderType);
	return UnmodifiedDamageMultiplier * TypeDamageMultiplier;
}

float AImmieCharacter::TotalDamageFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityDamage) const
{
	const float UnmodifiedDamageMultiplier = UFormula::Damage(AbilityDamage.Power, AbilityDamage.AttackerStat, AbilityDamage.DefenderStat, AbilityDamage.InstigatorLevel);
	const float TypeDamageMultiplier = UImmieType::TotalTypeDamageMultiplier(AbilityDamage.InstigatorType, AbilityDamage.DefenderType);
	return UnmodifiedDamageMultiplier * TypeDamageMultiplier;
}

void AImmieCharacter::SetImmieEnabled_Implementation(bool NewEnabled)
{
	SetActorTickEnabled(NewEnabled);
	SetActorHiddenInGame(!NewEnabled);
	SetActorEnableCollision(NewEnabled);
	bEnabled = NewEnabled;
}

FBattleStats AImmieCharacter::GetActiveStats_Implementation() const
{
	return ActiveStats;
}

FString AImmieCharacter::GetDisplayName_Implementation() const
{
	return ImmieObject->GetDisplayName();
}

#pragma region Battler_Interface

TArray<UImmie*> AImmieCharacter::GetBattlerTeam_Implementation() const
{
	const TArray<UImmie*> Immies = { ImmieObject };
	return Immies;
}

APawn* AImmieCharacter::GetPawn_Implementation() const
{
	return (APawn*)this;
}

FBattleTeamInit AImmieCharacter::GetBattleTeamInit_Implementation() const
{
	FBattleTeamInit TeamInit = DefaultBattleTeamInit();
	// Wild spawner is the owner of the team, so that it can potentially regain control of the immie afterwards.
	TeamInit.TeamOwner = WildSpawner;
	return TeamInit;
}

void AImmieCharacter::OnBattleStart_Implementation()
{
}

void AImmieCharacter::OnBattleEnd_Implementation(EBattleTeamWinState WinState)
{
}

bool AImmieCharacter::CanBeBattled_Implementation() const
{
	return true;
}

TSubclassOf<ABattleTeam> AImmieCharacter::GetBattleTeamClass_Implementation() const
{
	return BattleTeamClass;
}

#pragma endregion