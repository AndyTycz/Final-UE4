// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelTransitioner.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "HeroCharacter.h"

// Sets default values
ALevelTransitioner::ALevelTransitioner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	transitionHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionHitbox"));
	RootComponent = transitionHitbox;

	canvas = CreateDefaultSubobject<UBillboardComponent>(TEXT("Canvas"));
	canvas->SetupAttachment(GetRootComponent());

	levelName = "Level01";
}

// Called when the game starts or when spawned
void ALevelTransitioner::BeginPlay()
{
	Super::BeginPlay();

	transitionHitbox->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitioner::OnOverlapBegin);
}

// Called every frame
void ALevelTransitioner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitioner::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AHeroCharacter* player = Cast<AHeroCharacter>(OtherActor);
		{
			if (player)
			{
				player->SwitchLevel(levelName);
			}
		}
	}
}


