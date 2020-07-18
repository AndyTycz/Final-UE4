// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_01.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "HeroCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "MainPlayerController.h"

// Sets default values
AEnemy_01::AEnemy_01()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	agroSphereRadius = 400.f;
	attackSphereRadius = 50.f;

	agroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	agroSphere->SetupAttachment(GetRootComponent());
	agroSphere->InitSphereRadius(agroSphereRadius);

	attackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	attackSphere->SetupAttachment(GetRootComponent());
	attackSphere->InitSphereRadius(attackSphereRadius);

	weaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AxeCollider"));
	weaponCollider->SetupAttachment(GetMesh(), FName("WeaponSocket"));

	isOverlappingCombat = false;

	health = 75.f;
	MaxHP = 50.f;
	enemyDamage = 15.f;

	deathTime = 2.f;

}

// Called when the game starts or when spawned
void AEnemy_01::BeginPlay()
{
	Super::BeginPlay();

	idlePosition = GetActorLocation();

	myAIController = Cast<AAIController>(GetController());


	agroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_01::AgroSphereOnOverlapBegin);
	agroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy_01::AgroSphereOnOverlapEnd);

	attackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_01::AttackSphereOnOverlapBegin);
	attackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy_01::AttackSphereOnOverlapEnd);

	weaponCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_01::AxeOnOverlapBegin);
	weaponCollider->OnComponentEndOverlap.AddDynamic(this, &AEnemy_01::AxeOnOverlapEnd);

	weaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	weaponCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	weaponCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	weaponCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	SetEnemyState(EEnemyStatus::EState_Idle);
}

// Called every frame
void AEnemy_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_01::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy_01::AgroSphereOnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && Alive()) {
		AHeroCharacter* player = Cast<AHeroCharacter>(OtherActor);
		{
			if (player)
			{
				MoveToTarget(player);				
			}

		}
	}
}

void AEnemy_01::AgroSphereOnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive()) {
		AHeroCharacter* player = Cast<AHeroCharacter>(OtherActor);
		{
			if (player)
			{
				SetEnemyState(EEnemyStatus::EState_Idle);
				if (myAIController) myAIController->StopMovement();
				GoBackToIdle(idlePosition);
				if (player->playerHUDController)
				{
					player->playerHUDController->HideEnemyHPBar();
				}
			}
		}
	}
}

void AEnemy_01::AttackSphereOnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && Alive())
	{
		AHeroCharacter* player = Cast<AHeroCharacter>(OtherActor);
		{
			if (player)
			{
				player->SetAimTarget(this);
				player->SetHasAimTarget(true);
				if (player->playerHUDController)
				{
					player->playerHUDController->ShowEnemyHPBar();
				}
				heroTarget = player;
				isOverlappingCombat = true;
				Attack();
			}
		}
	}
}

void AEnemy_01::AttackSphereOnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive())
	{
		AHeroCharacter* player = Cast<AHeroCharacter>(OtherActor);
		{
			if (player)
			{
				if (player->aimTarget == this)
				{
					player->SetAimTarget(nullptr);
				}
				player->SetHasAimTarget(false);
				isOverlappingCombat = false;
				if (enemyState != EEnemyStatus::EState_Attack)
				{
					MoveToTarget(player);
					heroTarget = nullptr;
				}

			}
		}
	}
}


void AEnemy_01::AxeOnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && Alive())
	{
		AHeroCharacter* player = Cast<AHeroCharacter>(OtherActor);
		if (player)
		{
			if (player->hitParticle)
			{
				const USkeletalMeshSocket* particleSocket = GetMesh()->GetSocketByName("ParticleSocket");
				if (particleSocket)
				{
					FVector socketPos = particleSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), player->hitParticle, socketPos, FRotator(0.f), false);
				}
			}
			if (player->hitSound)
			{
				UGameplayStatics::PlaySound2D(this, player->hitSound);
			}

			if (damageTypeClass)
			{
				UGameplayStatics::ApplyDamage(player, enemyDamage, myAIController, this, damageTypeClass);
			}
		}
	}
}

void AEnemy_01::AxeOnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy_01::MoveToTarget(AHeroCharacter * target)
{
	SetEnemyState(EEnemyStatus::ESTate_MoveToHero);

	if (myAIController && Alive())
	{
		FAIMoveRequest moveRequest;
		moveRequest.SetGoalActor(target);
		moveRequest.SetAcceptanceRadius(10.f);

		FNavPathSharedPtr navPath;

		myAIController->MoveTo(moveRequest, &navPath);

		/*auto pathPoints = navPath->GetPathPoints();
		for (auto point : pathPoints) {
			FVector pos = point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, pos, 25.f, 8, FLinearColor::Blue, 15.f, 0.5f);
		}*/
	}
}

void AEnemy_01::GoBackToIdle(FVector pos)
{
	if (myAIController && Alive())
	{
		FAIMoveRequest moveRequest;
		moveRequest.SetGoalLocation(pos);
		moveRequest.SetAcceptanceRadius(5.f);

		FNavPathSharedPtr navPath;

		myAIController->MoveTo(moveRequest, &navPath);
	}
}


void AEnemy_01::ActivateCollision()
{
	weaponCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AEnemy_01::DeactivateCollision()
{
	weaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy_01::Attack()
{
	if (Alive())
	{
		if (myAIController)
		{
			myAIController->StopMovement();
			SetEnemyState(EEnemyStatus::EState_Attack);
		}
		if (!isAttacking)
		{
			isAttacking = true;
			UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
			if (animInstance)
			{
				animInstance->Montage_Play(combatAnims, 1.f);
				animInstance->Montage_JumpToSection(FName("Attack"), combatAnims);
				if (heroTarget)
				{
					FRotator lookPlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), heroTarget->GetActorLocation());
					lookPlayer.Pitch = 0.f;
					lookPlayer.Roll = 0.f;
					SetActorRotation(lookPlayer);
				}
			}
		}
	}
}

void AEnemy_01::AttackEnd()
{
	if (Alive())
	{
		isAttacking = false;
		if (isOverlappingCombat)
		{
			Attack();
		}
		else
		{
			MoveToTarget(heroTarget);
		}
	}
}


float AEnemy_01::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (health - DamageAmount <= 0.f)
	{
		health -= DamageAmount;
		Die();
	}
	else
	{
		health -= DamageAmount;
	}
	return DamageAmount;
}

void AEnemy_01::Die()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance)
	{
		animInstance->Montage_Play(combatAnims, 1.f);
		animInstance->Montage_JumpToSection(FName("Death"), combatAnims);
	}
	SetEnemyState(EEnemyStatus::EState_Death);

	weaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	agroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEnemy_01::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;


	GetWorldTimerManager().SetTimer(deathTimer, this, &AEnemy_01::VanishFromLife, deathTime);
	SetEnemyState(EEnemyStatus::EState_Death);
}

bool AEnemy_01::Alive()
{
	return enemyState != EEnemyStatus::EState_Death;
}

void AEnemy_01::VanishFromLife()
{
	Destroy();
}