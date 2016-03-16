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
// CBscToolTip.cpp: implementation of the CBscToolTip class.
//
// Author: M.T.Billington (24-Jun-2004)
//
// I've pretty much block-copied this from:
// //http://msdn2.microsoft.com/en-us/library/bb760252(VS.85).aspx
// is why it isn't so nice sylistically:) (so many #defines!! Mainly required for WinCE, I think)
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BscToolTip.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifndef TOOLTIPS_CLASS
#define TOOLTIPS_CLASSW         L"tooltips_class32"
#define TOOLTIPS_CLASSA         "tooltips_class32"
#define TTM_SETMAXTIPWIDTH      (WM_USER + 24)

#ifdef UNICODE
#define TOOLTIPS_CLASS          TOOLTIPS_CLASSW
#else
#define TOOLTIPS_CLASS          TOOLTIPS_CLASSA
#endif
#define TTS_ALWAYSTIP           0x01
#if (_WIN32_IE >= 0x0300)
#define LPTOOLINFOA   LPTTTOOLINFOA
#define LPTOOLINFOW   LPTTTOOLINFOW
#define TOOLINFOA       TTTOOLINFOA
#define TOOLINFOW       TTTOOLINFOW
#else
#define   TTTOOLINFOA   TOOLINFOA
#define LPTTTOOLINFOA LPTOOLINFOA
#define   TTTOOLINFOW   TOOLINFOW
#define LPTTTOOLINFOW LPTOOLINFOW
#endif

#define LPTOOLINFO    LPTTTOOLINFO
#define TOOLINFO        TTTOOLINFO

#define TTTOOLINFOA_V1_SIZE CCSIZEOF_STRUCT(TTTOOLINFOA, lpszText)
#define TTTOOLINFOW_V1_SIZE CCSIZEOF_STRUCT(TTTOOLINFOW, lpszText)
#define TTF_IDISHWND            0x0001
#define TTF_SUBCLASS            0x0010
#define TTM_ADDTOOLA            (WM_USER + 4)
#define TTM_ADDTOOLW            (WM_USER + 50)
#ifdef UNICODE
#define TTM_ADDTOOL             TTM_ADDTOOLW
#else
#define TTM_ADDTOOL             TTM_ADDTOOLA
#endif
typedef struct tagTOOLINFOA {
    UINT cbSize;
    UINT uFlags;
    HWND hwnd;
    UINT uId;
    RECT rect;
    HINSTANCE hinst;
    LPSTR lpszText;
#if (_WIN32_IE >= 0x0300)
    LPARAM lParam;
#endif
} TTTOOLINFOA, NEAR *PTOOLINFOA, FAR *LPTTTOOLINFOA;

typedef struct tagTOOLINFOW {
    UINT cbSize;
    UINT uFlags;
    HWND hwnd;
    UINT uId;
    RECT rect;
    HINSTANCE hinst;
    LPWSTR lpszText;
#if (_WIN32_IE >= 0x0300)
    LPARAM lParam;
#endif
} TTTOOLINFOW, NEAR *PTOOLINFOW, FAR* LPTTTOOLINFOW;

#ifdef UNICODE
#define TTTOOLINFO              TTTOOLINFOW
#define PTOOLINFO               PTOOLINFOW
#define LPTTTOOLINFO            LPTTTOOLINFOW
#define TTTOOLINFO_V1_SIZE TTTOOLINFOW_V1_SIZE
#else
#define PTOOLINFO               PTOOLINFOA
#define TTTOOLINFO              TTTOOLINFOA
#define LPTTTOOLINFO            LPTTTOOLINFOA
#define TTTOOLINFO_V1_SIZE TTTOOLINFOA_V1_SIZE
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Construction/ Destruction
//--------------------------------------------------------------------------------------------------
CBscToolTip::CBscToolTip(int const toolID, HWND const hDlg, HINSTANCE const hInstance, 
                         unsigned long const nIDTTip):
    m_hWndTip(NULL)
{   //http://msdn2.microsoft.com/en-us/library/bb760252(VS.85).aspx

    m_strTipText.resize(512+128);
    LoadString(hInstance, nIDTTip, &*m_strTipText.begin(), m_strTipText.length());
    m_strTipText.resize(_tcslen(m_strTipText.c_str()));

    // toolID:  the resource ID of the control.
    // hDlg:    the handle of the dialog box.
    // pText:   the text that appears in the ToolTip.
    // g_hInst: the global instance handle.
    
    if (!toolID || !hDlg || !m_strTipText.length())
        return;

    // Get the window of the tool.
    HWND hwndTool = GetDlgItem(hDlg, toolID);
    
    // Create the ToolTip.
    m_hWndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
                              WS_POPUP |TTS_ALWAYSTIP,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              hDlg, NULL, 
                              hInstance, NULL);

    if (!hwndTool || !m_hWndTip)
        return;
    
    RECT rcCli;
    ::GetClientRect(hDlg, &rcCli);
    SendMessage(m_hWndTip, TTM_SETMAXTIPWIDTH, 0, rcCli.right);

    // Associate the ToolTip with the tool.
    TOOLINFO toolInfo = { 0 };
    toolInfo.cbSize = sizeof(toolInfo);
    toolInfo.hwnd = hDlg;
    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId = (UINT_PTR)hwndTool;
    toolInfo.lpszText = (TCHAR*)m_strTipText.c_str();
    SendMessage(m_hWndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
}
//--------------------------------------------------------------------------------------------------
   

CBscToolTip::~CBscToolTip()
{
    if (m_hWndTip)
        DestroyWindow(m_hWndTip);
}
//--------------------------------------------------------------------------------------------------
