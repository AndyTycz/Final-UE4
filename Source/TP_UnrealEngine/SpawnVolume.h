// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class TP_UNREALENGINE_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AActor> actorToSpawn01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AActor> actorToSpawn02;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintPure)
		FVector GetSpawnPoint();

	UFUNCTION(BlueprintPure)
		TSubclassOf<AActor> GetActorSpawn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SpawnMyActor(UClass* ToSpawn, const FVector& location);

};
