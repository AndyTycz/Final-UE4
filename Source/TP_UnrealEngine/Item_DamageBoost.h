// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Item_DamageBoost.generated.h"

/**
 *
 */
UCLASS()
class TP_UNREALENGINE_API AItem_DamageBoost : public AItem
{
	GENERATED_BODY()

public:

	AItem_DamageBoost();

	UPROPERTY(EditAnywhere)
		float damageMultiplier;
	UPROPERTY(EditAnywhere)
		float boostTime;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
