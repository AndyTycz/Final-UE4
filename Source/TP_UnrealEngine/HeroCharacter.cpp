// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy_01.h"
#include "MainPlayerController.h"
#include "TimerManager.h"

AHeroCharacter::AHeroCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	//Create camera boom (Los scripts de UE lo tienen con este nombre ¯\_(._.)_/¯)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; //Camera distance
	CameraBoom->bUsePawnControlRotation = true; //Rotate based on controller

	//Tamaño para la collision capsule
	GetCapsuleComponent()->SetCapsuleSize(25.f, 88.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	jumpPower = 500.f;

	//El character mira para donde se mueve
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); //a donde le oriento la rotacion (como si fuera un lerp)
	GetCharacterMovement()->JumpZVelocity = jumpPower;
	GetCharacterMovement()->AirControl = .4f;


	maxHP = 100.f;
	health = 75.f;
	maxStamina = 300.f;
	stamina = 120.f;
	knowledgePoints = 0;


	moveSpeed = 600.f;
	runSpeed = 900.f;

	staminaDrainRate = 35.f;
	minSprintStamina = 50.f;

	att01Speed = 1.4f;
	att02Speed = 2.f;

	//enums
	movementType = EMovementType::EMType_Normal;
	staminaStatus = EStaminaStatus::EStatus_Normal;

	aimSpeed = 15.f;
	

	isLeftMousePressed = false;
	isRightMousePressed = false;
	isESCPressed = false;
	isSlowed = false;

}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	bUseControllerRotationYaw = false;

	playerHUDController = Cast<AMainPlayerController>(GetController());

	isAiming = false;
	hasAimTarget = false;

}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (movementType == EMovementType::EMType_Death) return;

	float deltaStamina = staminaDrainRate * DeltaTime;

	//el pequeño cancercito que queria ser una state machine
	switch (staminaStatus)
	{
	case EStaminaStatus::EStatus_Normal:
		if (isRunPressed && GetVelocity().X != 0.f) {
			SetMovementStatus(EMovementType::EMType_Running);
			if (stamina - deltaStamina <= minSprintStamina) {
				SetStaminaStatus(EStaminaStatus::EStatus_Minimum);
				stamina -= deltaStamina;
			}
			else  stamina -= deltaStamina;
		}
		else
		{
			if (stamina + deltaStamina >= maxStamina) stamina = maxStamina;
			else stamina += deltaStamina;

			SetMovementStatus(EMovementType::EMType_Normal);
		}
		break;

	case EStaminaStatus::EStatus_Minimum:

		if (isRunPressed && GetVelocity().X != 0.f)
		{
			if (stamina - deltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::EStatus_Zero);
				stamina = 0;
				SetMovementStatus(EMovementType::EMType_Normal);
			}
			else
			{
				stamina -= deltaStamina;
				SetMovementStatus(EMovementType::EMType_Running);
			}
		}
		else
		{
			if (stamina + deltaStamina >= minSprintStamina) {
				SetStaminaStatus(EStaminaStatus::EStatus_Normal);
				stamina += deltaStamina;
			}
			else
				stamina += deltaStamina;

			SetMovementStatus(EMovementType::EMType_Normal);
		}
		break;

	case EStaminaStatus::EStatus_Zero:
		if (isRunPressed)
		{
			stamina = 0;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::EStatus_Recovery);
			stamina += deltaStamina;
		}
		SetMovementStatus(EMovementType::EMType_Normal);
		break;

	case EStaminaStatus::EStatus_Recovery:
		if (stamina += deltaStamina >= minSprintStamina) {
			SetStaminaStatus(EStaminaStatus::EStatus_Normal);
			stamina += deltaStamina;
		}
		else {
			stamina += deltaStamina;
			SetStaminaStatus(EStaminaStatus::EStatus_Recovery);
		}
		SetMovementStatus(EMovementType::EMType_Normal);
		break;
	default:
		break;
	}


	if (isAiming && aimTarget)
	{
		FRotator lookAt = GetLookAtYaw(aimTarget->GetActorLocation());
		FRotator aimRotation = FMath::RInterpTo(GetActorRotation(), lookAt, DeltaTime, aimSpeed);
		SetActorRotation(aimRotation);
	}
	if (aimTarget)
	{
		aimTargetPosition = aimTarget->GetActorLocation();
		if (playerHUDController)
		{
			playerHUDController->enemyPosition = aimTargetPosition;
		}
	}
}

void AHeroCharacter::Jump()
{
	if (movementType != EMovementType::EMType_Death && !playerHUDController->isPauseMenuVisible)
	{
		Super::Jump();
	}
}

FRotator AHeroCharacter::GetLookAtYaw(FVector target)
{
	FRotator getLookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target);
	return FRotator(0.f, getLookAtRot.Yaw, 0.f);
}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//si esto no es valido para la ejecucion
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AHeroCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AHeroCharacter::ShiftKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AHeroCharacter::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AHeroCharacter::LMBUp);

	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AHeroCharacter::RMBDown);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &AHeroCharacter::RMBUp);

	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AHeroCharacter::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AHeroCharacter::ESCUp);

	PlayerInputComponent->BindAction("LockCamera", IE_Pressed, this, &AHeroCharacter::LockCamera);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveSides", this, &AHeroCharacter::MoveSides);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}

void AHeroCharacter::MoveForward(float Value)
{
	if (movementType == EMovementType::EMType_Death) return;
	if (Controller != nullptr && Value != 0.f && !isAttacking && !playerHUDController->isPauseMenuVisible)
	{

		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0.f, rotation.Yaw, 0.f);

		//consigo el vector en una direccion(EAxis::x) basada en el rotator (yawRotation)
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, Value);
	}
}

void AHeroCharacter::MoveSides(float Value)
{
	if (movementType == EMovementType::EMType_Death) return;
	if (Controller != nullptr && Value != 0.f && !isAttacking && !playerHUDController->isPauseMenuVisible)
	{

		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0.f, rotation.Yaw, 0.f);

		//consigo el vector en una direccion(EAxis::x) basada en el rotator (yawRotation)
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, Value);
	}
}

void AHeroCharacter::LockCamera()
{
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}


void AHeroCharacter::SetMovementStatus(EMovementType type)
{
	if (isSlowed) return;
	movementType = type;
	if (movementType == EMovementType::EMType_Running) {
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	}
	else if (movementType == EMovementType::EMType_Normal) {

		GetCharacterMovement()->MaxWalkSpeed = moveSpeed;
	}
}

#pragma region Inputs pressed released

void AHeroCharacter::ShiftKeyDown() {
	isRunPressed = true;
}
void AHeroCharacter::ShiftKeyUp() {
	isRunPressed = false;
}

void AHeroCharacter::LMBDown() {
	isLeftMousePressed = true;

	if (heroWeapon && isAttacking == false) {
		Attack();
	}
}
void AHeroCharacter::LMBUp() {
	isLeftMousePressed = false;

}
void AHeroCharacter::RMBDown() {
	isRightMousePressed = true;

	if (movementType == EMovementType::EMType_Death) return;

	if (currentOverlaped) {
		AWeapon* weapon = Cast<AWeapon>(currentOverlaped);
		if (weapon) {
			weapon->EquipOnHero(this);
			SetCurrentOverlap(nullptr);
		}
	}
}
void AHeroCharacter::RMBUp() {
	isRightMousePressed = false;
	SetAimTarget(nullptr);
}

void AHeroCharacter::ESCDown()
{
	isESCPressed = true;
	if (playerHUDController)
	{
		playerHUDController->TogglePauseMenu();
	}
}
void AHeroCharacter::ESCUp()
{
	isESCPressed = false;
}

#pragma endregion Inputs pressed released


void AHeroCharacter::Attack()
{
	if (movementType == EMovementType::EMType_Death) return;

	if (!isAttacking && !playerHUDController->isPauseMenuVisible) {
		isAttacking = true;
		isAiming = true;
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

		if (animInstance && combatAnims)
		{
			int random = FMath::RandRange(0, 1);
			if (random == 0) {

				animInstance->Montage_Play(combatAnims, att01Speed);
				animInstance->Montage_JumpToSection(FName("Attack_01"), combatAnims);
			}
			else
			{
				animInstance->Montage_Play(combatAnims, att02Speed);
				animInstance->Montage_JumpToSection(FName("Attack_02"), combatAnims);
			}
		}
	}
}

void AHeroCharacter::AttackEnd()
{
	isAiming = false;
	isAttacking = false;
	if (isLeftMousePressed)
		Attack();
}

void AHeroCharacter::MoreBooks(int amount) {
	knowledgePoints += amount;
}

float AHeroCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (!playerHUDController->isPauseMenuVisible)
	{

		if (health - DamageAmount <= 0.f) {
			health -= DamageAmount;
			if (health <= 0.f) {
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Ded();
				health = 0.f;
			}
		}
		else {

			health -= DamageAmount;
		}
	}
	return DamageAmount;
}

void AHeroCharacter::GetHealth(float amount)
{
	health += amount;
	if (health > maxHP) health = maxHP;
}
void AHeroCharacter::GetStamina(float amount)
{
	stamina += amount;
	if (stamina >= maxStamina) stamina = maxStamina;
}

void AHeroCharacter::LowerSpeed(float time)
{
	isSlowed = true;
	GetWorldTimerManager().SetTimer(slowerTimer, this, &AHeroCharacter::RestoreSpeed, time);
	GetCharacterMovement()->MaxWalkSpeed = (moveSpeed / 3.f);
	SetMovementStatus(EMovementType::EMType_Slowed);
}

void AHeroCharacter::RestoreSpeed()
{
	SetMovementStatus(EMovementType::EMType_Normal);
	isSlowed = false;
	GetCharacterMovement()->MaxWalkSpeed = moveSpeed;
}

void AHeroCharacter::Ded() {

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && combatAnims)
	{
		animInstance->Montage_Play(combatAnims, 1.5f);
		animInstance->Montage_JumpToSection(FName("Death"));
	}

	SetMovementStatus(EMovementType::EMType_Death);

}
void AHeroCharacter::DeathEnd()
{
	GetCharacterMovement()->JumpZVelocity = 0.f;
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHeroCharacter::SetWeapon(AWeapon * weapon)
{//para agregar mas armas
	if (heroWeapon) {

		heroWeapon->Destroy();
	}
	heroWeapon = weapon;
	heroWeapon->weaponState = EWeaponState::E_WState_Equipped;
}

void AHeroCharacter::SwitchLevel(FName levelName)
{
	UWorld* world = GetWorld();
	if (world)
	{
		FString currentLevel = world->GetMapName();

		FName currentLevelName(*currentLevel);

		if (currentLevelName != levelName)
		{
			UGameplayStatics::OpenLevel(world, levelName);
		}
	}
}

void AHeroCharacter::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AHeroCharacter::SetWinScreen()
{

}

void AHeroCharacter::SetGameOverPause()
{
	if (playerHUDController)
		playerHUDController->ShowPauseMenu();
}
