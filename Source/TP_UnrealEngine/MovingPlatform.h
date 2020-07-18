// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class TP_UNREALENGINE_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* platform;

	UPROPERTY(EditAnywhere)
		FVector startPoint;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
		FVector endPoint;

	UPROPERTY(EditAnywhere)
		float lerpSpeed;

	UPROPERTY(EditAnywhere)
	bool isGoing;

	float distance;

	UPROPERTY(EditAnywhere)
		float nearDistance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void DoSwap(FVector& originalVector, FVector& swapVector);
	
};
