// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyView.h"
#include "Enemy_Melee.h"



void UEnemyView::NativeInitializeAnimation()
{
	if (pawn == nullptr)
	{
		pawn = TryGetPawnOwner();
		if (pawn)
		{
			enemy = Cast<AEnemy_Melee>(pawn);
		}
	}
}

void UEnemyView::UpdateAnimations()
{
	if (pawn == nullptr)
	{
		pawn = TryGetPawnOwner();
		if (pawn)
		{
			enemy = Cast<AEnemy_Melee>(pawn);
		}
	}
	if (pawn) {
		FVector speed = pawn->GetVelocity();
		FVector groundSpeed = FVector(speed.X, speed.Y, 0.f);
		movementSpeed = groundSpeed.Size();
	}
}
