// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorSwitch.generated.h"

UCLASS()
class TP_UNREALENGINE_API ADoorSwitch : public AActor
{
	GENERATED_BODY()

public:

	ADoorSwitch();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger switch")
		class UBoxComponent* triggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger switch")
		class UStaticMeshComponent* floorButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
		UStaticMeshComponent* doorRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
		UStaticMeshComponent* doorLeft;

	UPROPERTY(BlueprintReadWrite, Category = "Trigger switch")
		FRotator initialRotationLeft;
	UPROPERTY(BlueprintReadWrite, Category = "Trigger switch")
		FRotator initialRotationRight;

	UPROPERTY(BlueprintReadWrite, Category = "Trigger switch")
		FVector initialButtonPos;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintImplementableEvent, Category = "Trigger switch")
		void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Trigger switch")
		void CloseDoor();


	UFUNCTION(BlueprintImplementableEvent, Category = "Trigger switch")
		void PressFloorButton();

	UFUNCTION(BlueprintImplementableEvent, Category = "Trigger switch")
		void UnpressFloorButton();

	UFUNCTION(BlueprintCallable, Category = "Trigger switch")
		void UpdateDoorRotation(float rightDoorZ, float leftDoorZ);

	UFUNCTION(BlueprintCallable, Category = "Trigger switch")
		void UpdateButtonPosition(float z);
};
