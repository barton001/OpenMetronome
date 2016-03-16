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
#if !defined(AFX_EditboxCatchKeypress_H__74EAD662_E563_11D8_92BE_444553540000__INCLUDED_)
#define AFX_EditboxCatchKeypress_H__74EAD662_E563_11D8_92BE_444553540000__INCLUDED_
// EditboxCatchKeypress.h: interface for the CEditboxCatchKeypress class.
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <string>
#include "BscSubclassWnd.h"


//Specialises ordinary editbox to catch keypresses, e.g., for shortcut keys if you pressed CTRL-ALT-L, it will display that
class CEditboxCatchKeypress : public CBscSubclassWnd
{
// Construction
public:
	CEditboxCatchKeypress(int const HotkeyID, unsigned long const Flag, unsigned long const VKey, HWND hWnd, bool const bDeferCreation = false);
	virtual ~CEditboxCatchKeypress();


    virtual unsigned long GetVKey() {return m_VKey;};
    virtual unsigned long GetFlag() {return m_Flag;};


    virtual void Clear();


protected:
    DECLARE_MSG_HANDLER_SC(OnKeydown, CEditboxCatchKeypress);

    virtual void UpdateWindowText();


private:
    int m_HotkeyID;
    unsigned long m_VKey;
    unsigned long m_Flag;
};


#endif // !defined(AFX_EditboxCatchKeypress_H__74EAD662_E563_11D8_92BE_444553540000__INCLUDED_)
