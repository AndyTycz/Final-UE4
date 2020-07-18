// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "HeroCharacter.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = collider;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(GetRootComponent());

	idleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Idle particle"));
	idleParticle->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	collider->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	collider->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canRotate) {
		FRotator rot = GetActorRotation();
		rot.Yaw += DeltaTime * rotSpeed;
		SetActorRotation(rot);
	}

}

void AItem::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (OtherActor) {
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);

		if (hero) {
			if (pickParticle) {

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), pickParticle, GetActorLocation(), FRotator(0.f), true);
				if (pickupSound) {
					UGameplayStatics::PlaySound2D(this, pickupSound);
				}
			}
		}
	}


}

void AItem::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

