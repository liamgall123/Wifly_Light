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

#ifndef _SCRIPTCTRL_H_
#define _SCRIPTCTRL_H_

#include "wifly_cmd.h"

#define SCRIPTCTRL_NUM_CMD_MAX 15
#define SCRIPTCTRL_LOOP_DEPTH_MAX 4

#define ScriptBufInc(x) (((x) + 1) & SCRIPTCTRL_NUM_CMD_MAX)

struct ScriptBuf {
	struct led_cmd cmd[SCRIPTCTRL_NUM_CMD_MAX];
	uns8 loopStart[SCRIPTCTRL_LOOP_DEPTH_MAX];
	uns8 loopDepth;
	uns8 execute;
	uns8 read;
	uns8 write;
	bit override;
	bit loopSkip;
};

void ScriptCtrl_Add(struct led_cmd* pCmd);
void ScriptCtrl_Run(void);
void ScriptCtrl_Write(struct led_cmd* pCmd);

#endif /* #ifndef _SCRIPTCTRL_H_ */
