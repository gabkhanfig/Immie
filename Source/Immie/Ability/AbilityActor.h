// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Immie/Core/Structures.h>
#include <Immie/Battle/Interfaces/BattleActor.h>
#include "AbilityDataTypes.h"
#include "AbilityActor.generated.h"

class UAbility;
class AImmieCharacter;
class UDamageComponent;
class ADummyAbilityActor;
class UImmieType; 
class UProjectileMovementComponent;

UCLASS()
/* Actor object representing in world abilities. Ensure calling AAbilityActor::DestroyAbilityActor() rather than AAbilityActor::Destroy(). */
class IMMIE_API AAbilityActor : public AActor, public IBattleActor
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/* The ability that "owns" this ability actor. Replicated via serialization on spawn. DO NOT CHANGE ON CLIENT. */
		UAbility* Ability;

	UPROPERTY()
		/* The damage component of this ability actor. Is constructed with this ability actor. Replicated via serialization on spawn. DO NOT CHANGE ON CLIENT. */
		UDamageComponent* DamageComponent;

	UPROPERTY()
		/* The active stats of the owning ability component when this ability actor was spawned. Replicated via serialization on spawn. DO NOT CHANGE ON CLIENT. */
		FBattleStats SpawnedActiveStats;

	UPROPERTY()
		/**/
		TMap<UObject*, FBattleActorColliderHitArray> BattleActorHitColliders;

protected:

	UPROPERTY(BlueprintReadWrite, replicated)
		/* The active stats of this ability actor. Health is the current health. Uses stat modifiers. Replicated via lifetime props. */
		FBattleStats ActiveStats;

	UPROPERTY(BlueprintReadWrite)
		/* Only exists on client. */
		ADummyAbilityActor* Dummy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/* Components of this Immie character that can collide with ability actors. */
		TArray<UPrimitiveComponent*> AbilityColliders;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/* Flag for if this ability actor should spawn a visual dummy on standalone or clients.
		Note that this being false does not prevent an ability actor from spawning a dummy. 
		Simply call SpawnVisualDummy(). */
		bool bShouldSpawnVisualDummy;

protected:

	/* Blueprint Note: Any network syncronization a blueprint ability actor should do should take place within the blueprint's BeginPlay() event.
	This way, when the RPC to inform clients to create their dummy actors gets executed, syncing should have already executed. */
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "InitializeForBattle")
		/* Called by authority side before  */
		void BP_InitializeForBattle();

	UFUNCTION(BlueprintCallable)
		/* Spawns a dummy ability actor used as a representation of this one. */
		void SpawnVisualDummy();

	UFUNCTION()
		/* Create a dummy actor using the corresponding UClass for this ability. */
		ADummyAbilityActor* CreateDummyActor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/* Get the transform the dummy ability actor should spawn at. By default, spawns at the same transform as this ability actor. Can be overridden in blueprints. */
		FTransform GetDummySpawnTransform();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Ability Actor Destroy")
		/* Called when this ability actor is about to be destroyed. The dummy is already informed prior to this event. */
		void BP_OnAbilityActorDestroy();

	UFUNCTION(BlueprintCallable)
		/* Add a component as a valid ability collider. */
		void AddAbilityCollider(UPrimitiveComponent* AbilityCollider);

	UFUNCTION(BlueprintCallable)
		/* Called when any of this ability actor's colliders have begun collision with another actor. */
		void OnCollision(UPrimitiveComponent* ThisOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent);

	UFUNCTION(BlueprintNativeEvent)
		/* Executes after AAbilityActor::OnCollision(). Can have base functionality overridden in blueprints. 
		See "call to parent" functionality in blueprints. For collision tracking, prefer call to parent AFTER blueprint logic. */
		void OnBattleActorCollision(const TScriptInterface<IBattleActor>& BattleActor, UPrimitiveComponent* ThisComponent, UPrimitiveComponent* OtherComponent, bool IsEnemy);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Authority Battle Tick")
		/**/
		void BP_AuthorityBattleTick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Client Battle Tick")
		/**/
		void BP_ClientBattleTick(float DeltaTime);

	UFUNCTION(BlueprintPure)
		/* Check if this battle actor has collided with this ability actor previously. See AAbilityActor::OnBattleActorCollision and AAbilityActor::AddCollidedBattleActor(). */
		bool HasBattleActorCollidedAlready(const TScriptInterface<IBattleActor>& BattleActor) const;

	UFUNCTION(BlueprintPure)
		/* Check if this battle actor has collided with a specific ability actor component previously. See AAbilityActor::OnBattleActorCollision and AAbilityActor::AddCollidedBattleActor(). */
		bool HasBattleActorCollidedWithCollider(const TScriptInterface<IBattleActor>& BattleActor, UPrimitiveComponent* CheckColliderHit) const;

	UFUNCTION(BlueprintCallable)
		/* Track that a battle actor has collided with a specific component. See AAbilityActor::HasBattleActorCollidedAlready() and AAbilityActor::HasBattleActorCollidedWithCollider(). */
		void AddCollidedBattleActor(const TScriptInterface<IBattleActor>& BattleActor, UPrimitiveComponent* HitCollider);

public:	

	UFUNCTION(BlueprintCallable)
		/* Works for dummy abilities. */
		static void EnableAbilityProjectileComponent(AActor* AbilityActor, UAbilityDataObject* AbilityDataObject, UProjectileMovementComponent* ProjMovement, AImmieCharacter* ImmieCharacter);

	/**/
	AAbilityActor();

	/**/
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		/* Prefer this to AAbilityActor::Destroy() */
		void DestroyAbilityActor();

	UFUNCTION(NetMulticast, Reliable)
		/* Should be called right before this ability actor is fully destroyed. */
		void OnAbilityActorDestroy();
 
	/* Set the ability that "owns" this ability actor. Not the same as the actors actual owner. */
	void SetAbility(UAbility* _Ability);

	/* Set the active stats from the corresponding ability component at the moment this ability actor was spawned. */
	void SetSpawnedActiveStats(FBattleStats _SpawnedActiveStats);

	void SetDamageComponent(UDamageComponent* _DamageComponent);

	/* Initialize this ability actor for battle. */
	void InitializeForBattle();

	/* Called when the client's actor channel opens for this ability actor. */
	virtual void OnActorChannelOpen(class FInBunch& InBunch, class UNetConnection* Connection) override;

	/* Called when the server will serialize this ability actor to clients. */
	virtual void OnSerializeNewActor(class FOutBunch& OutBunch) override;

	UFUNCTION(NetMulticast, Reliable)
		/**/
		void InformClientsSpawnDummy();

	UFUNCTION(BlueprintCallable)
		/**/
		FBattleActorHitscanResult HitscanShotFromImmie(
			AImmieCharacter* Start,
			float MaxDistance,
			UPARAM(meta = (Bitmask, BitmaskEnum = EAbilityHitscanShotTarget)) int TargetBitmask
		);

	UFUNCTION(BlueprintPure)
		/* Get the ability that "owns" this ability actor. Not the same as the actors actual owner. */
		FORCEINLINE UAbility* GetAbility() const { return Ability; }

	UFUNCTION(BlueprintPure)
		/* Get the id number of this ability. */
		int GetAbilityId() const;

	UFUNCTION(BlueprintPure)
		/* Get the Immie character that owns this ability. */
		AImmieCharacter* GetImmieCharacter() const;

	UFUNCTION(BlueprintPure)
		/* Get the battle instance that owns the battle this ability actor is participating in. */
		ABattleInstance* GetBattleInstance() const;

	UFUNCTION(BlueprintPure)
		/* Does this ability's team have authority in battle. */
		bool HasBattleAuthority() const;

	UFUNCTION(BlueprintPure)
		/* Get the ability data object that corresponds to this ability. */
		UAbilityDataObject* GetAbilityDataObject() const;

	UFUNCTION(BlueprintPure)
		/* Get the name of this ability. */
		FName GetAbilityName() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetDamagePower() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetDamageDuration() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetHealingPower() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetHealingDuration() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetSpeed() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetRange() const;

	UFUNCTION(BlueprintPure)
		/**/
		FBattleStats GetSpawnedActiveStats() const { return SpawnedActiveStats; }

	UFUNCTION(BlueprintPure)
		/**/
		FBattleStats GetAbilityInitialStats() const;

	UFUNCTION(BlueprintPure)
		/**/
		FBattleStats GetAbilityActiveStats() const;

	/**/
	virtual UDamageComponent* GetDamageComponent() const;

	virtual bool IsValidAbilityCollider_Implementation(UPrimitiveComponent* Collider) const override;

	virtual bool CanBeHealedByAbilityActor_Implementation(AAbilityActor* AbilityActor) const override;

	virtual bool CanBeDamagedByAbilityActor_Implementation(AAbilityActor* AbilityActor) const override;

	virtual ABattleTeam* GetTeam_Implementation() const override;

	virtual void AuthorityBattleTick_Implementation(float DeltaTime) override;

	virtual void ClientBattleTick_Implementation(float DeltaTime) override;

	virtual void IncreaseHealth_Implementation(float Amount) override;

	virtual void DecreaseHealth_Implementation(float Amount) override;

	virtual bool IsEnemy_Implementation(const TScriptInterface<IBattleActor>& OtherBattleActor) const override;

	virtual FBattleStats GetInitialStats_Implementation() const override;

	virtual FBattleStats GetActiveStats_Implementation() const override;

	virtual float TotalHealingFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityHealing) const override;

	virtual float TotalDamageFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityDamage) const override;

	virtual TArray<UImmieType*> GetType_Implementation() const override;

	virtual FString GetDisplayName_Implementation() const override;

	virtual void UpdateVisuals_Implementation() override;
};
