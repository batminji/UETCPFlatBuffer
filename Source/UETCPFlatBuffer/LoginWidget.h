// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTCPClientSubsystem;

UCLASS()
class UETCPFLATBUFFER_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> UsernameTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> PasswordTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> UserIDTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LoginButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RegisterButton;

	UFUNCTION(BlueprintCallable, Category = "Login")
	void OnLoginButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Login")
	void OnRegisterButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "TCP")
	UTCPClientSubsystem* GetTCPClientSubsystem() const;
};
