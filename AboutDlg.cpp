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
#include "stdafx.h"
#include "ShellAPI.h" //ShellExecute, requires shell32.lib
#include "cBscFileVersion.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//--------------------------------------------------------------------------------------------------
CAboutDlg::CAboutDlg(HINSTANCE hInstance, HWND hParent) :
    CBscBblDlg(IDD_ABOUTBOX, hInstance, hParent, false, true)
{
    ADD_CMD_HANDLER(IDC_PINKANDAINT_URL, OnPinkandaintUrl  );
    ADD_MSG_HANDLER(WM_CTLCOLORSTATIC  , OnCtlColor        );

    std::basic_string<TCHAR> buf(WMET_APP_TITLE _T("              "));
    cBscFileVersion::Get((TCHAR*)(buf.c_str()+_tcslen(WMET_APP_TITLE)));
    m_program_title = buf.c_str();
}
//--------------------------------------------------------------------------------------------------

    
long CAboutDlg::OnInitDialog()
{
    SetWindowText(GET_HWND(IDC_PROGRAM_TITLE), m_program_title.c_str());
    return 0;
}
//--------------------------------------------------------------------------------------------------


long CAboutDlg::OnPinkandaintUrl(unsigned long const nIgnore1, long const nIgnore2) 
{
	// Learned to do this from http://www.thescarms.com/VBasic/ShellExecute.asp (8/26/2003)
	// Open the Weird Metronome web page
    
    //Requires shell32.lib
#ifndef UNDER_CE //No about box in CE!
	ShellExecute(this->m_hWnd, _T("open"), _T("http://openmetronome.sourceforge.net"), _T(""), 0, SW_SHOWNORMAL);
#endif
    return 1;
}
//--------------------------------------------------------------------------------------------------


// Here's a function to color the Pinkandaint URL blue so the user  knows that it's clickable.
long CAboutDlg::OnCtlColor(unsigned long const hdcStatic, long const hwndStatic) 
{
    long lRet = 0;//Indicate the caller that we're done and it should do default action
	
	if (GET_HWND(IDC_PINKANDAINT_URL) == (HWND)hwndStatic)
    {
         SetTextColor((HDC)hdcStatic, GetSysColor(COLOR_ACTIVECAPTION));
         SetBkColor((HDC)hdcStatic, GetSysColor(COLOR_BTNFACE));
         lRet = (long)GetSysColorBrush(COLOR_BTNFACE);
    }

    return lRet;
}
//--------------------------------------------------------------------------------------------------
