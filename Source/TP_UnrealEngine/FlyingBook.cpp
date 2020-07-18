// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingBook.h"
#include "HeroCharacter.h"

AFlyingBook::AFlyingBook() {

}

void AFlyingBook::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor) {
		AHeroCharacter* hero = Cast<AHeroCharacter>(OtherActor);
		if (hero) {
			hero->MoreBooks(1);
			Destroy();
		}
	}
}

void AFlyingBook::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

