// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"



void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayWidget) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayWidget);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (enemyHPBarWidget)
	{
		enemyHPBar = CreateWidget<UUserWidget>(this, enemyHPBarWidget);
		if (enemyHPBar)
		{
			enemyHPBar->AddToViewport();

			enemyHPBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D vecDir(0.f, 0.f);
		enemyHPBar->SetAlignmentInViewport(vecDir); //a donde miro
	}

	if (pauseWidget)
	{
		pauseMenu = CreateWidget<UUserWidget>(this, pauseWidget);
		if (pauseMenu)
		{
			pauseMenu->AddToViewport();

			pauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	isPauseMenuVisible = false;
	TogglePauseMenu();

}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (enemyHPBar)
	{
		FVector2D posInCamera;
		ProjectWorldLocationToScreen(enemyPosition, posInCamera);
		posInCamera.Y -= 150.f;

		FVector2D widgetSize(250.f, 25.f);

		enemyHPBar->SetPositionInViewport(posInCamera);
		enemyHPBar->SetDesiredSizeInViewport(widgetSize);
	}
}


void AMainPlayerController::ShowEnemyHPBar()
{
	if (enemyHPBar)
	{
		canShowEnemyHPBar = true;
		enemyHPBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::HideEnemyHPBar()
{
	if (enemyHPBar)
	{
		canShowEnemyHPBar = false;
		enemyHPBar->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AMainPlayerController::ShowPauseMenu_Implementation()
{
	if (pauseMenu)
	{
		isPauseMenuVisible = true;
		pauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI inputMode;

		SetInputMode(inputMode);
		bShowMouseCursor = true;
	}
}
void AMainPlayerController::HidePauseMenu_Implementation()
{
	if (pauseMenu)
	{
		FInputModeGameOnly inputMode;
		bShowMouseCursor = false;
		SetInputMode(inputMode);

		isPauseMenuVisible = false;
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	if (isPauseMenuVisible)
	{
		HidePauseMenu();
	}
	else
	{
		ShowPauseMenu();
	}
}