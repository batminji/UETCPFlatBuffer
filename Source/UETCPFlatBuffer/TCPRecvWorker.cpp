// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPRecvWorker.h"
#include "SocketSubsystem.h"

FTCPRecvWorker::FTCPRecvWorker()
{
}

uint32 FTCPRecvWorker::Run()
{
	while (true)
	{
		if (!ServerSocket)
		{
			return;
		}

		uint32 Pending = 0;
		if (!ServerSocket->HasPendingData(Pending))
		{
			return;
		}

		// Header
		uint16 NetPacketSize = 0;
		uint16 PacketSize = 0;
		int32 TotalRecvBytes = 0;
		int32 RecvBytes = 0;
		while (TotalRecvBytes < (int32)sizeof(NetPacketSize))
		{
			if (!ServerSocket->Recv((uint8*)&NetPacketSize + TotalRecvBytes, sizeof(NetPacketSize) - TotalRecvBytes, RecvBytes) || RecvBytes == 0)
			{
				Disconnect();
				break;
			}
			TotalRecvBytes += RecvBytes;
		}

		PacketSize = BYTESWAP_ORDER16(NetPacketSize);

		// Body
		RecvBuffer.SetNumUninitialized(PacketSize);
		TotalRecvBytes = 0;
		RecvBytes = 0;
		while (TotalRecvBytes < (int32)(PacketSize))
		{
			if (!ServerSocket->Recv(RecvBuffer.GetData() + TotalRecvBytes, PacketSize - TotalRecvBytes, RecvBytes) || RecvBytes == 0)
			{
				Disconnect();
				break;
			}
			TotalRecvBytes += RecvBytes;
		}

		DispatchPacket();

		RecvBuffer.Reset();

	}
	return uint32();
}

void FTCPRecvWorker::Stop()
{
}
