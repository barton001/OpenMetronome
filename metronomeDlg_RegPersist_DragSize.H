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
#if !defined(AFX_MetronomeDlg_RegPersist_DragSize_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
#define AFX_MetronomeDlg_RegPersist_DragSize_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_
// MetronomeDlg_RegPersist_DragSize.h : header file
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <vector>
#include "MetronomeDlg_RegPersist.h"


//Specialises the base-class to allow a dialog that can be resized;
//The dialog can only be resized vertically, and a minimum size is enforced that ensures that
//the both the BPM slider and edit-box, and the measure configuration items are always visible.
//The effect is that if the windows is shrunk vertically, the BPM-slider (etc) are "glued" to the
//bottom of the dialog, and move upwards with it, rather than disappearing (run the app and see
//what I mean!).
class CMetronomeDlg_RegPersist_DragSize : public CMetronomeDlg_RegPersist
{
public:
	CMetronomeDlg_RegPersist_DragSize(HINSTANCE hInst = 0, HWND hParent = 0); //See base-class


protected:
    virtual long OnInitDialog(); //Overridden to read startup window size etc.
	
    DECLARE_MSG_HANDLER(OnSize, CMetronomeDlg_RegPersist_DragSize);//Overridde to show/hide btns etc

    //This is so that when we maximise the window,it goes to acutal max size,rather than full-screen
    //Also it fixes the horizontal size-border, and enforces the max and min vertical size border
	DECLARE_MSG_HANDLER(OnGetMinMaxInfo, CMetronomeDlg_RegPersist_DragSize);
    
private: //Member variables
    // Some vars for managing the dimensions of the dialog
	int const m_StdWidth;  //Dialog size as specified in the rc file, as opposed to observed startup
	int const m_MaxHeight; //Dialog size as specified in the rc file, as opposed to observed startup
    int const m_MinHeight; //Dialog is not allowed to be smaller than this

    // All the items that are anchored to the bottom of the dialog
    std::vector<int > m_aBotAnchoredDistFromBottoms; //Distance is maintained regardless of resize
    std::vector<HWND> m_ahwndBottomAnchored        ; //These windows are "locked" to bottom of dlg


private:
	void ReadInitialWindowSize(); //I.e., the on-screen window size
    static BOOL CALLBACK EnumChildProc(HWND hChild, LPARAM m_hWnd);
};
#endif // !defined(AFX_MetronomeDlg_RegPersist_DragSize_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
