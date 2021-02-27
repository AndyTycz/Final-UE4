// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageOrb.h"
#include "HeroCharacter.h"
#include "Enemy_Melee.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"

ADamageOrb::ADamageOrb() {

	damage = 25.f;
}

void ADamageOrb::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor) {
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);
		AEnemy_Melee* enem = Cast<AEnemy_Melee>(OtherActor);
		if (hero || enem) 
		{
			UGameplayStatics::ApplyDamage(OtherActor, damage, nullptr, this, damageTypeClass);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), pickParticle, GetActorLocation(), FRotator(0.f), true);
			if (pickupSound) {
				UGameplayStatics::PlaySound2D(this, pickupSound);
			}
			Destroy();
		}
	}

}

void ADamageOrb::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	
}


