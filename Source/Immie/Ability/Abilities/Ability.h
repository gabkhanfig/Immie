// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include <Immie/ImmieCore.h>
#include <Immie/Ability/AbilityDataTypes.h>
#include "Ability.generated.h"

class UImmie;
class AImmieCharacter;
class UAbilityDataObject;


UCLASS(Blueprintable)
class IMMIE_API UAbility : public UActorComponent
{
	GENERATED_BODY()

private:

	/* The id of this ability. Copied from global ability to battle ability. */
	int AbilityId;

	/* The ability data object containing the relevant unmodifiable ability data. Copied on client side from network syncing within the battle ability manager itself. */
	UAbilityDataObject* AbilityDataObject;

	/* Is the input for this ability currently being held down. Replicates from client TO server. */
	uint8 bInputHeld : 1;

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Battle|Stats")
		/* The unmodified stats of this ability. Health is the max health. Attack, defense, and speed are all the stat values without any modifiers. */
		FBattleStats InitialStats;

	UPROPERTY(BlueprintReadWrite, Category = "Battle|Stats")
		/* The potentially modified stats of this ability. Health is the current health. Attack, defense, and speed are all the stat values with modifiers. */
		FBattleStats ActiveStats;

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
		/* The battle type objects of this ability. */
		TArray<UImmieType*> Type;

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
		/* The current cooldown of this ability. Determines when it's uses will restock. */
		float CurrentCooldown;

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
		/* The current amount of uses this ability has. Decrements by one per use. Naturally, when this value is 0, the ability cannot be used. */
		int CurrentUses;

	UPROPERTY(BlueprintReadWrite, Category = "Battle|Input")
		/**/
		float CurrentDelay;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
		/* Multicast RPC to sync ability data to all connected clients. */
		void SyncClientAbilityData(
			int _AbilityId,
			FBattleStats _InitialStats,
			FBattleStats _ActiveStats,
			const TArray<UImmieType*>& _Type, 
			float _CurrentCooldown, 
			int _CurrentUses,
			float _CurrentDelay
		);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Initialize For Battle")
		/* Ability Data Object can be cast to the corresponding ability data object class.
		Blueprint sided initialization for battle. See UAbility::InitializeForBattle() for what is already handled. */
		void BP_InitializeForBattle(UAbilityDataObject* _AbilityDataObject);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Sync To Clients")
		/* Blueprint sided handling to sync this ability's data to the clients. Anything handled already in UAbility::SyncToClients will not need to be synced.
		Use reliable RPC to guarantee ordering in the event the blueprint rpc relies on values defined in UAbility itself. */
		void BP_SyncToClients();

	/* Execute what should happen when this ability input is pressed. */
	void ExecuteInputPress();

	UFUNCTION(Server, Reliable)
		/* RPC to inform server that input was pressed for this ability. */
		void ServerInputPress();
	
	UFUNCTION(NetMulticast, Reliable)
		/* TODO check if its ok with AI controllers on network. */
		void InformClientsInputPress();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "On Input Press")
		/**/
		void BP_OnInputPress(bool HasBattleAuthority, FAbilityFlags AbilityFlags);

	/* Execute what should happen when this ability input is released. */
	void ExecuteInputRelease();

	UFUNCTION(Server, Reliable)
		/* RPC to inform server that input was released for this ability. */
		void ServerInputRelease();

	UFUNCTION(NetMulticast, Reliable)
		/* TODO check if its ok with AI controllers on network. */
		void InformClientsInputRelease();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "On Input Release")
		/* Blueprint sided handling for when this ability has it's input released. Executes on all clients and server. Ensure checking correct battle authority. See UAbility::HasBattleAuthority() */
		void BP_OnInputRelease(bool HasBattleAuthority, FAbilityFlags AbilityFlags);

	UFUNCTION(BlueprintCallable)
		/* Spawns and initializes the default actor class of this ability. */
		AAbilityActor* SpawnAbilityActor(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
		/* Spawns and initializes an ability actor from a provided class. */
		AAbilityActor* SpawnAbilityActorFromClass(TSubclassOf<AAbilityActor> AbilityActorClass, const FTransform& SpawnTransform);

public:	

	UAbility();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Make a new ability object instance, also setting the ability id.  */
	static UAbility* NewAbility(AImmieCharacter* Owner, int _AbilityId);

	void InitializeForBattle();

	/* Called on server-side to syncrhonize all connected client's ability data. */
	void SyncToClients();

	/* Called by immie character controller for the ability's input pressed. */
	void InputPress();

	/* Called by immie character controller for the ability's input released. */
	void InputRelease();

	/* Authority active battle ticking for this ability. */
	void AuthorityBattleTick(float DeltaTime);

	/* Client active battle ticking for this ability. */
	void ClientBattleTick(float DeltaTime);

	UFUNCTION(BlueprintPure)
		/* Get the id number of this ability. */
		FORCEINLINE int GetAbilityId() const { return AbilityId; }

	UFUNCTION(BlueprintPure)
		/* Get the Immie character that owns this ability. */
		AImmieCharacter* GetImmieCharacter() const;

	UFUNCTION(BlueprintPure)
		/* Get this abilities battle team. */
		ABattleTeam* GetTeam() const;

	UFUNCTION(BlueprintPure)
		/**/
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
		UClass* GetActorClass() const;

	UFUNCTION(BlueprintPure)
		/**/
		FAbilityFlags GetAbilityFlags() const;

	UFUNCTION(BlueprintPure)
		/**/
		int GetTypeBitmask() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetInitialCooldown() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetMaxCooldown() const;

	UFUNCTION(BlueprintPure)
		/**/
		int GetInitialUses() const;

	UFUNCTION(BlueprintPure)
		/**/
		int GetMaxUses() const;

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
		float GetSpeed() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetRange() const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetMaxDelay() const;

	UFUNCTION(BlueprintPure)
		/**/
		FBaseStats GetBaseStats() const;

	UFUNCTION(BlueprintPure)
		/**/
		FBattleStats GetRelativeStats() const;

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE FBattleStats GetInitialStats() const { return InitialStats; }

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE FBattleStats GetActiveStats() const { return ActiveStats; }

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE TArray<UImmieType*> GetType() const { return Type; }


};
