// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	RootComponent = platform;

	endPoint = FVector(0.f);
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	startPoint = GetActorLocation();
	endPoint += startPoint;

	distance = (endPoint - startPoint).Size();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector currentPos = GetActorLocation();

	FVector lerp = FMath::VInterpTo(currentPos, endPoint, DeltaTime, lerpSpeed);
	SetActorLocation(lerp);

	float pos = (GetActorLocation() - startPoint).Size();
	if (distance - pos <= nearDistance) DoSwap(startPoint, endPoint);

}

void AMovingPlatform::DoSwap(FVector& originalVector, FVector& swapVector)
{
	FVector temp = originalVector;
	originalVector = swapVector;
	swapVector = temp;

}

