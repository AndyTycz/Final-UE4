// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_Ranged.h"
#include "Components/SkeletalMeshComponent.h"
#include "HeroCharacter.h"
#include "Components/SphereComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Bullet.h"
#include "Sound/SoundCue.h"

// Sets default values
AEnemy_Ranged::AEnemy_Ranged()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootcomp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	rootcomp->SetupAttachment(GetRootComponent());

	pawnSenses = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn senses"));
	pawnSenses->bHearNoises = false;

	skelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body mesh"));
	skelMesh->SetupAttachment(rootcomp);

	bulletSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawner"));
	bulletSpawner->SetupAttachment(skelMesh);
}

// Called when the game starts or when spawned
void AEnemy_Ranged::BeginPlay()
{
	Super::BeginPlay();

	canShoot = true;
	pawnSenses->OnSeePawn.AddDynamic(this, &AEnemy_Ranged::LookAtPlayer);
}

// Called every frame
void AEnemy_Ranged::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_Ranged::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy_Ranged::LookAtPlayer(APawn *inPawn)
{
	AHeroCharacter* player = Cast<AHeroCharacter>(inPawn);
	{
		if (player)
		{		
			FRotator newRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetActorLocation());
			SetActorRotation(newRot);
			if (canShoot)
				AttackPlayer();
		}
	}
}

void AEnemy_Ranged::AttackPlayer()
{
	if (bulletPref)
	{
		UGameplayStatics::PlaySound2D(this, shootSound);
		FActorSpawnParameters params;
		GetWorld()->SpawnActor<ABullet>(bulletPref, bulletSpawner->GetComponentLocation(), GetActorRotation(), params);
		canShoot = false;
	}
}

void AEnemy_Ranged::AttackTimer()
{
	canShoot = true;
}


