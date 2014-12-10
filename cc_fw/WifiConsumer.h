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

#ifndef __cc_firmware__WifiConsumer__
#define __cc_firmware__WifiConsumer__

#include <vector>

class WifiConsumer {
	static std::vector<WifiConsumer*> Consumers;
	
public:
	WifiConsumer() { Consumers.emplace_back(this); }
	virtual void run(void) { while(1); /* Error */ };
	virtual void stop(void) { while(1); /* Error */ };
	static void startService(void) { for(WifiConsumer* consumer : Consumers) consumer->run();}
	static void stopService(void) { for(WifiConsumer* consumer : Consumers) consumer->stop();}
};


#endif /* defined(__cc_firmware__WifiConsumer__) */
