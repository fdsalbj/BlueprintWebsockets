// Copyright 2022 Chris Ringenberg https://www.ringenberg.dev/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Websocket.generated.h"

class IWebSocket;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWebSocketConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWebSocketConnectionError, const FString&, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWebSocketClosed, int32, StatusCode, const FString&, Reason, bool, bWasClean);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWebSocketMessageReceived, const FString&,  Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWebSocketBinaryDataReceived, const TArray<uint8>&, Data, bool, bIsLastFragment);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWebSocketMessageSent, const FString&, Message);

UCLASS(MinimalAPI, BlueprintType)
class UWebSocket final : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnWebSocketConnected OnWebSocketConnected;

	UPROPERTY(BlueprintAssignable)
	FOnWebSocketConnectionError OnWebSocketConnectionError;

	UPROPERTY(BlueprintAssignable)
	FOnWebSocketClosed OnWebSocketClosed;

	UPROPERTY(BlueprintAssignable)
	FOnWebSocketMessageReceived OnWebSocketMessageReceived;
	
	UPROPERTY(BlueprintAssignable)
	FWebSocketBinaryDataReceived OnWebSocketBinaryDataReceived;

	UPROPERTY(BlueprintAssignable)
	FOnWebSocketMessageSent OnWebSocketMessageSent;

	void InitWebSocket(TSharedPtr<IWebSocket> InWebSocket);

	UFUNCTION(BlueprintCallable, Category = "Easy WebSockets")
	void Connect();

	UFUNCTION(BlueprintCallable, Category = "Easy WebSockets")
	void Close(int32 StatusCode = 1000, const FString& Reason = TEXT(""));

	UFUNCTION(BlueprintPure, Category = "Easy WebSockets")
	bool IsConnected() const;

	UFUNCTION(BlueprintCallable, Category = "Easy WebSockets")
	void SendMessage(const FString& Message);
	
	UFUNCTION(BlueprintCallable, Category = "Easy WebSockets")
	bool SendBinary(const TArray<uint8>& Data);

private:

	UFUNCTION()
	void OnWebSocketConnected_Internal();

	UFUNCTION()
	void OnWebSocketConnectionError_Internal(const FString& Error);

	UFUNCTION()
	void OnWebSocketClosed_Internal(int32 StatusCode, const FString& Reason, bool bWasClean);

	UFUNCTION()
	void OnWebSocketMessageReceived_Internal(const FString& Message);
	
	void OnWebSocketBinaryDataReceived_Internal(const void* Data, SIZE_T NumBytes, bool bIsLastFragment);

	UFUNCTION()
	void OnWebSocketMessageSent_Internal(const FString& Message);
	
	TSharedPtr<IWebSocket> InternalWebSocket;
	
};