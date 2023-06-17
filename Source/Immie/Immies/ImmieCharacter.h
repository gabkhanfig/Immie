// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Immie/ImmieCore.h>
#include <Immie/Battle/Interfaces/BattleActor.h>
#include <Immie/Overworld/Interfaces/Battler.h>
#include <Components/WidgetComponent.h>
#include "SpecieDataTypes.h"
#include "ImmieCharacter.generated.h"

class UImmie;
class UImmieType;
class UAbility;
class ABattleInstance;
class ABattleTeam;
class USpecieDataObject;
class UCameraComponent;
class UImmieMovementComponent;
class UImmieBattleHud;
class UFloatingBattleHealthbar;
class AWildImmieSpawner;
class USphereComponent;

UCLASS()
class IMMIE_API AImmieCharacter : public ACharacter, public IBattleActor, public IBattler
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/**/
		FName SpecieName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadOnly)
		/* The Immie object of this Immie character. Read in values from the object for battle and whatever else. 
		Serialized and send over network to clients for battle. */
		UImmie* ImmieObject;

	UPROPERTY(BlueprintReadOnly)
		/* Whether the Immie is visible, collidable, and tickable. Replicated via RPC. */
		uint8 bEnabled : 1;

	UPROPERTY(BlueprintReadOnly)
		/* The actual type objects of this Immie. Uses the species type bitmask. Replicated via RPC. */
		TArray<UImmieType*> Type;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
		/* The battle ability objects of this Immie. Replicated via RPC. */
		TArray<UAbility*> Abilities;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
		/* The battle team that owns this Immie character. Replicated via RPC. */
		ABattleTeam* Team;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
		/**/
		UDamageComponent* DamageComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
		/* Replicated via RPC. */
		int TeamSlot;

	UPROPERTY(BlueprintReadOnly)
		/* The level of this Immie character (From 1 to the defined max level). Replicated via RPC. */
		uint8 Level;

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
		/* The unmodified stats of this Immie. Health is this Immie's max health. Attack, defense, and speed are all the stat values without any modifiers. Replicated via RPC. */
		FBattleStats InitialStats;

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
		/* The potentially modified stats of this Immie. Health is the current health. Attack, defense, and speed are all the stat values with modifiers. Replicated via RPC. */
		FBattleStats ActiveStats;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle")
		/* Components of this Immie character that can collide with ability actors. */
		TArray<UPrimitiveComponent*> AbilityColliders;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle")
		/* The class used to instantiate the battle hud for this Immie character. */
		TSubclassOf<UImmieBattleHud> BattleHudClass;

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
		/* The battle hud this Immie character is using (must be controlled by local controller). */
		UImmieBattleHud* BattleHud;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/* See FloatingBattleHealthbarClass to set the UClass properly. */
		UWidgetComponent* FloatingBattleHealthbarComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle")
		/* The UClass that the floating battle healthbar will use for this Immie character. */
		TSubclassOf<UFloatingBattleHealthbar> FloatingBattleHealthbarClass;

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
		/**/
		UFloatingBattleHealthbar* FloatingBattleHealthbar;

	UPROPERTY(BlueprintReadWrite)
		/**/
		EImmieCharacterMode ImmieCharacterMode;

	UPROPERTY(BlueprintReadWrite, Category = "Wild")
		/**/
		AWildImmieSpawner* WildSpawner;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		/**/
		USphereComponent* WildBattlerCollider;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Class to be used to create a battle team. */
		TSubclassOf<ABattleTeam> BattleTeamClass;

protected:

	virtual void BeginPlay() override;

	/* Input for an ability at a specified index. */
	void PressAbility(int Index);

	FORCEINLINE void PressAbility0() { PressAbility(0); }
	FORCEINLINE void PressAbility1() { PressAbility(1); }
	FORCEINLINE void PressAbility2() { PressAbility(2); }
	FORCEINLINE void PressAbility3() { PressAbility(3); }

	/* Stop input for an ability at a specified index. */
	void ReleaseAbility(int Index);

	FORCEINLINE void ReleaseAbility0() { ReleaseAbility(0); }
	FORCEINLINE void ReleaseAbility1() { ReleaseAbility(1); }
	FORCEINLINE void ReleaseAbility2() { ReleaseAbility(2); }
	FORCEINLINE void ReleaseAbility3() { ReleaseAbility(3); }

	/* Left and right mouse input. */
	void ImmieYawInput(float AxisValue);

	/* Up and down mouse input. */
	void ImmiePitchInput(float AxisValue);

	/* Forward or backwards input. */
	void AddForwardMovement(float ScaleValue);

	/* Right or left input. */
	void AddRightMovement(float ScaleValue);

	/* Jump input. */
	virtual void Jump() override;

	/* Stop jump input. */
	virtual void StopJumping() override;

	/* Crouch input. */
	void InputCrouch();

	/* Stop crouch input. */
	void InputUnCrouch();

	UFUNCTION(Client, Reliable, Category = "Battle")
		/* Client side execution for when this Immie character is possessed by an Immie player controller. */
		void ClientPossessedByPlayerController(AImmiePlayerController* PlayerController);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Client Possessed By Player Controller", Category = "Battle")
		/* Blueprint and client sided execution for when this Immie character is possessed by an Immie player controller. */
		void BP_ClientPossessedByPlayerController(AImmiePlayerController* PlayerController);

	UFUNCTION(Client, Reliable, Category = "Battle")
		/* Client side execution for when this Immie character is un-possessed by an Immie player controller. */
		void ClientUnPossessedByPlayerController(AImmiePlayerController* PlayerController);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Client Unpossessed By Player Controller", Category = "Battle")
		/* Blueprint and client sided execution for when this Immie character is un-possessed by an Immie player controller. */
		void BP_ClientUnPossessedByPlayerController(AImmiePlayerController* PlayerController);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Initialize For Battle")
		/* Specie Data Object can be cast to the corresponding specie data object class.
		Blueprint sided initialization for battle. See AImmieCharacter::InitializeForBattle() for what is already handled. */
		void BP_InitializeForBattle(USpecieDataObject* SpecieDataObject);

	UFUNCTION(NetMulticast, Reliable)
		/* Synchronizes all of the Immie characters subobjects with the clients. */
		void SetClientSubobjects(const FString& ImmieObjectString, const TArray<UImmieType*>& TypesObjects, const TArray<UAbility*>& AbilityObjects, ABattleTeam* BattleTeamObject, UDamageComponent* DamageComponentObject);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Set Client Subobjects")
		/* Blueprint sided functionality to set any subobject values on connected clients for this Immie character.
		Avoid sending the values held within the objects, only the objects themselves.
		See AImmieCharacter::SyncClientSubobjects() for what is already handled. */
		void BP_SyncClientSubobjects();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "All Client Battle Subobjects Valid")
		/* Returns true by default. Override to check any blueprint specific sub objects that need to be loaded on the client for battle. */
		bool BP_AllClientBattleSubobjectsValid();

	UFUNCTION(NetMulticast, Reliable)
		/* RPC to send battle data to connected clients. */
		void SetClientBattleData(int _TeamSlot, uint8 _Level, FBattleStats _InitialStats, FBattleStats _ActiveStats);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Sync Client Battle Data")
		/* Blueprint sided functionality to replicate battle data on connected clients for this Immie character.
		See AImmieCharacter::SyncClientBattleData() for what is already handled. */
		void BP_SyncClientBattleData();

	UFUNCTION(BlueprintCallable)
		/* Perform authority battle ticking on the components this Immie character owns. */
		void AuthorityBattleTickComponents(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		/* Perform client battle ticking on the components this Immie character owns. */
		void ClientBattleTickComponents(float DeltaTime);

	/**/
	void SetImmieObjectFromJsonString(const FString& JsonString);

	UFUNCTION(BlueprintCallable)
		/* Add a component as a valid ability collider. */
		void AddAbilityCollider(UPrimitiveComponent* AbilityCollider);

	/* Sets BattleHud to a valid instance. */
	void CreateBattleHud();

	/**/
	void CreateFloatingHealthBar();

	UFUNCTION(NetMulticast, Reliable)
		/* Update active stats to all clients. */
		void UpdateActiveStats(FBattleStats NewActiveStats);

public:

	AImmieCharacter(const FObjectInitializer& ObjectInitializer);

	void PostLoad() override;

	/**/
	//virtual void OnActorChannelOpen(class FInBunch& InBunch, class UNetConnection* Connection) override;

	/**/
	//virtual void OnSerializeNewActor(class FOutBunch& OutBunch) override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

	UFUNCTION(BlueprintCallable)
		/* Make a new Immie character. The Immie object must be valid due to it using it's specie id to get the correct specie character class. */
		static AImmieCharacter* NewImmieCharacter(AActor* _Owner, const FTransform& Transform, UImmie* _ImmieObject, bool EnabledOnSpawn = true, ESpawnActorCollisionHandlingMethod SpawnCollisionHandling = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	UFUNCTION(BlueprintCallable)
		/* Spawns a new battle immie character. Immie Object does not change ownership. */
		static AImmieCharacter* SpawnBattleImmieCharacter(ABattleTeam* BattleTeam, const FTransform& Transform, UImmie* _ImmieObject);

	UFUNCTION(BlueprintCallable)
		/* Spawns a new wild immie character. Immie Object changes ownership to the wild immie character. */
		static AImmieCharacter* SpawnWildImmieCharacter(AWildImmieSpawner* Spawner, const FTransform& Transform, UImmie* _ImmieObject);

	UFUNCTION(BlueprintCallable)
		/* Turns the immie character into a battle immie character. Changes ownership to the battle team.
		If currently a wild immie, will remove it from the spawner. */
		void MakeBattle(ABattleTeam* BattleTeam);

	UFUNCTION(BlueprintCallable)
		/* Turns the immie character into a wild immie character. Changes ownership to the spawner. */
		void MakeWild(AWildImmieSpawner* Spawner);


	UFUNCTION(BlueprintCallable, Category = "Battle")
		/* Call to make a controller possess this Immie character for battle. Performs necessary network syncing. */
		void PossessForBattle(AController* NewController);

	UFUNCTION(BlueprintCallable, Category = "Battle")
		/* Call to make the current controller unpossess this Immie character for battle. Performs necessary network syncing. */
		void UnPossessForBattle();

	/* C++ sided initialization for an Immie character for battle. Calls blueprint functionality as well. */
	void InitializeForBattle(ABattleTeam* OwningTeam, int SlotOnTeam);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		/* Sets the Immie to be visible, tick, and have collision, OR not for all of those. Replicates to connected clients. */
		void SetImmieEnabled(bool NewEnabled);

	/* Prepares and subsequently sends the rpc with all of this Immie character subobjects to the clients. 
	Does not send the values held within the objects, only the objects themselves. */
	void SyncClientSubobjects();

	/* Prepares and subsequently sends the rpc with all of this Immie character's battle data to the clients. */
	void SyncClientBattleData();

	/* Checks if this Immie character has all valid subobjects on the clients end (technically can be called on the server or singleplayer). */
	bool AllClientBattleSubobjectsValid();

	UFUNCTION(NetMulticast, Reliable)
		/* Called when this Immie character is set to battle. */
		void OnBeginBattle();

	UFUNCTION(NetMulticast, Reliable)
		/* Called when this Immie character is removed from battle. */
		void OnRemoveFromBattle();

	UFUNCTION(BlueprintNativeEvent, Category = "BattleEvents", DisplayName = "Player Dealt Healing")
		/* Event that executes immediately before an ability this Immie character owns is dealing healing. The amount of healing done can be freely modified. */
		void EventPlayerDealtHealing(const TScriptInterface<IBattleActor>& Target, UPARAM(ref) float& Amount, UPARAM(ref) FBattleDamage& Healing);

	UFUNCTION(BlueprintNativeEvent, Category = "BattleEvents", DisplayName = "Player Dealt Damage")
		/* Event that executes immediately before an ability this Immie character owns is dealing damage. The amount of damage done can be freely modified. */
		void EventPlayerDealtDamage(const TScriptInterface<IBattleActor>& Target, UPARAM(ref) float& Amount, UPARAM(ref) FBattleDamage& Damage);

	/**/
	void WildTick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Wild Tick")
		/**/
		void BP_WildTick(float DeltaTime);

	UFUNCTION(BlueprintPure)
		/* Get the camera component for this Immie character. */
		FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure)
		/**/
		inline UImmie* GetImmieObject() const { return ImmieObject; }

	UFUNCTION(BlueprintPure)
		/**/
		FName GetSpecieName() const { return SpecieName; }

	UFUNCTION(BlueprintPure, Category = "Battle")
		/**/
		ABattleInstance* GetBattleInstance() const;

	UFUNCTION(BlueprintPure, Category = "Battle")
		/**/
		inline TArray<UAbility*> GetAbilities() const { return Abilities; }

	UFUNCTION(BlueprintPure, Category = "Battle")
		/**/
		inline int GetTeamSlot() const { return TeamSlot; }

	UFUNCTION(BlueprintPure)
		/**/
		inline uint8 GetImmieLevel() const { return Level; }

	UFUNCTION(BlueprintPure, Category = "Battle")
		/**/
		inline float GetMaxHealth() const { return InitialStats.Health; }

	UFUNCTION(BlueprintPure, Category = "Battle")
		/**/
		inline float GetCurrentHealth() const { return ActiveStats.Health; }

	UFUNCTION(BlueprintPure, Category = "Battle")
		/**/
		inline bool IsAlive() const { return ActiveStats.Health > 0; }

	UFUNCTION(BlueprintPure)
		/**/
		bool IsControlledByLocalPlayer() const;

	UFUNCTION(BlueprintPure)
		/**/
		EImmieCharacterMode GetImmieCharacterMode() const { return ImmieCharacterMode; }

#pragma region Battle_Actor_Interface

	virtual UDamageComponent* GetDamageComponent_Implementation() const override;
	virtual float TotalHealingFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityHealing) const override;
	virtual float TotalDamageFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityDamage) const override;
	virtual void AuthorityBattleTick_Implementation(float DeltaTime) override;
	virtual void ClientBattleTick_Implementation(float DeltaTime) override;
	virtual bool IsValidAbilityCollider_Implementation(UPrimitiveComponent* Collider) const override;
	virtual bool CanBeHealedByAbilityActor_Implementation(AAbilityActor* AbilityActor) const override;
	virtual bool CanBeDamagedByAbilityActor_Implementation(AAbilityActor* AbilityActor) const override;
	virtual bool IsEnemy_Implementation(const TScriptInterface<IBattleActor>& OtherBattleActor) const override;
	virtual FBattleStats GetInitialStats_Implementation() const override;
	virtual FBattleStats GetActiveStats_Implementation() const override;
	virtual ABattleTeam* GetTeam_Implementation() const override;
	virtual TArray<UImmieType*> GetType_Implementation() const override;
	virtual FString GetDisplayName_Implementation() const override;
	virtual void IncreaseHealth_Implementation(float Amount) override;
	virtual void DecreaseHealth_Implementation(float Amount) override;
	virtual void UpdateVisuals_Implementation() override;

#pragma endregion

#pragma region Battler_Interface

	virtual TArray<UImmie*> GetBattlerTeam_Implementation() const override;
	virtual APawn* GetPawn_Implementation() const override;
	virtual FBattleTeamInit GetBattleTeamInit_Implementation() const override;
	virtual void OnBattleStart_Implementation() override;
	virtual void OnBattleEnd_Implementation(EBattleTeamWinState WinState) override;
	virtual bool CanBeBattled_Implementation() const override;
	virtual TSubclassOf<ABattleTeam> GetBattleTeamClass_Implementation() const override;

#pragma endregion

};
