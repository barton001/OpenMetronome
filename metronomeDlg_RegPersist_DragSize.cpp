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
// MetronomeDlg_RegPersist_DragSize.cpp : implementation file
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MetronomeDlg_RegPersist_DragSize.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                                                          Construction/Destruction
//--------------------------------------------------------------------------------------------------
CMetronomeDlg_RegPersist_DragSize::CMetronomeDlg_RegPersist_DragSize(HINSTANCE hInst, HWND hParent): 
    CMetronomeDlg_RegPersist(hInst, hParent),
    m_StdWidth  (0),
    m_MaxHeight (0),
    m_MinHeight (0)
{
    //Message Map
    ADD_MSG_HANDLER(WM_SIZE, OnSize);
    ADD_MSG_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                                                                  Message Handlers
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg_RegPersist_DragSize::OnInitDialog()
{
	// Assign the anchored control pointers
    m_ahwndBottomAnchored.clear();
    m_ahwndBottomAnchored.push_back(GET_HWND(IDC_PRESETS             ));
	m_ahwndBottomAnchored.push_back(GET_HWND(IDC_SAVE_PRESET_BUTTON  ));
	m_ahwndBottomAnchored.push_back(GET_HWND(IDC_EXPORT_PRESET_BUTTON));
	m_ahwndBottomAnchored.push_back(GET_HWND(IDC_DELETE_PRESET_BUTTON));
	m_ahwndBottomAnchored.push_back(GET_HWND(IDC_PRESET_COMBO        ));
	m_ahwndBottomAnchored.push_back(GET_HWND(IDC_BPMINUTE_SLIDER     ));
	m_ahwndBottomAnchored.push_back(GET_HWND(IDC_TEMPO               ));
	m_ahwndBottomAnchored.push_back(GET_HWND(IDC_BPMINUTE_EDIT       ));

    ReadInitialWindowSize();

	CMetronomeDlg_RegPersist::OnInitDialog();

    return 1;  // return TRUE  unless you set the focus to a control
}
//--------------------------------------------------------------------------------------------------


BOOL CALLBACK CMetronomeDlg_RegPersist_DragSize::EnumChildProc(HWND hChild, LPARAM lpThis)
{
    CMetronomeDlg_RegPersist_DragSize* pThis = (CMetronomeDlg_RegPersist_DragSize*) lpThis;
    HWND hWnd = *pThis;

	RECT rcwndCurrent; // Figure out what the current size is of the window		
	GetWindowRect(GetDlgItem(hWnd, IDC_BPMINUTE_SLIDER), &rcwndCurrent);
	RECT rcWndChild; // Figure out what the current size is of the window		
    GetWindowRect(hChild, &rcWndChild);

    if ((hChild != GetDlgItem(hWnd, IDC_PRESETS             )) &&
        (hChild != GetDlgItem(hWnd, IDC_SAVE_PRESET_BUTTON  )) &&
        (hChild != GetDlgItem(hWnd, IDC_DELETE_PRESET_BUTTON)) &&
        (hChild != GetDlgItem(hWnd, IDC_EXPORT_PRESET_BUTTON)) &&
        (hChild != GetDlgItem(hWnd, IDC_PRESET_COMBO        )) &&
        (hChild !=             pThis->m_hComboEditChild      ) &&
        (hChild != GetDlgItem(hWnd, IDC_BPMINUTE_SLIDER     )) &&
        (hChild != GetDlgItem(hWnd, IDC_TEMPO               )) &&
        (hChild != GetDlgItem(hWnd, IDC_BPMINUTE_EDIT       )))
        ShowWindow(hChild, (rcWndChild.bottom > rcwndCurrent.top+2)?SW_HIDE:SW_SHOW);
    return TRUE;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg_RegPersist_DragSize::OnSize(unsigned long const nType, long const lCxCy)
{
    int const cx = LOWORD(lCxCy), cy = HIWORD(lCxCy);


	if(m_MaxHeight > 0) //else un-initialised
	{
		// Hide the text labels if necessary
        //::ShowWindow(GET_HWND(IDC_BLINK_CHECK), first_tick_to_hide >= 0);

        //Glue the anchored controls to the bottom of the screen...
	    RECT rccliCurrent;
		GetClientRect(m_hWnd, &rccliCurrent);
	    RECT rcscrCurrent = rccliCurrent;

        POINT tl = {rcscrCurrent.left , rcscrCurrent.top   };
        POINT br = {rcscrCurrent.right, rcscrCurrent.bottom};
	    ClientToScreen(m_hWnd, &tl);
	    ClientToScreen(m_hWnd, &br);
        rcscrCurrent.left   = tl.x;
        rcscrCurrent.top    = tl.y;
        rcscrCurrent.right  = br.x;
        rcscrCurrent.bottom = br.y;
        {for(unsigned long i = 0; i < m_ahwndBottomAnchored.size(); ++i)
		{
			// Move the presets box
            RECT rcwndBA;
            ::GetWindowRect(m_ahwndBottomAnchored[i], &rcwndBA);

            ::SetWindowPos(m_ahwndBottomAnchored[i], 0, 
				rcwndBA.left - rcscrCurrent.left, 
				rccliCurrent.bottom - m_aBotAnchoredDistFromBottoms[i],
				0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);
        }}

        //And hide any controls that they now would obscure
        EnumChildWindows(m_hWnd, EnumChildProc, (long)this);
	}

	return 0;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg_RegPersist_DragSize::OnGetMinMaxInfo(unsigned long const Dummy, long const lpmmi)
{
    MINMAXINFO * lpMMI = (MINMAXINFO *)lpmmi;

    long lRet = 0;

    if (m_MaxHeight > 0)
    {
        //Set size limits of our window...
        lpMMI->ptMaxTrackSize.x = m_StdWidth; 
	    lpMMI->ptMaxTrackSize.y = m_MaxHeight; 
	    lpMMI->ptMinTrackSize.x = m_StdWidth; 
	    lpMMI->ptMinTrackSize.y = m_MinHeight; 
	    lpMMI->ptMaxSize = lpMMI->ptMaxTrackSize;

        //Stop thet window jumping to top-left of screen on maximise
        RECT rcwndMe;
        ::GetWindowRect(m_hWnd, &rcwndMe);
	    lpMMI->ptMaxPosition.x = rcwndMe.left; 
	    lpMMI->ptMaxPosition.y = rcwndMe.top ; 
        lRet = 1;
    }
    //else uninitialised; we were called pre-on-init-dialog, allow .rc default maximum to be set

    return lRet;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                                                                           Helpers
//--------------------------------------------------------------------------------------------------
void CMetronomeDlg_RegPersist_DragSize::ReadInitialWindowSize()
{	
	//Get the current inner-window-size (client-rect), and its on-screen position ("scr" rect)
    RECT rccliMain = {0,0,0,0};
	GetClientRect(m_hWnd, &rccliMain);
	RECT rcscrMain = rccliMain;
    POINT ptTL = {rcscrMain.left, rcscrMain.top}, ptBR = {rcscrMain.right, rcscrMain.bottom};
	ClientToScreen(m_hWnd, &ptTL);
	ClientToScreen(m_hWnd, &ptBR);
    rcscrMain.top = ptTL.y;
    rcscrMain.left = ptTL.x;
    rcscrMain.bottom = ptBR.y;
    rcscrMain.right = ptBR.x;

	//Figure out the size and position of the anchored controls;work out the topmost y-coord of them
    //This topmost y-coord is used in with the bottom of the group-edit to define min window y-size
    long nTopmost = LONG_MAX;
    {for(unsigned long i = 0; i < m_ahwndBottomAnchored.size(); ++i)
	{
        RECT rcwndBaCtrl = {0,0,0,0};
        ::GetWindowRect(m_ahwndBottomAnchored[i], &rcwndBaCtrl);
		m_aBotAnchoredDistFromBottoms.push_back(rcscrMain.bottom - rcwndBaCtrl.top);
        nTopmost = __min(rcwndBaCtrl.top, nTopmost);
    }}

	//Now work out the main window size and position, and the position of the GroupEdit;
    //We'll set min height to be so that the anchored controls and the GroupEdit are always visible
	RECT rcwndMain      = {0,0,0,0};
    ::GetWindowRect(m_hWnd,                               &rcwndMain     );
    RECT rcwndGroupEdit = {0,0,0,0};
    ::GetWindowRect(GET_HWND(IDC_GROUP_EDIT), &rcwndGroupEdit);

	//Figure out how high each tick entry is so we know when to make them disappear when resizing
    RECT rcwndSlider2 = {0,0,0,0};
    RECT rcwndSlider1 = {0,0,0,0};
    ::GetWindowRect(GET_HWND(IDC_TICK_VOLUME_SLIDER2), &rcwndSlider2);
    ::GetWindowRect(GET_HWND(IDC_TICK_VOLUME_SLIDER1), &rcwndSlider1);

    //We're going to specify these as const because, after OnInitDialog, they never change
    //This is a bit cheaty, but IMO nicer than having non-const variables in the first place.
    //Ideally we'd assign them in the constructor, but MFC-style 2-stage construction won't let us
	*const_cast<int*>(&m_StdWidth  ) = rcwndMain.right       - rcwndMain.left  ;
	*const_cast<int*>(&m_MaxHeight ) = GetSystemMetrics(SM_CYMENU) + rcwndMain.bottom      - rcwndMain.top   ;
	*const_cast<int*>(&m_MinHeight ) =(rcwndMain.bottom      - nTopmost        ) + 
                                      (rcwndGroupEdit.bottom - rcwndMain.top);
}
//--------------------------------------------------------------------------------------------------


