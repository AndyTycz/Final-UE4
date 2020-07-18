// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroView.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeroCharacter.h"

void UHeroView::NativeInitializeAnimation()
{
	if (pawn == nullptr) {
		pawn = TryGetPawnOwner();
		if (pawn) hero = Cast<AHeroCharacter>(pawn);
	}


}

void UHeroView::UpdateAnimations()
{
	if (pawn == nullptr) {
		pawn = TryGetPawnOwner();
	}

	if (pawn) {

		FVector speed = pawn->GetVelocity();
		FVector groundSpeed = FVector(speed.X, speed.Y, 0.f);
		movementSpeed = groundSpeed.Size();


		isJumping = pawn->GetMovementComponent()->IsFalling();

		if (hero == nullptr) {
			hero = Cast<AHeroCharacter>(pawn);
		}
	}

}
