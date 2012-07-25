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

#include "RingBuf.h"

bank1 struct RingBuffer g_RingBuf;

void RingBuf_Init(void)
{
	g_RingBuf.read = 0;
	g_RingBuf.write = 0;
	g_RingBuf.error_full = 0;
}

char RingBuf_Get(void)
{
	char result = g_RingBuf.data[g_RingBuf.read];
	g_RingBuf.read = RingBufInc(g_RingBuf.read);
	return result;
}

void RingBuf_Put(char value)
{
	char writeNext = RingBufInc(g_RingBuf.write);
	if(writeNext != g_RingBuf.read)
	{
		g_RingBuf.data[g_RingBuf.write] = value;
		g_RingBuf.write = writeNext;
	}
	else g_RingBuf.error_full = 1;
}

