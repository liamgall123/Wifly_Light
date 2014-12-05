/*
 Copyright (C) 2012 - 2014 Nils Weiss, Patrick Bruenn.

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

#ifndef _WIFI_H_
#define _WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "osi.h"

#define ATTEMPTING_TO_CONNECT_TO_AP			"Attempting to auto connect to AP\r\n"
#define NOT_CONNECTED_TO_AP					"Not connected to AP\r\n"

extern OsiSyncObj_t WlanSupportProvisioningDataAddedSemaphore;
extern OsiTaskHandle WlanSupportTaskHandle;

void WlanSupport_TaskInit(void);

void WlanSupport_Task(void *pvParameters);
	
#ifdef __cplusplus
}
#endif

#endif /* _WIFI_H_ */


