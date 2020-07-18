// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"



ADoorSwitch::ADoorSwitch()
{

	PrimaryActorTick.bCanEverTick = true;


	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = triggerBox;

	triggerBox->SetCollisionEnabled((ECollisionEnabled::QueryOnly));
	triggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	triggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);							//Ignore all collisions
	triggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //Except pawn



	floorButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorButton"));
	floorButton->SetupAttachment(GetRootComponent());



	doorRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorRight"));
	doorRight->SetupAttachment(GetRootComponent());
	doorLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeft"));
	doorLeft->SetupAttachment(GetRootComponent());

}


void ADoorSwitch::BeginPlay()
{
	Super::BeginPlay();


	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADoorSwitch::OnTriggerEnter);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &ADoorSwitch::OnTriggerEnd);


	initialRotationLeft = doorLeft->GetComponentRotation();
	initialRotationRight = doorRight->GetComponentRotation();
	initialButtonPos = floorButton->GetComponentLocation();
}


void ADoorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorSwitch::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	OpenDoor();
	PressFloorButton();
}

void ADoorSwitch::OnTriggerEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
	CloseDoor();
	UnpressFloorButton();
}

void ADoorSwitch::UpdateDoorRotation(float rightDoorZ, float leftDoorZ)
{
	FRotator newRotLeft = initialRotationLeft;
	FRotator newRotRight = initialRotationRight;
	newRotLeft.Yaw += leftDoorZ;
	newRotRight.Yaw += rightDoorZ;

	doorLeft->SetWorldRotation(newRotLeft);
	doorRight->SetWorldRotation(newRotRight);
}

void ADoorSwitch::UpdateButtonPosition(float z)
{
	FVector newPos = initialButtonPos;
	newPos.Z += z;
	floorButton->SetWorldLocation(newPos);
}



