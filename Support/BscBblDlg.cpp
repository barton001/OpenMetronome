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
// CBscBblDlg.cpp: implementation of the CBscBblDlg class.
//
// Author: M.T.Billington (24-Jun-2004)
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "cBscCatch.h" //Open-source-land can't have this:) Also slows down PPCs, apparently
#include "BscBblDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Construction/ Destruction
//--------------------------------------------------------------------------------------------------
CBscBblDlg::CBscBblDlg(WORD wTemplate, HINSTANCE hInstance,
                       HWND hParent,
                       bool const bCreateModelessOnConstruction,
                       bool const bDeferModelessCreation) :
 
    m_wTemplate(wTemplate),
    m_hInstance(hInstance),
    m_hParent(hParent),
    m_hWnd(0)
{
    //cBscTry
    {
        if ((bCreateModelessOnConstruction) && (!bDeferModelessCreation))
            CreateIt();
    }
    //cBscCatchLogAndRethrow(_T("CBscBblDlg()"));
}
//--------------------------------------------------------------------------------------------------
   

CBscBblDlg::~CBscBblDlg()
{
    //cBscTry
    {
        //Certainly in the case of a modal dialog, I don't need to do any cleanup
    }
    //cBscCatchLogAndMsg(_T("~CBscBblDlg()"));//Dont let exception escape destructor
}
//--------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    Public Members
//--------------------------------------------------------------------------------------------------

    
long CBscBblDlg::DoModal()
{
    long lRet = 0;

    //cBscTry
    {
        lRet = DialogBoxParam(m_hInstance,  
            MAKEINTRESOURCE(m_wTemplate), m_hParent, MyMsgMap_stub, (long)this);

        long lLastError = GetLastError();
        m_hWnd = NULL; //After return of DialogBoxParam, m_hWnd is no longer valid.
    }
    //cBscCatchLogAndRethrow(_T("DoModal()"));
    return lRet;
}
//--------------------------------------------------------------------------------------------------


std::basic_string<TCHAR> CBscBblDlg::GetWindowText(unsigned long ctrlID)
{
    std::basic_string<TCHAR> strRet;

    //cBscTry
    {
        strRet.resize(256);
        ::GetWindowText(GET_HWND(ctrlID), &*strRet.begin(), strRet.length());
    }
    //cBscCatchLogAndRethrow(_T("DoModal()"));
    return strRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                  Helper Functions
//--------------------------------------------------------------------------------------------------

    
void CBscBblDlg::AddMsgHandler(unsigned long const Msg, MsgHandler pfnHandleMsg)
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

    
void CBscBblDlg::AddCmdHandler(unsigned short const CtrlID, MsgHandler pfnHandleMsg)
{
    //cBscTry
    {
        //Provided this is only called in constructor before the class kicks off I shouldn't need
        //critical sections etc to protect this member
        m_mapCmdHandlers[CtrlID] = pfnHandleMsg;
    }
    //cBscCatchLogAndRethrow(_T("AddMsgHandler(...)"));//Dont let exception escape destructor
}
//--------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       Message Map
//--------------------------------------------------------------------------------------------------
BOOL CBscBblDlg::MyMsgMap_stub(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    long lRet = 0;
    //cBscTry
    {   //We need to intercept WM_INITDIALOG to set our this pointer that we'll use everywhere else.
        if (Msg == WM_INITDIALOG)
        {
            SetWindowLong(hWnd, GWL_USERDATA, (long)lParam);
            CBscBblDlg * pThis = reinterpret_cast<CBscBblDlg *>(lParam);
            pThis->m_hWnd = hWnd;
        }

        CBscBblDlg * pThis = reinterpret_cast<CBscBblDlg *>(GetWindowLong(hWnd, GWL_USERDATA));
        if (pThis)
            lRet = pThis->MyMsgMap(hWnd, Msg, wParam, lParam);
    }
    ////cBscCatchLogAndMsg(_T("MyMsgMap_stub(") << (unsigned long const)hWnd << _T(",") << 
    //    (unsigned long const)Msg << _T(",") << (unsigned long const)wParam << _T(",") << 
    //    (unsigned long const)lParam << _T(")"));
    return lRet;
}
//--------------------------------------------------------------------------------------------------


LRESULT CBscBblDlg::MyMsgMap(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRet = 0;
    WINDOWPOS* pLp = (WINDOWPOS*)lParam;
    //cBscTry
    {
        switch(Msg)
        {
        case WM_INITDIALOG:
            {
                lRet = OnInitDialog();
                break;
            }        
        case WM_COMMAND:
            {
                switch(LOWORD(wParam))
                {
                case IDOK:
                    {
                        lRet = OnOK();
                        break;
                    }                
                case IDCANCEL:
                    {
                        lRet = OnCancel();
                        break;
                    }
                default:
                    {
                        lRet = OnCommand(LOWORD(wParam), HIWORD(wParam), lParam);
                    }
                }
                break;
            }
        default:
            {
                if (m_mapMsgHandlers[Msg])
                    lRet = m_mapMsgHandlers[Msg](wParam, lParam, this);
                break;
            }
        }
    }
    ////cBscCatchLogAndMsg(_T("MyMsgMap(") << (unsigned long const)hWnd << _T(",") << 
    //    (unsigned long const)Msg << _T(",") << (unsigned long const)wParam << _T(",") << 
    //    (unsigned long const)lParam << _T(")"));

    return lRet; //We didn't handle the message, so let windows handle it
}
//--------------------------------------------------------------------------------------------------


long CBscBblDlg::OnInitDialog()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------


long CBscBblDlg::OnCancel()
{
    //cBscTry
    {
        EndDialog(m_hWnd, IDCANCEL);
    }
    //cBscCatchLogAndRethrow(_T("OnCancel()"));//Dont let exception escape destructor
    return 1;
}
//--------------------------------------------------------------------------------------------------


long CBscBblDlg::OnOK()
{
    //cBscTry
    {
        EndDialog(m_hWnd, IDOK);
    }
    //cBscCatchLogAndRethrow(_T("OnOK()"));//Dont let exception escape destructor
    return 1;
}
//--------------------------------------------------------------------------------------------------


long CBscBblDlg::OnCommand(unsigned short const CtrlID, 
                           unsigned short const HiWordWparam, LPARAM const lParam)
{
    long lRet = 0;
    //cBscTry
    {
        if (m_mapCmdHandlers[CtrlID])
            lRet = m_mapCmdHandlers[CtrlID](HiWordWparam, lParam, this);
    }
    ////cBscCatchLogAndMsg(_T("MyMsgMap_stub(") << (unsigned long const)CtrlID << _T(",") << 
    //    (unsigned long const)HiWordWparam << _T(",") << (unsigned long const)lParam << _T(")"));
    return lRet;
}
//--------------------------------------------------------------------------------------------------


void CBscBblDlg::CreateIt()
{
    //cBscTry
    {
        m_hWnd = CreateDialogParam(m_hInstance, 
            MAKEINTRESOURCE(m_wTemplate), m_hParent, MyMsgMap_stub, (long)this);

        //if (!m_hWnd)
        //    cBscThrowLastError(_T("CreateDialogParam failed:"));
    }
    //cBscCatchLogAndRethrow(_T("CreateIt()"));//Dont let exception escape destructor
}
//--------------------------------------------------------------------------------------------------
