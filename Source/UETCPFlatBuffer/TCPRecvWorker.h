// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"

class UETCPFLATBUFFER_API FTCPRecvWorker : public FRunnable
{
public:
	FTCPRecvWorker();

	virtual uint32 Run() override;

	virtual void Stop() override;

	TArray<uint8> RecvBuffer;

	FSocket* ServerSocket = nullptr;
};
