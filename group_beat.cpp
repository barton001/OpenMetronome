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
// group_beat.cpp: implementation of the group_beat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "metronome.h"
#include "group_beat.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

group_beat::group_beat():
    single(true), single_value(-1), multiple_length(0)
{
    memset(multiple_value, 0, sizeof(multiple_value));
}

group_beat::~group_beat()
{

}

bool group_beat::set_multiple_value(const TCHAR * const multi)
{
	// if there's a non-numeric character in the string or
	// it's longer than the max substring size then 
	// there's a formatting problem, so return failure
	int string_len = _tcsspn(multi, _T("0123456789"));
	if(multi[string_len] != 0 || string_len > MAX_GROUP_SUBSTRING)
		return false;

	// copy the string into the multiple value array
	for(int i = 0; i < string_len; i++)
		multiple_value[i] = multi[i] - _T('1');

	multiple_length = string_len;
	single = false;

	return true;
}

