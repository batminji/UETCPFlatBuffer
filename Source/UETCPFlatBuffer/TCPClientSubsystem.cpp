// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPClientSubsystem.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"

#include "UserPacket_generated.h"

bool UTCPClientSubsystem::Connect(const FString& Host, int32 Port)
{
	return false;
}

void UTCPClientSubsystem::Disconnect()
{
}

bool UTCPClientSubsystem::IsConnected() const
{
	return false;
}

void UTCPClientSubsystem::RecvAll()
{
}

bool UTCPClientSubsystem::SendAll(const uint8* Body, uint32 BodyLength)
{
	return false;
}
