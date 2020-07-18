// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroView.generated.h"

/**
 *
 */
UCLASS()
class TP_UNREALENGINE_API UHeroView : public UAnimInstance
{
	GENERATED_BODY()


public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = Animations)
	void UpdateAnimations();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool isJumping;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class AHeroCharacter* hero;

};
