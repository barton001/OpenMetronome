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
#if !defined(AFX_CBscSubclassWnd_H__E852D391_8F43_4DDB_89A3_65FCC24D01B0__INCLUDED_)
#define AFX_CBscSubclassWnd_H__E852D391_8F43_4DDB_89A3_65FCC24D01B0__INCLUDED_
// CBscSubclassWnd.h: interface for the CBscSubclassWnd class.
//
// Author: M.T.Billington (24-Jun-2004)
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

//I used to use dynamic cast here, but it seemed to cause problems in PPC2002 and earlier...
#define DECLARE_MSG_HANDLER_SC(MyHandlerName, MyHandlerClass) virtual long MyHandlerName(unsigned long const wParam, long const lParam); static long stub##MyHandlerName (unsigned long const wParam, long const lParam, CBscSubclassWnd * pThis){return ((MyHandlerClass*)(pThis))->MyHandlerName(wParam,lParam);};
#define ADD_MSG_HANDLER_SC(Msg, MyHandlerName) AddMsgHandler(Msg, stub##MyHandlerName);

//Allows general-purpose subclassing of windows to allow the user to associate their own message-
//handlers with windows.  See CBscBblDlg - this class does almost exactly the same thing, except for
//general subclassing rather than just dialogs.  I will "merge" these two classes at some point in the
//future.
class CBscSubclassWnd
{
public:
    CBscSubclassWnd(HWND hWnd, bool const bDeferCreation = false);
    virtual ~CBscSubclassWnd();
    

    static LRESULT WINAPI MsgMap_stub(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


    operator HWND() const {return m_hWnd;};


protected:
    virtual LRESULT MyMsgMap(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    typedef long (*MsgHandler)(unsigned long const wParam, long const lParam, CBscSubclassWnd * pvThis);
    virtual void AddMsgHandler(unsigned long  const Msg   , MsgHandler pfnHandleMsg);


    virtual void CreateIt();


    HWND m_hWnd;
    long m_lOriginalProc;


private:
    std::map<unsigned long, MsgHandler> m_mapMsgHandlers;
    static BOOL CALLBACK MyMsgMap_stub(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};

#endif // !defined(AFX_CBscSubclassWnd_H__E852D391_8F43_4DDB_89A3_65FCC24D01B0__INCLUDED_)
