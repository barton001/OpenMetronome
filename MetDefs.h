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
#if !defined(AFX_METDEFS_H__B2991204_D4EE_11D7_92BD_444553540000__INCLUDED_)
#define AFX_METDEFS_H__B2991204_D4EE_11D7_92BD_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SOUNDS    9    /* this can't be any more than 9*/
#define MAX_GROUP_SUBSTRING MAX_SOUNDS
#define MAX_BPMEASURE 1000 /* this is an arbitrarily chosen number: 
                            I've overridden it for custom beats now, so have as many as you want!
                            It is still used for the max spin value for simple measure*/

#endif // !defined(AFX_METDEFS_H__B2991204_D4EE_11D7_92BD_444553540000__INCLUDED_)
