// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPPlayerController.h"
#include "TCPClientSubsystem.h"
#include "LoginWidget.h"

void ATCPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		ShowLoginWidget();
	}

	UTCPClientSubsystem* TCPClientSubsystem = GetTCPClientSubsystem();
	if (TCPClientSubsystem)
	{
		TCPClientSubsystem->OnTCPConnected.AddDynamic(this, &ATCPPlayerController::HandleConnected);
		TCPClientSubsystem->OnTCPDisconnected.AddDynamic(this, &ATCPPlayerController::HandleDisconnected);

		TCPClientSubsystem->Connect(TEXT("127.0.0.1"), 35000);
	}
}

UTCPClientSubsystem* ATCPPlayerController::GetTCPClientSubsystem() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		return GameInstance->GetSubsystem<UTCPClientSubsystem>();
	}

	return nullptr;
}

void ATCPPlayerController::HandleConnected()
{
    UE_LOG(LogTemp, Warning, TEXT("Connected!"));
}

void ATCPPlayerController::HandleDisconnected()
{
    UE_LOG(LogTemp, Warning, TEXT("Disconnected!"));
}

void ATCPPlayerController::ShowLoginWidget()
{
	if (LoginWidgetClass)
	{
		LoginWidget = CreateWidget<ULoginWidget>(this, LoginWidgetClass);
		if (LoginWidget)
		{
			LoginWidget->AddToViewport();
		}
	}

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

