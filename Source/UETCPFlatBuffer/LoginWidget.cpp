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

    UTCPClientSubsystem* TCPClientSubsystem = GetTCPClientSubsystem();
    
}

void ULoginWidget::OnLoginButtonClicked()
{
    UTCPClientSubsystem* TCPClientSubsystem = GetTCPClientSubsystem();

    if (TCPClientSubsystem)
    {
		FString UserID = UserIDTextBox->GetText().ToString();
        FString Password = PasswordTextBox->GetText().ToString();

		TCPClientSubsystem->SendLogin(UserID, Password);
    }
}

void ULoginWidget::OnRegisterButtonClicked()
{
    UTCPClientSubsystem* TCPClientSubsystem = GetTCPClientSubsystem();

    if (TCPClientSubsystem)
    {
        FString UserID = UserIDTextBox->GetText().ToString();
        FString Password = PasswordTextBox->GetText().ToString();
		FString Username = UsernameTextBox->GetText().ToString();

		TCPClientSubsystem->SendRegister(UserID, Password, Username);
    }
}

void ULoginWidget::OnLoginResult(bool bSuccess)
{
}

void ULoginWidget::OnRegisterResult(bool bSuccess)
{
}

UTCPClientSubsystem* ULoginWidget::GetTCPClientSubsystem() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		return GameInstance->GetSubsystem<UTCPClientSubsystem>();
	}

	return nullptr;
}
