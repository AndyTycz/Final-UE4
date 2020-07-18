// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "HeroCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy_01.h"

AWeapon::AWeapon() {

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	weaponMesh->SetupAttachment(GetRootComponent());

	swordCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollider"));
	swordCollider->SetupAttachment(GetRootComponent());

	

	keepParticles = false;

	weaponState = EWeaponState::E_WState_inGame;

	damage = 25.f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	swordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	swordCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	swordCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	swordCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	swordCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::SwordOnOverlapBegin);
	swordCollider->OnComponentEndOverlap.AddDynamic(this, &AWeapon::SwordOnOverlapEnd);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (weaponState == EWeaponState::E_WState_inGame && OtherActor) {
		//weaponState = EWeaponState::E_WState_Equipped;
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);
		if (hero) {
			hero->SetCurrentOverlap(this);
		}
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor) {
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);
		if (hero) {
			hero->SetCurrentOverlap(nullptr);
		}
	}
}

void AWeapon::SwordOnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AEnemy_01* enemy = Cast<AEnemy_01>(OtherActor);
		if (enemy)
		{
			if (enemy->hitParticle)
			{
				const USkeletalMeshSocket* particleSocket = weaponMesh->GetSocketByName("ParticleSocket");
				if (particleSocket)
				{
					FVector socketPos = particleSocket->GetSocketLocation(weaponMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), enemy->hitParticle, socketPos, FRotator(0.f), false);
				}
			}
			if (enemy->hitSound)
			{
				UGameplayStatics::PlaySound2D(this, enemy->hitSound);
			}
			if (damageTypeClass)
			{
				UGameplayStatics::ApplyDamage(enemy, damage, weaponInstigator, this, damageTypeClass);
			}
		}
	}
}

void AWeapon::SwordOnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::EquipOnHero(AHeroCharacter * hero)
{
	if (hero) {

		SetInstigator(hero->GetController());

		weaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		weaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		weaponMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* handSocket = hero->GetMesh()->GetSocketByName("SwordSocket");
		if (handSocket) {
			handSocket->AttachActor(this, hero->GetMesh());

			canRotate = false;

			hero->SetWeapon(this);
			hero->SetCurrentOverlap(nullptr);
		}
		if (onGrabSound) UGameplayStatics::PlaySound2D(this, onGrabSound);
		if (!keepParticles)
		{
			idleParticle->Deactivate();
		}
	}
}


void AWeapon::ActivateCollision() 
{
	swordCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


void AWeapon::DeactivateCollision() 
{
	swordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
