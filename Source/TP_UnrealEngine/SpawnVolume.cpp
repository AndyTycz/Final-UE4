// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy_01.h"
#include "AIController.h"
// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector extent = SpawnBox->GetScaledBoxExtent(); //box size
	FVector origin = SpawnBox->GetComponentLocation(); // box center

	return UKismetMathLibrary::RandomPointInBoundingBox(origin, extent); //clavo un punto entre el centro y los bordes de la box

}

void ASpawnVolume::SpawnMyActor_Implementation(UClass * ToSpawn, const FVector & location)
{
	if (ToSpawn) 
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters params;
		if (World) 
		{
			AActor* actor = World->SpawnActor<AActor>(ToSpawn, location, FRotator(0.f), params);
			AEnemy_01* enemy = Cast<AEnemy_01>(actor);
			if (enemy)
			{
				enemy->SpawnDefaultController();

				AAIController* AICont = Cast<AAIController>(enemy->GetController());
				if (AICont)
				{
					enemy->myAIController = AICont;
				}
			}
			
		}
	}
}

TSubclassOf<AActor> ASpawnVolume::GetActorSpawn()
{
	if (actorToSpawn01 && actorToSpawn02)
	{
		int random = FMath::RandRange(0, 1);
		if (random == 0) {
			return actorToSpawn01;
		}
		else {
			return actorToSpawn02;
		}
	}
	else return nullptr;
}

