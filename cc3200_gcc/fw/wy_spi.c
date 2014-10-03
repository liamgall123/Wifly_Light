/**
 Copyright (C) 2014 Nils Weiss, Patrick Br�nn.

 This file is part of WyLight.

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

#include "../../firmware/spi.h"
#include "../driverlib/spi.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"

#define SPI_IF_BIT_RATE  100000

void SPI_Init() {
	/*MAP_PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK);
	MAP_PRCMPeripheralReset(PRCM_GSPI);

	MAP_SPIReset(GSPI_BASE);

	MAP_SPIConfigSetExpClk(GSPI_BASE, MAP_PRCMPeripheralClockGet(PRCM_GSPI), SPI_IF_BIT_RATE, SPI_MODE_MASTER,
	SPI_SUB_MODE_1, (SPI_SW_CTRL_CS |
	SPI_3PIN_MODE |
	SPI_TURBO_OFF |
	SPI_CS_ACTIVELOW |
	SPI_WL_8));

	MAP_SPIFIFOEnable(GSPI_BASE, SPI_TX_FIFO);

	MAP_SPIEnable(GSPI_BASE);*/
}

uns8 SPI_Send(const uns8 data) {
	//MAP_SPIDataPutNonBlocking(GSPI_BASE, data);
	return data;
}

void SPI_SendLedBuffer(uns8 *array) {
	const uns8 *end = (uns8 *) (array + (NUM_OF_LED * 3)); /* array must be the address of the first byte*/
	/* calculate where the end is */
	for (; array < end; array++) { /* send all data */
		SPI_Send(*array);
	}

	/* If we really have to garantee a sleep after data was written to the LEDs, it should be added here.
	 * Other locations would be more attractive to avoid a waiting core, but here it is much clearer and easier
	 * to find for later optimization. In my opinion we should spend this 1ms waste here, before we make the main
	 * loop more complex. */
}
