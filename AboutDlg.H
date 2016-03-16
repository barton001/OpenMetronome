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
#if !defined(AFX_AboutDLG_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
#define AFX_AboutDLG_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"
#include "BscBblDlg.h"


//See also base-class.
//This implements the "about box", and allows the user to click on the WeridMet homepage link
//(which pops up the homepage: neat!)
class CAboutDlg : public CBscBblDlg
{
public:
	CAboutDlg(HINSTANCE hInstance = 0, HWND hParent   = 0);


protected:
    DECLARE_MSG_HANDLER(OnPinkandaintUrl, CAboutDlg); //Launch browser on Pinkandaint URL click
    DECLARE_MSG_HANDLER(OnCtlColor      , CAboutDlg); //color the Pinkandaint URL blue so the user knows that it's clickable.
    virtual long OnInitDialog();


private:
    std::basic_string<TCHAR> m_program_title;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AboutDLG_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
