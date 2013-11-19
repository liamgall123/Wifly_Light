/*
 Copyright (C) 2013 Nils Weiss, Patrick Bruenn.
 
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

#ifndef __WyLight__Script__
#define __WyLight__Script__

#include "FwCommand.h"

#include <list>
#include <string>
#include <memory>

namespace WyLight {

class Script
{
	std::string mName;
	std::list<FwCmdScript*> mList;

public:
	static void deserialize(const std::string& filename, Script& newScript) throw (FatalError);
	static void serialize(const std::string& filename, const Script& newScript) throw (FatalError);

	Script();
	Script(const std::string& filename);
	~Script(void);

	bool operator ==(const Script& ref) const;

	std::list<FwCmdScript*>::const_iterator begin() const noexcept;
	void clear();
	std::list<FwCmdScript*>::const_iterator end() const noexcept;
	const std::string& getName() const;
	void push_back(FwCmdScript* pNew);
	size_t size() const;
};
} /* namespace WyLight */
#endif /* #ifndef __WyLight__Script__ */

