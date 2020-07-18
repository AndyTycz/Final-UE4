// Fill out your copyright notice in the Description page of Project Settings.

#include "StaminaPickUp.h"
#include "HeroCharacter.h"

AStaminaPickUp::AStaminaPickUp()
{
	StaminaRegenValue = 50.f;
}

void AStaminaPickUp::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor) {
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);
		if (hero) {
			hero->GetStamina(StaminaRegenValue);
			Destroy();
		}
	}
}

void AStaminaPickUp::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}