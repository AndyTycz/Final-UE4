// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponDestroyer.h"
#include "Components/BoxComponent.h"
#include "HeroCharacter.h"

// Sets default values
AWeaponDestroyer::AWeaponDestroyer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	destroyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DestroyBox"));
	RootComponent = destroyBox;

}

// Called when the game starts or when spawned
void AWeaponDestroyer::BeginPlay()
{
	Super::BeginPlay();

	destroyBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponDestroyer::OnOverlapBegin);
	
}

// Called every frame
void AWeaponDestroyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

void AWeaponDestroyer::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AHeroCharacter* player = Cast<AHeroCharacter>(OtherActor);
		{
			//player->DestroyWeapon();
		}
	}
}

