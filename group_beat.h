//Open Metronome: Copyright 2004 David Johnston, 2009 Mark Billington.

//This file is part of "Open Metronome".
//
//"Open Metronome" is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//"Open Metronome" is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with "Open Metronome".  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_GROUP_BEAT_H__74EAD661_E563_11D8_92BE_444553540000__INCLUDED_)
#define AFX_GROUP_BEAT_H__74EAD661_E563_11D8_92BE_444553540000__INCLUDED_
// group_beat.h: interface for the group_beat class.
//////////////////////////////////////////////////////////////////////


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MetDefs.h"

class group_beat
{
private:
	bool single;
	int single_value;
	int multiple_length;
	int multiple_value[MAX_GROUP_SUBSTRING];;
public:
    group_beat();
	virtual ~group_beat();

	void set_single_value(const int val)
	{
		single_value = val;
		single = true;
	}

	bool set_multiple_value(const TCHAR * const multi);

	int get_single_value() const
	{
		////ASSERT(single);
		return single_value;
	}

	int get_multiple_value(const int index) const
	{
		//ASSERT(!single);
		//ASSERT(index < multiple_length);

		return multiple_value[index];
	}

	int get_multiple_length() const
	{
		//ASSERT(!single);
		return multiple_length;
	}

	bool is_single_value() const
	{
		return single;
	}

};

#endif // !defined(AFX_GROUP_BEAT_H__74EAD661_E563_11D8_92BE_444553540000__INCLUDED_)
