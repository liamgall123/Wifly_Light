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

#include <stdio.h>
#include "trace.h"
#define NUM_TEST_LOOPS 255
#define Assert(EXPRESSION) if(!(EXPRESSION)) {errors++; printf("Assert(" #EXPRESSION ") failed\n");}
#define RunTest(FUNC) { int errors= FUNC(); printf(#FUNC"() run with %d errors\n", errors); numErrors+= errors; numTests++;}  
int gSetColorWasCalled;
int gSetFadeWasCalled;

/**************** includes and functions for wrapping ****************/
#include "platform.h"
jmp_buf g_ResetEnvironment;

#include "ScriptCtrl.h"
struct ScriptBuf gScriptBuf;

#include "ledstrip.h"
void Ledstrip_SetColor(struct cmd_set_color *pCmd)
{
	Trace_String("Ledstrip_SetColor was called\n");
	gSetColorWasCalled = TRUE;
}

void Ledstrip_SetFade(struct cmd_set_fade *pCmd)
{
	Trace_String("Ledstrip_SetFade was called\n");
	gSetFadeWasCalled = TRUE;
}

/**************** test functions ****************/
/* add simple command and read back */
int ut_ScriptCtrl_SimpleReadWrite(void)
{
	int errors = 0;
	struct led_cmd testCmd;
	testCmd.cmd = SET_COLOR;
	ScriptCtrl_Init();

	/* read from empty script buffer */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();

	/* add simple command and read back */
	ScriptCtrl_Add(&testCmd);
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	Assert(gSetColorWasCalled);

	/* buffer should be empty again */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	Assert(!gSetColorWasCalled);

	return errors;
}

/* add clear command */
int ut_ScriptCtrl_Clear(void)
{
	int errors = 0;
	struct led_cmd testCmd;
	testCmd.cmd = SET_COLOR;

	/* add dummy command to buffer */
	ScriptCtrl_Add(&testCmd);
	gSetColorWasCalled = FALSE;

	/* send DELETE */
	testCmd.cmd = DELETE;
	ScriptCtrl_Add(&testCmd);

	/* buffer should be empty again */
	ScriptCtrl_Run();
	Assert(!gSetColorWasCalled);

	return errors;
}

/* add simple loop */
int ut_ScriptCtrl_SimpleLoop(void)
{
	int errors = 0;
	struct led_cmd testCmd;

	/* add loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add dummy command to buffer */
	testCmd.cmd = SET_COLOR;
	ScriptCtrl_Add(&testCmd);

	/* add loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = NUM_TEST_LOOPS;
	ScriptCtrl_Add(&testCmd);

	/* start loop should be read */
	Assert(!gScriptBuf.inLoop)
	ScriptCtrl_Run();
	Assert(gScriptBuf.inLoop);

	int i;
	for(i = 0; i < NUM_TEST_LOOPS; i++)
	{
		/* dummy command should be executed */
		gSetColorWasCalled = FALSE;
		ScriptCtrl_Run();
		Assert(gSetColorWasCalled);
		
		/* now next loop should be called */
		gSetColorWasCalled = FALSE;
		ScriptCtrl_Run();
		Assert(!gSetColorWasCalled);
	}

	/* no more command should be available */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	Assert(!gSetColorWasCalled);

	return errors;
}

int ut_ScriptCtrl_DoOuterInnerLoop(int loopCount)
{
	int errors = 0;
	int i, j;
	for(i = 0; i < loopCount; i++)
	{
		/* outer dummy command should be executed */
		gSetColorWasCalled = FALSE;
		ScriptCtrl_Run();
		Assert(gSetColorWasCalled);

		/* start inner loop should be read */
		Assert(gScriptBuf.inLoop)
		ScriptCtrl_Run();
		Assert(gScriptBuf.inLoop);

		for(j = 0; j < loopCount; j++)
		{
			/* outer dummy command should be executed */
			gSetFadeWasCalled = FALSE;
			ScriptCtrl_Run();
			Assert(gSetFadeWasCalled);

			/* now next inner loop should be called */
			gSetColorWasCalled = FALSE;
			ScriptCtrl_Run();
			Assert(!gSetColorWasCalled);
		}

		/* now next outer loop should be called */
		gSetColorWasCalled = FALSE;
		ScriptCtrl_Run();
		Assert(!gSetColorWasCalled);
	}
	return errors;
}


/* add inner loop */
int ut_ScriptCtrl_InnerLoop(void)
{
	int errors = 0;
	struct led_cmd testCmd;

	/* add outer loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add outer dummy command to buffer */
	testCmd.cmd = SET_COLOR;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add inner dummy command to buffer */
	testCmd.cmd = SET_FADE;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = NUM_TEST_LOOPS;
	ScriptCtrl_Add(&testCmd);

	/* add outer loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = NUM_TEST_LOOPS;
	ScriptCtrl_Add(&testCmd);

	/* start outer loop should be read */
	Assert(!gScriptBuf.inLoop)
	ScriptCtrl_Run();
	Assert(gScriptBuf.inLoop);

	errors+= ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);

	/* no more command should be available */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	Assert(!gSetColorWasCalled);

	return errors;
}

/* add infinite loop */
int ut_ScriptCtrl_InfiniteLoop(void)
{
	int errors = 0;
	struct led_cmd testCmd;

	/* add outer loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add outer dummy command to buffer */
	testCmd.cmd = SET_COLOR;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add inner dummy command to buffer */
	testCmd.cmd = SET_FADE;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = NUM_TEST_LOOPS;
	ScriptCtrl_Add(&testCmd);

	/* add outer loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = LOOP_INFINITE;
	ScriptCtrl_Add(&testCmd);

	/* start outer loop should be read */
	Assert(!gScriptBuf.inLoop)
	ScriptCtrl_Run();
	Assert(gScriptBuf.inLoop);

	/* multiple calls should be no problem since we are in an infinite loop */
	errors+= ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);
	errors+= ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);
	errors+= ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);

	/* now terminate the loop */
	testCmd.cmd = DELETE;
	ScriptCtrl_Add(&testCmd);

	/* buffer should be empty again */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	Assert(!gSetColorWasCalled);

	return errors;
}

/* write to full script buffer */
/* write incomplete loop to full buffer */
int ut_ScriptCtrl_FullBuffer(void)
{
	int errors = 0;
	struct led_cmd testCmd;

	/* add outer loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add outer dummy command to buffer */
	testCmd.cmd = SET_COLOR;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	int i;
	for(i = 0; i < SCRIPTCTRL_NUM_CMD_MAX-3; i++)
	{
		/* add inner dummy command to buffer */
		testCmd.cmd = SET_FADE;
		Assert(ScriptCtrl_Add(&testCmd));
	}

	/* Buffer full */
	Assert(!ScriptCtrl_Add(&testCmd));
	return errors;
}

/* add clear command */
int ut_ScriptCtrl_StartBootloader(void)
{
	struct led_cmd testCmd;

	if(0 == softResetJumpDestination())
	{
		/* send START_BL */
		testCmd.cmd = START_BL;
		ScriptCtrl_Add(&testCmd);
		
		/* shouldn't reach this -> return error */
		return 1;
	}
	
	/* we just jumped to softResetJumpDestination() -> it worked */
	return 0;
}

int main(int argc, const char* argv[])
{
	int numErrors;
	int numTests;
	numErrors = 0;
	numTests = 0;
	RunTest(ut_ScriptCtrl_SimpleReadWrite);
	RunTest(ut_ScriptCtrl_Clear);
	RunTest(ut_ScriptCtrl_SimpleLoop);
	RunTest(ut_ScriptCtrl_InnerLoop);
	RunTest(ut_ScriptCtrl_InfiniteLoop);
	RunTest(ut_ScriptCtrl_FullBuffer);
	RunTest(ut_ScriptCtrl_StartBootloader);
	printf("run %d Tests with %d errors\n", numTests, numErrors);
	return numErrors;
}

