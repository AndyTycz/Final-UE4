// Fill out your copyright notice in the Description page of Project Settings.

#include "LastLevelManager.h"
#include "HeroCharacter.h"

// Sets default values
ALastLevelManager::ALastLevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALastLevelManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALastLevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALastLevelManager::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AHeroCharacter* player = Cast<AHeroCharacter>(OtherActor); 
		{
			if (player)
			{
				if (isWin)
				{
					player->SetWinScreen();
				}
				else
				{
					player->SetGameOverPause();
				}
			}
		}
	}

}

