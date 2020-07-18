// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TP_UNREALENGINE_API ABullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "My Mesh")
		class UStaticMeshComponent* bulletMesh;
	

public:

	UPROPERTY(EditAnywhere, Category = "My Mesh")
		class UParticleSystem* bulletParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* hitSound;

	// Sets default values for this actor's properties
	ABullet();

	UPROPERTY(EditAnywhere)
		float bulletLifeSpan;
	UPROPERTY(EditAnywhere)
		float bulletSpeed;
	UPROPERTY(EditAnywhere)
		float bulletDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> damageTypeClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
