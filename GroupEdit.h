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
#if !defined(AFX_GROUPEDIT_H__74EAD662_E563_11D8_92BE_444553540000__INCLUDED_)
#define AFX_GROUPEDIT_H__74EAD662_E563_11D8_92BE_444553540000__INCLUDED_
// GroupEdit.h: interface for the CGroupEdit class.
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <vector>
#include <string>
#include "group_beat.h"
#include "BscSubclassWnd.h"


//Specialises ordinary editbox to only allow integer numbers, brackets, and backspace characters
class CGroupEdit : public CBscSubclassWnd
{
// Construction
public:
	CGroupEdit(HWND hWnd, bool const bDeferCreation = false);
	virtual ~CGroupEdit();


    enum analyse_groups_error{
        ag_success,ag_misplaced_paren,ag_substring_overrun,ag_unclosed_paren,ag_no_beats};

	// returns an erro of type analyse_groups_error if the group string is invalid
	analyse_groups_error Parse(std::vector<group_beat> * pBeatData, unsigned long * pBPMeasure, std::basic_string<TCHAR> * pstrAdvancedCfg);


protected:
    DECLARE_MSG_HANDLER_SC(OnChar, CGroupEdit);
};


#endif // !defined(AFX_GROUPEDIT_H__74EAD662_E563_11D8_92BE_444553540000__INCLUDED_)
