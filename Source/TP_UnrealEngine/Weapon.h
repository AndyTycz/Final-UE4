// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	E_WState_inGame UMETA(DisplayName = "ForPickup"),
	E_WState_Equipped UMETA(DisplayName = "Equipped")

};

/**
 *
 */
UCLASS()
class TP_UNREALENGINE_API AWeapon : public AItem
{
	GENERATED_BODY()


public:

	AWeapon();

	UPROPERTY(VisibleAnywhere)
		EWeaponState weaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* onGrabSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* swingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent*  weaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* swordCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool keepParticles;

protected:

	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
		void SwordOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void SwordOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void EquipOnHero(class AHeroCharacter* hero);

	UFUNCTION(BlueprintCallable)
		void ActivateCollision();

	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();



	FORCEINLINE void SetWeaponState(EWeaponState state) { weaponState = state; }
	FORCEINLINE EWeaponState GetWeaponState() { return weaponState; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> damageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AController* weaponInstigator;

	FORCEINLINE void SetInstigator(AController* instigator) { weaponInstigator = instigator; }
};
