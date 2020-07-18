// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 *
 */
UCLASS()
class TP_UNREALENGINE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Things")
		TSubclassOf<class UUserWidget> HUDOverlayWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Things")
		UUserWidget* HUDOverlay;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Things")
		TSubclassOf<UUserWidget> enemyHPBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD Things")
		UUserWidget* enemyHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Things")
		TSubclassOf<UUserWidget> pauseWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD Things")
		UUserWidget* pauseMenu;

	bool canShowEnemyHPBar;

	void ShowEnemyHPBar();
	void HideEnemyHPBar();
	void TogglePauseMenu();


	FVector enemyPosition;


	bool isPauseMenuVisible;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ShowPauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void HidePauseMenu();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
