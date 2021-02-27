// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy_Ranged.generated.h"

UCLASS()
class TP_UNREALENGINE_API AEnemy_Ranged : public APawn
{
	GENERATED_BODY()




public:
	
	AEnemy_Ranged();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* hitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* shootSound;

	
		class USceneComponent* rootcomp;
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* skelMesh;
	UPROPERTY(EditAnywhere)
		class UPawnSensingComponent* pawnSenses;
	UPROPERTY(EditAnywhere)
		class USceneComponent* bulletSpawner;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABullet> bulletPref;




	UPROPERTY(BlueprintReadWrite)
		bool canShoot;

	bool followPlayer;

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void LookAtPlayer(APawn *inPawn);

		void AttackPlayer();

	UFUNCTION(BlueprintCallable)
		void AttackTimer();

};
