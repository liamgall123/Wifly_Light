/**
 Copyright (C) 2012 Nils Weiss, Patrick Brünn.
 
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

#ifndef _ERROR_H_
#define _ERROR_H_
// *** ERRORBITS
struct ErrorBits {
		char crc_failure:1;
		char eeprom_failure:1;
};
extern struct ErrorBits gERROR;

#define ErrorInit(x) \
	gERROR.crc_failure = 0; \
	gERROR.eeprom_failure = 0;

void throw_errors();
#endif /* #ifndef _ERROR_H_ */
