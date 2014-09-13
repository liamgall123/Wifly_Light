/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

 This file is part of WyLight.

 WyLight is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 WyLight is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICUL<AR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include "string.h"

#include "socket.h"

//WyLight
#include "bootloader.h"
#include "firmware_loader.h"
#include "tcp_server.h"

#define SERVER_PORT htons(2000)

/**
 * @return 0 on success, if new firmware was saved and validated
 */
static int ReceiveFw(int SocketTcpChild)
{
	uint8_t *pFirmware = (uint8_t *) FIRMWARE_ORIGIN;
	UART_PRINT("Start writing Firmware at 0x%x \r\n", pFirmware);

	for(;;) {
		int bytesReceived = recv(SocketTcpChild, pFirmware, BUFFERSIZE, 0);

		if (bytesReceived > 0) {
			// Received some bytes
			pFirmware += bytesReceived;
			UART_PRINT("Tcp: Received %d bytes\r\n", bytesReceived);
			continue;
		}

		if (EAGAIN == bytesReceived) {
			_SlNonOsMainLoopTask();
			continue;
		}

		if (bytesReceived < 0) {
			return bytesReceived;
		}

		const size_t length = (size_t) (pFirmware - FIRMWARE_ORIGIN);
		return SaveSRAMContentAsFirmware((uint8_t *) FIRMWARE_ORIGIN, length);
	}
}

/**
 * Create a new tcp server socket and start listening on it
 * @return positiv socket descriptor, or negative value on error
 */
static int TcpServer_Listen()
{
	static const sockaddr_in localAddr = {
		.sin_family = AF_INET,
		.sin_port = SERVER_PORT,
		.sin_addr.s_addr = 0
	};
	int status;
	
	const int listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket >= 0) {
		const int dontBlock = 1;
		status = setsockopt(listenSocket, SOL_SOCKET, SO_NONBLOCKING, &dontBlock, sizeof(dontBlock));	
		if (status) {
			UART_PRINT("Setsockopt ERROR \r\n");
			goto on_error_close;
		}

		status = bind(listenSocket, (sockaddr *) &localAddr, sizeof(localAddr));
		if (status) {
			UART_PRINT("Bind Error\n\r");
			goto on_error_close;
		}

		const int maxConnections = 1;
		status = listen(listenSocket, maxConnections);
		if (status) {
			UART_PRINT("Listen Error\n\r");
			goto on_error_close;
		}
	}
	return listenSocket;

on_error_close:
	close(listenSocket);
	return status;
}

/**
 * Wait for client to connect
 * NOTE: this function blocks until a good socket is accepted, it retries forever!
 * @return accepted socket descriptor
 */
int TcpServer_Accept(const int listenSocket)
{
	sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	for(;;) {
		const int childSocket = accept(listenSocket, (sockaddr *) &clientAddr, &clientAddrLen);

		if (childSocket >= 0) {
			UART_PRINT("Connected TCP Client\r\n");
			return childSocket;
		}

		if (EAGAIN == childSocket) {
			_SlNonOsMainLoopTask();
		} else {
			UART_PRINT("Error: %d occured on accept\r\n", childSocket);
		}
	}
}

/**
 * @return if new firmware was received and validated
 */
extern void TcpServer(void)
{
	static const uint32_t BL_VERSION = htonl(BOOTLOADER_VERSION);
	char welcome[] = "\0\0\0\0WyLightBootloader";

	memcpy(welcome, &BL_VERSION, sizeof(uint32_t));

	const int listenSocket = TcpServer_Listen();
	if (listenSocket < 0) {
		UART_PRINT("Socket Error: %d \r\n", listenSocket);
		return;
	}

	for(int fwStatus = 0xDEAD; fwStatus;) {
		const int clientSock = TcpServer_Accept(listenSocket);

		if (sizeof(welcome) ==  send(clientSock, welcome, sizeof(welcome), 0)) {
			fwStatus = ReceiveFw(clientSock);
		}
		close(clientSock);
	}
}
