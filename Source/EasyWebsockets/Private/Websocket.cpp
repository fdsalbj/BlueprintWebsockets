// Copyright 2022 Chris Ringenberg https://www.ringenberg.dev/

#include "Websocket.h"
#include "IWebSocket.h"

void UWebSocket::InitWebSocket(TSharedPtr<IWebSocket> InWebSocket)
{
	InternalWebSocket = InWebSocket;

	InternalWebSocket->OnConnected().AddUObject(this, &ThisClass::OnWebSocketConnected_Internal);
	InternalWebSocket->OnConnectionError().AddUObject(this, &ThisClass::OnWebSocketConnectionError_Internal);
	InternalWebSocket->OnClosed().AddUObject(this, &ThisClass::OnWebSocketClosed_Internal);
	InternalWebSocket->OnMessage().AddUObject(this, &ThisClass::OnWebSocketMessageReceived_Internal);
	InternalWebSocket->OnBinaryMessage().AddUObject(this, &ThisClass::OnWebSocketBinaryDataReceived_Internal);
	InternalWebSocket->OnMessageSent().AddUObject(this, &ThisClass::OnWebSocketMessageSent_Internal);
}

void UWebSocket::Connect()
{
	InternalWebSocket->Connect();
}

void UWebSocket::Close(int32 StatusCode, const FString& Reason)
{
	InternalWebSocket->Close(StatusCode, Reason);
}

bool UWebSocket::IsConnected() const
{
	return InternalWebSocket->IsConnected();
}

void UWebSocket::SendMessage(const FString& Message)
{
	InternalWebSocket->Send(Message);
}

bool UWebSocket::SendBinary(const TArray<uint8>& Data)
{
#if WITH_WEBSOCKETS
	if (!InternalWebSocket.IsValid() || !InternalWebSocket->IsConnected())
	{
		return false;
	}

	if (Data.Num() == 0)
	{
		return false;
	}

	InternalWebSocket->Send(Data.GetData(), Data.Num(), /*bIsBinary=*/true);
	return true;
#else
	return false;
#endif
}

void UWebSocket::OnWebSocketConnected_Internal()
{
	OnWebSocketConnected.Broadcast();
}

void UWebSocket::OnWebSocketConnectionError_Internal(const FString& Error)
{
	OnWebSocketConnectionError.Broadcast(Error);
}

void UWebSocket::OnWebSocketClosed_Internal(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	OnWebSocketClosed.Broadcast(StatusCode, Reason, bWasClean);
}

void UWebSocket::OnWebSocketMessageReceived_Internal(const FString& Message)
{
	OnWebSocketMessageReceived.Broadcast(Message);
}

void UWebSocket::OnWebSocketBinaryDataReceived_Internal(const void* Data, SIZE_T NumBytes, bool bIsLastFragment)
{
	TArray<uint8> BinaryData;
	BinaryData.Append(static_cast<const uint8*>(Data), NumBytes);
	OnWebSocketBinaryDataReceived.Broadcast(BinaryData, bIsLastFragment);
}


void UWebSocket::OnWebSocketMessageSent_Internal(const FString& Message)
{
	OnWebSocketMessageSent.Broadcast(Message);
}
