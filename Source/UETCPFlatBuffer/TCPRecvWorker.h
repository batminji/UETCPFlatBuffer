// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Containers/Queue.h"

#include <atomic>

class UETCPFLATBUFFER_API FTCPRecvWorker : public FRunnable
{
public:
	FTCPRecvWorker(FSocket* InServerSocker, TQueue<TArray<uint8>>& InRecvQueue);

	virtual uint32 Run() override;

	virtual void Stop() override;

private:
	TArray<uint8> RecvBuffer;

	FSocket* ServerSocket = nullptr;

	TQueue<TArray<uint8>>& RecvQueue;

	std::atomic<bool> bStopRequested = false;
};
