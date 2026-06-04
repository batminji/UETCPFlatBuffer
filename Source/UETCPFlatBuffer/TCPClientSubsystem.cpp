// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPClientSubsystem.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"

#include "UserPacket_generated.h"

bool UTCPClientSubsystem::Connect(const FString& Host, int32 Port)
{
	// Socket
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	// Get Host Address
	FAddressInfoResult AddressResult = SocketSubsystem->GetAddressInfo(*Host, nullptr, EAddressInfoFlags::Default, ESocketProtocolFamily::IPv4);

	// Socket Address
	TSharedPtr<FInternetAddr> ServerAddr = AddressResult.Results[0].Address;

	// Port
	ServerAddr->SetPort(Port);

	// Socket()
	ServerSocket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("TCPClientSocket"), ServerAddr->GetProtocolType());

	// Connect
	if(!ServerSocket->Connect(*ServerAddr))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to connect to server"));
		return false;
	}
	RecvBuffer.Reset();

	OnTCPConnected.Broadcast();

	return true;
}

void UTCPClientSubsystem::Disconnect()
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	// Close Socket
	if(ServerSocket)
	{
		ServerSocket->Close();
		SocketSubsystem->DestroySocket(ServerSocket);
		ServerSocket = nullptr;
	}

	OnTCPDisconnected.Broadcast();
}

bool UTCPClientSubsystem::IsConnected() const
{
	return ServerSocket != nullptr && ServerSocket->GetConnectionState() == SCS_Connected;
}

void UTCPClientSubsystem::RecvAll()
{
	if (!ServerSocket)
	{
		return;
	}

	uint32 Pending = 0;
	uint16 PacketSize = 0;
	int32 RecvBytes = 0;
	while (ServerSocket->HasPendingData(Pending))
	{
		if(!ServerSocket->Recv((uint8*)&PacketSize, sizeof(PacketSize), RecvBytes) || RecvBytes == 0)
		{
			Disconnect();
			break;
		}

		if(RecvBytes == sizeof(PacketSize))
		{
			break;
		}
	}

	while (ServerSocket->HasPendingData(Pending))
	{
		if (!ServerSocket->Recv(RecvBuffer.GetData(), PacketSize, RecvBytes) || RecvBytes == 0)
		{
			Disconnect();
			break;
		}

		if (RecvBytes == PacketSize)
		{
			break;
		}
	}
	
	if (RecvBytes > 0)
	{
		RecvBuffer.SetNum(RecvBytes);

		DispatchPacket();
	}
}

bool UTCPClientSubsystem::SendAll(const uint8* Body, uint32 BodyLength)
{
	return false;
}

void UTCPClientSubsystem::DispatchPacket()
{
	// flat buffer -> extract
}


void UTCPClientSubsystem::Tick(float DeltaTime)
{
	RecvAll();
}