// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPTestActor.h"
#include "TCPClientSubsystem.h"

// Sets default values
ATCPTestActor::ATCPTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATCPTestActor::BeginPlay()
{
	Super::BeginPlay();
	
	UTCPClientSubsystem* TCPClientSubsystem = GetTCPClientSubsystem();

	if (TCPClientSubsystem)
	{
		TCPClientSubsystem->OnTCPConnected.AddDynamic(this, &ATCPTestActor::HandleConnected);
		TCPClientSubsystem->OnTCPDisconnected.AddDynamic(this, &ATCPTestActor::HandleDisconnected);

		TCPClientSubsystem->Connect(TEXT("127.0.0.1"), 35000);
	}
}

void ATCPTestActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UTCPClientSubsystem* TCPClientSubsystem = GetTCPClientSubsystem();
	if(TCPClientSubsystem)
	{
		TCPClientSubsystem->OnTCPConnected.RemoveDynamic(this, &ATCPTestActor::HandleConnected);
		TCPClientSubsystem->OnTCPDisconnected.RemoveDynamic(this, &ATCPTestActor::HandleDisconnected);
		TCPClientSubsystem->Disconnect();
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ATCPTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UTCPClientSubsystem* ATCPTestActor::GetTCPClientSubsystem() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		return GameInstance->GetSubsystem<UTCPClientSubsystem>();
	}

	return nullptr;
}

void ATCPTestActor::HandleConnected()
{

}

void ATCPTestActor::HandleDisconnected()
{

}

