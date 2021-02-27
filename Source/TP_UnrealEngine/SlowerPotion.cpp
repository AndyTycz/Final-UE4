// Fill out your copyright notice in the Description page of Project Settings.

#include "SlowerPotion.h"
#include "HeroCharacter.h"
#include "Enemy_Melee.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"

ASlowerPotion::ASlowerPotion()
{
	amountTimeSlower = 5.f;
}

void ASlowerPotion::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor) {
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);

		if (hero)
		{
			hero->LowerSpeed(amountTimeSlower);

			Destroy();
			if (pickupSound) {
				UGameplayStatics::PlaySound2D(this, pickupSound);
			}
		}
	}
}

void ASlowerPotion::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
