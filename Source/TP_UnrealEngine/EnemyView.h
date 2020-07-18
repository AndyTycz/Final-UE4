// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyView.generated.h"

/**
 *
 */
UCLASS()
class TP_UNREALENGINE_API UEnemyView : public UAnimInstance
{
	GENERATED_BODY()

public:

		virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
		void UpdateAnimations();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float movementSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class AEnemy_01* enemy;

};
