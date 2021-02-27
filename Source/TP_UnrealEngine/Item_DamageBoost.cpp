// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_DamageBoost.h"
#include "HeroCharacter.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

AItem_DamageBoost::AItem_DamageBoost()
{
	boostTime = 5.f;
	damageMultiplier = 2.f;
}



void AItem_DamageBoost::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor) {
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);

		if (hero)
		{
			hero->BoostDamage(damageMultiplier, boostTime);

			Destroy();
			if (pickupSound) {
				UGameplayStatics::PlaySound2D(this, pickupSound);
			}
		}
	}
}

void AItem_DamageBoost::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
