// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "TCPClientSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTCPConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTCPDisconnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoginCallback, bool, bSuccess, const FString&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRegisterCallback, bool, bSuccess, const FString&, Message);

UCLASS()
class UETCPFLATBUFFER_API UTCPClientSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

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

	UPROPERTY(BlueprintAssignable, Category = "TCPClient")
	FOnLoginCallback OnLogin;

	UPROPERTY(BlueprintAssignable, Category = "TCPClient")
	FOnRegisterCallback OnRegister;

	void SendLogin(const FString& UserID, const FString& Password);

	void SendRegister(const FString& UserID, const FString& Password, const FString& Username);

private:
	TArray<uint8> RecvBuffer;

	void RecvAll();

	bool SendAll(const uint8* Body, uint32 BodyLength);

	FSocket* ServerSocket = nullptr;

	void DispatchPacket();

	virtual void Tick(float DeltaTime) override;

	TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UTCPClientSubsystem, STATGROUP_Tickables);
	}
};
