// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "FlyingBook.generated.h"

/**
 *
 */
UCLASS()
class TP_UNREALENGINE_API AFlyingBook : public AItem
{
	GENERATED_BODY()


public:
	AFlyingBook();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int coinValue;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


};
