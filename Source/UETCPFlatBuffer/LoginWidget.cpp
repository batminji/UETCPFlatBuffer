// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "TCPClientSubsystem.h"

void ULoginWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LoginButton)
    {
		LoginButton->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginButtonClicked);
    }
    if (RegisterButton)
    {
        RegisterButton->OnClicked.AddDynamic(this, &ULoginWidget::OnRegisterButtonClicked);
    }    
}

void ULoginWidget::OnLoginButtonClicked()
{
	UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
		UTCPClientSubsystem* TCPClientSubsystem = GameInstance->GetSubsystem<UTCPClientSubsystem>();
        if (TCPClientSubsystem)
        {
            TCPClientSubsystem->OnLogin.AddDynamic(this, &ULoginWidget::ProcessLogin);
			TCPClientSubsystem->SendLogin(UserIDTextBox->GetText().ToString(), PasswordTextBox->GetText().ToString());
        }
    }
}

void ULoginWidget::OnRegisterButtonClicked()
{
	UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        UTCPClientSubsystem* TCPClientSubsystem = GameInstance->GetSubsystem<UTCPClientSubsystem>();
        if (TCPClientSubsystem)
        {
            TCPClientSubsystem->OnRegister.AddDynamic(this, &ULoginWidget::ProcessRegister);
			TCPClientSubsystem->SendRegister(UserIDTextBox->GetText().ToString(), PasswordTextBox->GetText().ToString(), UsernameTextBox->GetText().ToString());
        }
    }
}

void ULoginWidget::ProcessLogin(bool bSuccess, const FString& Message)
{
	UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        UTCPClientSubsystem* TCPClientSubsystem = GameInstance->GetSubsystem<UTCPClientSubsystem>();
        if (TCPClientSubsystem)
        {
            TCPClientSubsystem->OnLogin.RemoveDynamic(this, &ULoginWidget::ProcessLogin);
		}
    }
}

void ULoginWidget::ProcessRegister(bool bSuccess, const FString& Message)
{
	UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        UTCPClientSubsystem* TCPClientSubsystem = GameInstance->GetSubsystem<UTCPClientSubsystem>();
        if (TCPClientSubsystem)
        {
            TCPClientSubsystem->OnRegister.RemoveDynamic(this, &ULoginWidget::ProcessRegister);
		}
    }
}
