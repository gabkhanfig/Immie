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

	UPROPERTY(BlueprintReadWrite)
		/**/
		class UMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		/**/
		class UProjectileMovementComponent* ProjectileComponent;

protected:

	/* Actors begin play. Note that not all objects and values will be initialized by the time this executes. */
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
		/* Informs clients or standalone of spawn. */
		void InformSpawn();

	UFUNCTION()
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
		/* Executes after AAbilityActor::OnCollision(). Can have base functionality overridden in blueprints. See "call to parent" functionality in blueprints. */
		void OnBattleActorCollision(const TScriptInterface<IBattleActor>& BattleActor, UPrimitiveComponent* ThisComponent, UPrimitiveComponent* OtherComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/* Event for handling ability actor collision with an ally battle actor. Can have base functionality overridden in blueprints. See "call to parent" functionality in blueprints. */
		void OnAllyCollision(const TScriptInterface<IBattleActor>& Ally, UPrimitiveComponent* ThisComponent, UPrimitiveComponent* OtherComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/* Event for handling ability actor collision with an enemy battle actor. Can have base functionality overridden in blueprints. See "call to parent" functionality in blueprints. */
		void OnEnemyCollision(const TScriptInterface<IBattleActor>& Enemy, UPrimitiveComponent* ThisComponent, UPrimitiveComponent* OtherComponent);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Authority Battle Tick")
		/**/
		void BP_AuthorityBattleTick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Client Battle Tick")
		/**/
		void BP_ClientBattleTick(float DeltaTime);

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
		/* Get the behavior flags of this ability. */
		FAbilityFlags GetAbilityFlags() const;

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

	virtual bool IsAlly_Implementation(const TScriptInterface<IBattleActor>& OtherBattleActor) const override;

	virtual FBattleStats GetInitialStats_Implementation() const override;

	virtual FBattleStats GetActiveStats_Implementation() const override;

	virtual float TotalHealingFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityHealing) const override;

	virtual float TotalDamageFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityDamage) const override;

	virtual TArray<UImmieType*> GetType_Implementation() const override;

	virtual FString GetDisplayName_Implementation() const override;

	virtual void UpdateVisuals_Implementation() override;
};
