// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp.h"
#include "HeroCharacter.h"

APickUp::APickUp() {

	HPRegenValue = 20.f;
}

void APickUp::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor) {
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);
		if (hero) {
			hero->GetHealth(HPRegenValue);
			Destroy();
		}
	}
}

void APickUp::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

