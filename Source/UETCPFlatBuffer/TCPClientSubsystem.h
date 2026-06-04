// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TCPClientSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTCPConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTCPDisconnected);

class FSocket;

UCLASS()
class UETCPFLATBUFFER_API UTCPClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TCPClient")
	bool Connect(const FString& Host, int32 Port);
	
	UFUNCTION(BlueprintCallable, Category = "TCPClient")
	void Disconnect();

	UFUNCTION(BlueprintCallable, Category = "TCPClient")
	bool IsConnected() const;

	UPROPERTY(BlueprintAssignable, Category = "TCPClient")
	FOnTCPConnected OnTCPConnected;

	UPROPERTY(BlueprintAssignable, Category = "TCPClient")
	FOnTCPDisconnected OnTCPDisconnected;


private:
	TArray<uint8> RecvBuffer;

	void RecvAll();

	bool SendAll(const uint8* Body, uint32 BodyLength);

	FSocket* ServerSocket = nullptr;
};
