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
// CBscSubclassWnd.cpp: implementation of the CBscSubclassWnd class.
//
// Author: M.T.Billington (24-Jun-2004)
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "cBscCatch.h" //Open-source-land can't have this:) Also slows down PPCs, apparently
#include "BscSubclassWnd.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef UNDER_CE

//this rubbish is for wince's broken map implementation; it is copied from Win32's non-broken code
/*
_STD_BEGIN

static CRITICAL_SECTION _CritSec;

static long _InitFlag = 0L;

static void _CleanUp()
{
        long InitFlagValue;

        if ( InitFlagValue = InterlockedExchange( &_InitFlag, 3L ) == 2L )
            // Should be okay to delete critical section
            DeleteCriticalSection( &_CritSec );
}

_Lockit::_Lockit()
{

        // Most common case - just enter the critical section

        if ( _InitFlag == 2L ) {
            EnterCriticalSection( &_CritSec );
            return;
        }

        // Critical section either needs to be initialized.

        if ( _InitFlag == 0L ) {

            long InitFlagVal;

            if ( (InitFlagVal = InterlockedExchange( &_InitFlag, 1L )) == 0L ) {
                InitializeCriticalSection( &_CritSec );
                atexit( _CleanUp );
                _InitFlag = 2L;
            }
            else if ( InitFlagVal == 2L )
                _InitFlag = 2L;
        }

        // If necessary, wait while another thread finishes initializing the
        // critical section

        while ( _InitFlag == 1L )
            Sleep( 1 );

        if ( _InitFlag == 2L )
            EnterCriticalSection( &_CritSec );
}

_Lockit::~_Lockit()
{
        if ( _InitFlag == 2L ) 
            LeaveCriticalSection( &_CritSec );
}

_STD_END
*/
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                            Do It!
//--------------------------------------------------------------------------------------------------
CBscSubclassWnd::CBscSubclassWnd(HWND hWnd, bool const bDeferCreation) :
    m_hWnd(hWnd),
    m_lOriginalProc(0)
{
    //cBscTry
    {
        if (!bDeferCreation)
            CreateIt();
    }
    //cBscCatchLogAndRethrow(_T("CBscSubclassWnd(") << hWnd <<  _T(")"));
}
//--------------------------------------------------------------------------------------------------


CBscSubclassWnd::~CBscSubclassWnd()
{
    //cBscTry
    {
        SetWindowLong(m_hWnd, GWL_WNDPROC, m_lOriginalProc);
    }
    //cBscCatchLogAndMsg(_T("~CBscSubclassWnd()"));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                  Helper Functions
//--------------------------------------------------------------------------------------------------

    
void CBscSubclassWnd::AddMsgHandler(unsigned long const Msg, MsgHandler pfnHandleMsg)
{
    //cBscTry
    {
        //Provided this is only called in constructor before the class kicks off I shouldn't need
        //critical sections etc to protect this member
        m_mapMsgHandlers[Msg] = pfnHandleMsg;
    }
    //cBscCatchLogAndRethrow(_T("AddMsgHandler(...)"));//Dont let exception escape destructor
}
//--------------------------------------------------------------------------------------------------


LRESULT WINAPI CBscSubclassWnd::MsgMap_stub(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    long lRet = 0;
    //cBscTry
    {
        CBscSubclassWnd * pThis = reinterpret_cast<CBscSubclassWnd *>(GetWindowLong(hWnd, GWL_USERDATA));
        if (pThis)
            lRet = pThis->MyMsgMap(hWnd, Msg, wParam, lParam);
    }
    //cBscCatchLogAndMsg(_T("MyMsgMap_stub(") << (unsigned long const)hWnd << _T(",") << (unsigned long const)Msg << _T(",") << (unsigned long const)wParam << _T(",") << (unsigned long const)lParam << _T(")"));
    return lRet;
}
//--------------------------------------------------------------------------------------------------


LRESULT CBscSubclassWnd::MyMsgMap(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRet = 0;
    //cBscTry
    {
        if (m_mapMsgHandlers[Msg])
            lRet = m_mapMsgHandlers[Msg](wParam, lParam, this);
        else
        {
            if (m_lOriginalProc)
                lRet = CallWindowProc((WNDPROC)m_lOriginalProc, hWnd, Msg, wParam, lParam);
        }
    }
    //cBscCatchLogAndMsg(_T("MyMsgMap(") << (unsigned long const)hWnd << _T(",") << (unsigned long const)Msg << _T(",") << (unsigned long const)wParam << _T(",") << (unsigned long const)lParam << _T(")"));
    return lRet; //We didn't handle the message, so let windows handle it
}
//--------------------------------------------------------------------------------------------------


void CBscSubclassWnd::CreateIt()
{
    //cBscTry
    {
        SetWindowLong(m_hWnd, GWL_USERDATA, (long)this);
        m_lOriginalProc = (LONG)GetWindowLong(m_hWnd, GWL_WNDPROC );
        SetWindowLong(m_hWnd, GWL_WNDPROC, (long)MsgMap_stub);
    }
    //cBscCatchLogAndRethrow(_T("CreateIt()"));
}
//--------------------------------------------------------------------------------------------------
