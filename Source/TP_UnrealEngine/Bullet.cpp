// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = bulletMesh;
	bulletMesh->SetSimulatePhysics(false);
	bulletMesh->SetEnableGravity(false);
	bulletMesh->SetNotifyRigidBodyCollision(false);
	bulletMesh->BodyInstance.SetCollisionProfileName("OverlapAll");


}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	bulletMesh->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnTriggerEnter);

	SetLifeSpan(bulletLifeSpan);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * bulletSpeed * DeltaTime);
}

void ABullet::OnTriggerEnter(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);
		if (hero)
		{
			UGameplayStatics::ApplyDamage(hero, bulletDamage, nullptr, this, damageTypeClass);
			if (bulletParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, GetActorLocation(), FRotator(0.f), false);
			}
			UGameplayStatics::PlaySound2D(this, hitSound);
			Destroy();
		}	
	}
}