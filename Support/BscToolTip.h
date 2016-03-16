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
#if !defined(AFX_CBscToolTip_H__E852D391_8F43_4DDB_89A3_65FCC24D01B0__INCLUDED_)
#define AFX_CBscToolTip_H__E852D391_8F43_4DDB_89A3_65FCC24D01B0__INCLUDED_
// CBscToolTip.h: interface for the CBscToolTip class.
//
// Author: M.T.Billington (24-Jun-2004)
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

//Associates the constructor-specified tooltip-text with the constructor-specified IDC_id resorce(in
//the constructor-specified dialog), such that the text will be displayed when the user "hovers" the
//mouse over the control specified by the IDC_id
//Usage (e.g., from within a CBscBblDialog):
//m_autopTTRADIO_PLAIN = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_RADIO_PLAIN    , 
//    m_hWnd, m_hInstance, _T("Documentation for Plain Radio Button Goes Here!"));
class CBscToolTip
{
public:
    //See main class comment
	CBscToolTip(int const IDC_YourToolID, 
                HWND hDlgToolLivesIn, HINSTANCE hGlobalInstanceHandle, unsigned long const nIDTTip);


    //See main class comment
	virtual ~CBscToolTip();


protected:
    HWND m_hWndTip;
    std::basic_string<TCHAR> m_strTipText;
};

#endif // !defined(AFX_CBscToolTip_H__E852D391_8F43_4DDB_89A3_65FCC24D01B0__INCLUDED_)
