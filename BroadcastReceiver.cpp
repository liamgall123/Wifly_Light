/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
 
 This file is part of Wifly_Light.
 
 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

#include "BroadcastReceiver.h"

#include <iostream>
#include <stdio.h>

const char BroadcastReceiver::BROADCAST_DEVICE_ID[] = "WiFly";
const size_t BroadcastReceiver::BROADCAST_DEVICE_ID_LENGTH = 5;
const char BroadcastReceiver::STOP_MSG[] = "StopThread";
const size_t BroadcastReceiver::STOP_MSG_LENGTH = sizeof(STOP_MSG);

BroadcastReceiver::BroadcastReceiver(unsigned short port)
	: mPort(port), mMutex(PTHREAD_MUTEX_INITIALIZER)
{
}

BroadcastReceiver::~BroadcastReceiver(void)
{
	//Stop();
	//TODO cleanup mIpTable
}

void BroadcastReceiver::operator() (std::ostream& out, unsigned short timeout)
{
	size_t numRemotes = 0;
	time_t endTime = time(NULL) + timeout;
	do
	{
		uint32_t remote = GetNextRemote();
		out << numRemotes << ':' << std::hex << remote << std::endl;
		numRemotes++;
	} while(time(NULL) < endTime);
}

uint32_t BroadcastReceiver::GetIp(size_t index) const
{
	return mIpTable[index]->m_Addr;
}

uint32_t BroadcastReceiver::GetNextRemote(void)
{
	UdpSocket udpSock(INADDR_ANY, mPort, true, true);
	sockaddr_storage remoteAddr;
	socklen_t remoteAddrLength = sizeof(remoteAddr);
	
	for(;;)
	{
		BroadcastMessage msg;
		size_t bytesRead = udpSock.RecvFrom((unsigned char*)&msg, sizeof(msg), NULL, (sockaddr*)&remoteAddr, &remoteAddrLength);
		if(msg.IsWiflyBroadcast(bytesRead))
		{
			Endpoint* newRemote = new Endpoint(remoteAddr, remoteAddrLength);
			pthread_mutex_lock(&mMutex);
			mIpTable.push_back(newRemote);
			pthread_mutex_unlock(&mMutex);
			return newRemote->m_Addr;
		}
	}
}

uint16_t BroadcastReceiver::GetPort(size_t index) const
{
	return mIpTable[index]->m_Port;
}

size_t BroadcastReceiver::NumRemotes(void) const
{
	return mIpTable.size();
}

void BroadcastReceiver::ShowRemotes(std::ostream& out) const
{
	size_t index = 0;
	for(vector<Endpoint*>::const_iterator it = mIpTable.begin(); it != mIpTable.end(); *it++, index++)
	{
		out << index << ':' << std::hex << (*it)->m_Addr << '\n';
	}
}

void BroadcastReceiver::Stop(void)
{
	UdpSocket sock(INADDR_LOOPBACK, mPort, false);
	sock.Send((unsigned char const*)STOP_MSG, STOP_MSG_LENGTH);
	pthread_join(mThread, NULL);
	return;
}

