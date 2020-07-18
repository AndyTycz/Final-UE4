// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitioner.generated.h"

UCLASS()
class TP_UNREALENGINE_API ALevelTransitioner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelTransitioner();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* transitionHitbox;

	class UBillboardComponent* canvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName levelName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


};
