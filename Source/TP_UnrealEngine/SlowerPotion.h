// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "SlowerPotion.generated.h"

/**
 *
 */
UCLASS()
class TP_UNREALENGINE_API ASlowerPotion : public AItem
{
	GENERATED_BODY()

public:

	ASlowerPotion();

	UPROPERTY(EditAnywhere)
		float amountTimeSlower;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
