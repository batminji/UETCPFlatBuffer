// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TCPPlayerController.generated.h"

class UTCPClientSubsystem;
class ULoginWidget;

UCLASS()
class UETCPFLATBUFFER_API ATCPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<ULoginWidget> LoginWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "TCP")
	UTCPClientSubsystem* GetTCPClientSubsystem() const;

	UFUNCTION()
	void HandleConnected();

	UFUNCTION()
	void HandleDisconnected();

private:
	UPROPERTY()
	TObjectPtr<ULoginWidget> LoginWidget;
};
