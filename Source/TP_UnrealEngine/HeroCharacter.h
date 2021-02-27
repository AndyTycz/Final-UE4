// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeroCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8 {
	EMType_Normal UMETA(DisplayName = "Normal"),
	EMType_Running UMETA(DisplayName = "Running"),
	EMType_Slowed UMETA(DisplayName = "Slowed"),
	EMType_Death UMETA(DisplayName = "Death")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8 {
	EStatus_Normal UMETA(DisplayName = "Normal"),
	EStatus_Minimum UMETA(DisplayName = "Minimum"),
	EStatus_Zero UMETA(DisplayName = "Depleted"),
	EStatus_Recovery UMETA(DisplayName = "Recovering")
};

UCLASS()
class TP_UNREALENGINE_API AHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AHeroCharacter();


#pragma region Run

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EMovementType movementType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float runSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float moveSpeed;

	bool isRunPressed;
	void ShiftKeyDown();
	void ShiftKeyUp();

#pragma endregion Run
#pragma region Stamina

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EStaminaStatus staminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus status) { staminaStatus = status; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float staminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float minSprintStamina;


#pragma endregion Stamina



	/** Palito para poner la camara atras del player
	/* AllowprivateAcess = Spring accessible in blueprint used but not outside of it*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAcces = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAcces = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stuff")
		float jumpPower;

	virtual void Jump() override;

	//Aim
	float aimSpeed;
	bool isAiming;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool hasAimTarget;
	FORCEINLINE void SetHasAimTarget(bool hasTarget) { hasAimTarget = hasTarget; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AEnemy_Melee* aimTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector aimTargetPosition;


	FORCEINLINE void SetAimTarget(AEnemy_Melee *target) { aimTarget = target; }
	FRotator GetLookAtYaw(FVector target);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float maxHP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float maxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float stamina;

		float damageMultiplierValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int knowledgePoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isAttacking;

	bool isSlowed;

	bool isBoosted;

	UPROPERTY(EditAnywhere)
		float att01Speed;
	UPROPERTY(EditAnywhere)
		float att02Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* hitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* hitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UAnimMontage* combatAnims;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class AWeapon* heroWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class AItem* currentOverlaped; //para agregar mas armas en el final :)


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AMainPlayerController* playerHUDController;


	FTimerHandle slowerTimer;

	FTimerHandle boostTimer;


protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveSides(float Value);

	void LockCamera();


	bool isLeftMousePressed;
	void LMBDown();
	void LMBUp();
	bool isRightMousePressed;
	void RMBDown();
	void RMBUp();

	bool isESCPressed;
	void ESCDown();
	void ESCUp();


	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void SetWeapon(AWeapon* weapon);
	FORCEINLINE AWeapon* GetWeapon() { return heroWeapon; }
	FORCEINLINE void SetCurrentOverlap(AItem* item) { currentOverlaped = item; }




	void SetMovementStatus(EMovementType type);

	void Attack();
	UFUNCTION(BlueprintCallable)
		void AttackEnd();



	void MoreBooks(int amount);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	void GetHealth(float amount);
	void GetStamina(float amount);

	void LowerSpeed(float time);
	void RestoreSpeed();

	void BoostDamage(float damageMult, float time);
	void RestoreDamage();


	void Ded();
	UFUNCTION(BlueprintCallable)
		void DeathEnd();

	void SwitchLevel(FName levelName);

	UFUNCTION(BlueprintCallable)
		void RestartLevel();

	void SetWinScreen();
	void SetGameOverPause();
};
