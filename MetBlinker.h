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
#if !defined(AFX_METBLINKER_H__B2991204_D4EE_11D7_92BD_444553540000__INCLUDED_)
#define AFX_METBLINKER_H__B2991204_D4EE_11D7_92BD_444553540000__INCLUDED_
// MetBlinker.h: interface for the CMetBlinker class.
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <vector>
#include "BscSubclassWnd.h"


//CMetBlinker: this class is a specialised CStatic that creates a solid 32x32 
//(or 16x16 if the bBigElseSmall constructor bool is 0) circle in the top-left
//area of the static's client area.  The size of the circle can be altered using
//SetState(). It cannot be set beyond its initial size, nor below 0.
//Additionally, the circle can be made to "beat": This draws the circle at the specified
//initial state, and the circle automatically decrements to zero.  This is used to give a
//visual indication of when a metronome "voice" beats.
//Usage:
//See CBscSubclassWnd, and above.
class CMetBlinker : public CBscSubclassWnd
{
public:
    //See main class/baseclass comment for details
	CMetBlinker(unsigned char const bBigElseSmall, HWND hWnd, bool const bDeferCreation = false); 
	virtual ~CMetBlinker();

    enum BLINKER_STATES{bsSmallest = 0, bsHalf = 4, bsBiggest = 8};

    //See main class comment for details
	virtual int  SetState(unsigned int const new_state /*bsSmallest to bsBiggest*/);

    //See main class comment for details
    virtual void Beat(unsigned long  const MetronomeBeatsEveryThisMany_ms,//Sets time for beat-decay
                      BLINKER_STATES const InitialState);


protected:
    DECLARE_MSG_HANDLER_SC(OnTimer  , CMetBlinker);
    DECLARE_MSG_HANDLER_SC(OnDestroy, CMetBlinker);


    virtual int  IncrementState(); //See main class comment for details
	virtual int  DecrementState(); //See main class comment for details
	virtual int  GetState() const; //See main class comment for details


private:
    unsigned long m_uidTimer;
    unsigned long m_MetronomeBeatsEveryThisMany_ms;
    unsigned long m_TimerInterval_ms;
    unsigned long m_ApproxElapsedTime_ms;
	unsigned int m_state; //As set by SetState; defaults to 0
    std::vector<WORD> m_aaActualBitmapBits[bsBiggest+1];
	HBITMAP m_HIDots[bsBiggest+1];  // The BITMAPs to be used for the blinker
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_METBLINKER_H__B2991204_D4EE_11D7_92BD_444553540000__INCLUDED_)
