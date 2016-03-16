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
// EditboxCatchKeypress.cpp : implementation file
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EditboxCatchKeypress.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//--------------------------------------------------------------------------------------------------
CEditboxCatchKeypress::CEditboxCatchKeypress(int const HotkeyID, unsigned long const Flag, unsigned long const VKey, HWND hWnd, bool const bDeferCreation) :
    m_HotkeyID(HotkeyID), 
    m_VKey(VKey),
    m_Flag(Flag),
    CBscSubclassWnd(hWnd, true)
{
    ADD_MSG_HANDLER_SC(WM_KEYDOWN, OnKeydown);
    //ADD_MSG_HANDLER_SC(WM_SYSCHAR, OnChar);
    ADD_MSG_HANDLER_SC(WM_SYSKEYDOWN, OnKeydown);

    if (!bDeferCreation)
        CreateIt();

    UpdateWindowText();
}
//--------------------------------------------------------------------------------------------------


CEditboxCatchKeypress::~CEditboxCatchKeypress()
{
}
//--------------------------------------------------------------------------------------------------


long CEditboxCatchKeypress::OnKeydown(unsigned long const nVirtKey, long const lKeyData)
{
    bool const bALT   = ((lKeyData & (1<<29)) != 0);
    bool const bCTRL  = ((GetKeyState(VK_CONTROL) & 0xFF00) != 0);
    bool const bSHIFT = ((GetKeyState(VK_SHIFT  ) & 0xFF00) != 0);

	TCHAR const tcBufKey[] = {((TCHAR const) MapVirtualKeyA(nVirtKey, 2)), 0 };

    if (tcBufKey[0])
    {
        m_VKey = nVirtKey;
        m_Flag = (bALT?MOD_ALT:0)|(bSHIFT?MOD_SHIFT:0)|(bCTRL?MOD_CONTROL:0);
        UpdateWindowText();
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------


void CEditboxCatchKeypress::UpdateWindowText()
{
    TCHAR const tcBufKey[] = {((TCHAR const)MapVirtualKey(m_VKey, 2)), 0};

    TCHAR tcBuf[] = _T("SHIFT-CTRL-ALT-C "); //Init to this to get the length right

    tcBuf[0] = 0; //Now set length 0

    if (m_Flag&MOD_CONTROL)
        _tcscat(tcBuf, _T("CTRL-"));
    if (m_Flag&MOD_ALT)
        _tcscat(tcBuf, _T("ALT-"));
    if (m_Flag&MOD_SHIFT)
        _tcscat(tcBuf, _T("SHIFT-"));

    switch (tcBufKey[0])
    {
    case _T('\b'): {_tcscat(tcBuf, _T("<BACKSPACE>")); break;}
    case _T(' '): {_tcscat(tcBuf, _T("<SPACE>")); break;}
    default     : {_tcscat(tcBuf, tcBufKey     ); break;}
    }
    
    if (tcBufKey[0])
        SetWindowText(m_hWnd, tcBuf);
}
//--------------------------------------------------------------------------------------------------


void CEditboxCatchKeypress::Clear()
{
    SetWindowText(m_hWnd, _T("")); 
    m_VKey = 0; //MSDN (VC++ v6.0): "The key code values 0 and 0FFh are not used."
    m_Flag = 0;
}
//--------------------------------------------------------------------------------------------------
