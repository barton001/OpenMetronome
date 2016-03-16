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
// FileVersion.h: interface for the NFileVersion class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEVERSION_H__ECE2D0C3_545E_4141_BAF4_1BF299E2BF69__INCLUDED_)
#define AFX_FILEVERSION_H__ECE2D0C3_545E_4141_BAF4_1BF299E2BF69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Reads the windows VS_VERSION_INFO resource associated with the application (e.g., what
//you can see when you right-click->properties a Win32 or MFC app; doesn't seem to work
//for console apps).  The FILEVERSION is the field read.
//These version labels are of the form of, e.g., 1,0,0,1
//Setting the last number to non-zero designates the app an "uncontrolled prototype" ("u#" suffix)
//Setting the penultimate number designates the app:
//  1: "Alpha" release ("A#" suffix)
//  2: "Beta"  release ("B#" suffix)
//  3: "Release Candidate"  release ("rc#" suffix)
//Setting the last 2 numbers to zero designates a "proper" controlled release, in which the
//first two numbers are used to populate the version label as "first number.second number".
//Where the last number is non-zero, it is appended to the version label to allow different
//beta (etc) versions to be differentiated.
//Link against version.lib
namespace cBscFileVersion  
{
    //See main namespace comment for details; a null filename will ensure the current app
    //or module name is used.
    void Get(TCHAR * pBuf, LPCTSTR strFilename = NULL);
};

#endif // !defined(AFX_FILEVERSION_H__ECE2D0C3_545E_4141_BAF4_1BF299E2BF69__INCLUDED_)
