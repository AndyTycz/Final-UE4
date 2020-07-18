// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DamageOrb.generated.h"

/**
 *
 */
UCLASS()
class TP_UNREALENGINE_API ADamageOrb : public AItem
{
	GENERATED_BODY()


public:
	ADamageOrb();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> damageTypeClass;
};
