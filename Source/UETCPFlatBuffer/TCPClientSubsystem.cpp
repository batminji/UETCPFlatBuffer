// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPClientSubsystem.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"

#include "UserPacket_generated.h"

void UTCPClientSubsystem::Deinitialize()
{
	Disconnect();

	Super::Deinitialize();
}

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

	ServerSocket->SetNonBlocking(false);
 
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

void UTCPClientSubsystem::SendLogin(const FString& UserID, const FString& Password)
{
	flatbuffers::FlatBufferBuilder Builder;

	const FTCHARToUTF8 UserUTF8(UserID);
	const FTCHARToUTF8 PasswordUTF8(Password);

	auto LoginData = UserPacket::CreateC2S_LoginDirect(Builder, UserUTF8.Get(), PasswordUTF8.Get());

	auto PacketData = UserPacket::CreatePacketData(Builder, UserPacket::PacketType_C2S_Login, LoginData.Union());

	Builder.Finish(PacketData);

	SendAll(Builder.GetBufferPointer(), Builder.GetSize());
}

void UTCPClientSubsystem::RecvAll()
{
	if (!ServerSocket)
	{
		return;
	}

	uint32 Pending = 0;
	if (ServerSocket->HasPendingData(Pending))
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
		if(!ServerSocket->Recv((uint8*)&NetPacketSize + TotalRecvBytes, sizeof(NetPacketSize) - TotalRecvBytes, RecvBytes) || RecvBytes == 0)
		{
			Disconnect();
			break;
		}
		TotalRecvBytes += RecvBytes;
	}

	PacketSize = NETWORK_ORDER16(NetPacketSize);

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

bool UTCPClientSubsystem::SendAll(const uint8* Body, uint32 BodyLength)
{
	TArray<uint8> Packet;
	Packet.Reserve(2 + BodyLength);
	Packet.Add((uint8)(BodyLength >> 8) & 0xFF);
	Packet.Add((uint8)(BodyLength & 0xFF));
	Packet.Append(Body, BodyLength);

	int32 SendTotalBytes = 0;
	while(SendTotalBytes < Packet.Num())
	{
		int32 SendBytes = 0;
		if (!ServerSocket->Send(Packet.GetData() + SendTotalBytes, Packet.Num() - SendTotalBytes, SendBytes) || SendBytes < 0)
		{
			return false;
		}
		SendTotalBytes += SendBytes;
	}

	return true;
}

void UTCPClientSubsystem::DispatchPacket()
{
	// flat buffer -> extract

	const auto Packet = UserPacket::GetPacketData(RecvBuffer.GetData());

	switch (Packet->data_type())
	{
	case UserPacket::PacketType_S2C_Login:
	{
		const auto LoginData = Packet->data_as_S2C_Login();

		LoginData->client_socket_id();
		FString Message = UTF8_TO_TCHAR(LoginData->message()->c_str());

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}
	break;
	case UserPacket::PacketType_S2C_Spawn:
	{
	}
	break;
	case UserPacket::PacketType_S2C_Move:
	{
	}
	break;
	case UserPacket::PacketType_S2C_Destroy:
	{
	}
	break;

	case UserPacket::PacketType_S2C_ChangeColor:
	{
	}
	break;

	case UserPacket::PacketType_S2C_Signup:
	{
	}
	break;
	}
}


void UTCPClientSubsystem::Tick(float DeltaTime)
{
	RecvAll();
}