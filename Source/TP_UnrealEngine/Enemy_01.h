// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy_01.generated.h"

UENUM(BlueprintType)
enum class EEnemyStatus : uint8
{
	EState_Idle			UMETA(DisplayName = "Idle"),
	ESTate_MoveToHero	UMETA(DisplayName = "Chase"),
	EState_Attack		UMETA(DisplayName = "Attacking"),
	EState_Death		UMETA(DisplayName = "Dead")
};


UCLASS()
class TP_UNREALENGINE_API AEnemy_01 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_01();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EEnemyStatus enemyState;

	FORCEINLINE void SetEnemyState(EEnemyStatus state) { enemyState = state; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* agroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* attackSphere;


	UPROPERTY(EditAnywhere)
		float agroSphereRadius;
	UPROPERTY(EditAnywhere)
		float attackSphereRadius;

	UPROPERTY(BlueprintReadWrite)
		bool isOverlappingCombat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isAttacking;

	FVector idlePosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AAIController* myAIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AHeroCharacter* heroTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float enemyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> damageTypeClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* hitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* hitSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* swingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* weaponCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UAnimMontage* combatAnims;


	FTimerHandle deathTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float deathTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		virtual void AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void AxeOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void AxeOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;


	UFUNCTION(BlueprintCallable)
		void MoveToTarget(class AHeroCharacter* target);

	void GoBackToIdle(FVector pos);

	UFUNCTION(BlueprintCallable)
		void ActivateCollision();

	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();


	void Attack();

	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	
	void Die();

	UFUNCTION(BlueprintCallable)
		void DeathEnd();

	void VanishFromLife();

	bool Alive();
};
