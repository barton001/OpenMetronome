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
// MetBlinker.cpp : implementation file
//
// Note to self:
// I have tried impelementing this with off-screen buffers and bitblts to try and stop apparent
// flicker but it doesn't seem to make any difference, so I'm concluding that the decay is so fast
// that the flicker is a trick of the eye, rather than the OS!
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cmath> //for sqrt, fabs etc
#include "MetBlinker.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//--------------------------------------------------------------------------------------------------
//                                                                                         Constants
//--------------------------------------------------------------------------------------------------


// The duration of each blink of the beat indicator (milliseconds)
static const int s_DurationOfInitialBlinkState_ms=20;
static const int s_InitialInterBlinkStateDecayDelay_ms=10;
static const int s_FinalInterBlinkStateDecayDelay_ms=2;


//--------------------------------------------------------------------------------------------------
//                                                                          Construction/Destruction
//--------------------------------------------------------------------------------------------------


CMetBlinker::CMetBlinker(unsigned char const bBigElseSmall, HWND hWnd, bool const bDeferCreation) :
    CBscSubclassWnd(hWnd, true),
    m_state(0),
    m_TimerInterval_ms(0),
    m_MetronomeBeatsEveryThisMany_ms(0),
    m_ApproxElapsedTime_ms(0),
    m_uidTimer(NULL)
{
    ADD_MSG_HANDLER_SC(WM_DESTROY, OnDestroy);
    ADD_MSG_HANDLER_SC(WM_TIMER  , OnTimer  );

    if (!bDeferCreation)
        CreateIt();

    //What size is our bitmap-square, and how many pixels does it have?
    //(Must be an integer multiple of 16 or finesse is required for word-alignment:)
    unsigned long const Size = (bBigElseSmall?32:16); 
    unsigned long const NumberOfPixels = Size*Size;

    //For each entry in our array, create some memory for the bitmap bits,
    //Populate that memory with a circle of the correct radius, and assign
    //that memory to a monochrome bitmap that we will create on the fly
    for (unsigned long i = 0; i < bsBiggest+1; ++i) //"i" is the size of the displayed "beat-circle"
    {
        //Allocate memory for bitmap (autoptr means we don't have to delete it later)
        //We use WORDs to ensure word-alignment (I think!)
        m_aaActualBitmapBits[i].resize(NumberOfPixels/sizeof(WORD));

        //Initially set our memory "all white"
        memset(&*m_aaActualBitmapBits[i].begin(), 0xFF, NumberOfPixels/8);

        //Now "draw" a filled circle onto our memory-area by examining each "pixel"...
        unsigned long ulWord = 0;
        for (unsigned long y = 0; y < Size; ++y)
        {
            WORD BitMask = 0x8000;
            for (unsigned long x = 0; x < Size; ++x)
            {
                //...And calculating its radial distance from the centre
                double dXDist = fabs((Size/2.0)-(x+.5)); //The .5's give centre us properly 
                double dYDist = fabs((Size/2.0)-(y+.5)); //(an even size means no actual centre-dot)
                double Radius = sqrt(dXDist*dXDist + dYDist*dYDist);

                //Apply a scaling factor to work out the limiting radius for our circle 
                double dMaxVal = bsBiggest+1-1;
                double dMaxRad = Size/2;

                if (Radius < dMaxRad*i/dMaxVal) //Set the bit if it is within the radius
                    m_aaActualBitmapBits[i][ulWord] &= ~BitMask; 
                    //(I say "set", we have to reset to make it black!)

                BitMask = BitMask>>1; //Shift our memory bitmask
                if (BitMask == 0x0000)
                {   //Ooops, our bitmask has fallen off the end of the current word...
                    BitMask = 0x8000; //...Reset it...
                    ++ulWord;         //...And point to the next word in our memory
                }
            }
        }

        //Intel WORD internal byte ordering will mess up our bitmap when the memory is mapped onto 
        //the screen; no problem, swap the hi and lo bytes
        _swab((char*)&*m_aaActualBitmapBits[i].begin(), (char*)&*m_aaActualBitmapBits[i].begin(), 
            NumberOfPixels/8);

        //And finally create our actual bitmap and point it at our previously allocated memory area
        m_HIDots[i] = CreateBitmap(Size, Size, 1, 1, &*m_aaActualBitmapBits[i].begin());
    }
}
//--------------------------------------------------------------------------------------------------


CMetBlinker::~CMetBlinker()
{
    for (unsigned long i = 0; i < bsBiggest+1; ++i)
        DeleteObject(m_HIDots[i]);
}
//--------------------------------------------------------------------------------------------------
//                                                                                    Public Members
//--------------------------------------------------------------------------------------------------


int CMetBlinker::SetState(const unsigned int new_state)
{
	unsigned int old_state = m_state;
	if(new_state != old_state)
	{
        m_state = ( (new_state>=0)?((new_state<bsBiggest+1)?new_state:bsSmallest):bsSmallest);
        SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (long)m_HIDots[new_state]);
	}

	return old_state;
}
//--------------------------------------------------------------------------------------------------


int CMetBlinker::IncrementState()
{
	int new_state = m_state + 1;
	if (new_state == bsBiggest+1)
		new_state--;

	return SetState(new_state);
}
//--------------------------------------------------------------------------------------------------


int CMetBlinker::DecrementState()
{
	int new_state = m_state - 1;
	if (new_state < 0)
		new_state = 0;

	return SetState(new_state);
}
//--------------------------------------------------------------------------------------------------


int CMetBlinker::GetState() const
{
	return m_state;
}
//--------------------------------------------------------------------------------------------------


static unsigned long const s_IDDecayTimer = 1;
void CMetBlinker::Beat(unsigned long  const MetronomeBeatsEveryThisMany_ms, 
                       BLINKER_STATES const InitialState)
{
    if (m_uidTimer)
        KillTimer(m_hWnd, m_uidTimer);
    m_MetronomeBeatsEveryThisMany_ms = MetronomeBeatsEveryThisMany_ms;
    SetState(InitialState);
    m_ApproxElapsedTime_ms = 0;
    m_uidTimer = SetTimer(m_hWnd, s_IDDecayTimer, m_TimerInterval_ms=s_DurationOfInitialBlinkState_ms,NULL);
}
//--------------------------------------------------------------------------------------------------
//                                                                                  Message Handlers
//--------------------------------------------------------------------------------------------------


long CMetBlinker::OnTimer(unsigned long const nIDEvent, long const lParam) 
{
	if (nIDEvent == s_IDDecayTimer)
    {
        if (GetState() > bsSmallest)
            DecrementState();

        m_ApproxElapsedTime_ms+=m_TimerInterval_ms;

        if (GetState() <= bsSmallest)
        {   //We are finished decrementing the state, give up
            KillTimer(m_hWnd, m_uidTimer);
            m_uidTimer = 0;
        }
        else
        {   //We have definitely sustained our initial blink state for the initial decay period;
            //We now need to decrement either every s_InitialInterBlinkStateDecayDelay_ms or
            //every s_FinalInterBlinkStateDecayDelay_ms depending on how close we are to next beat
            unsigned long const TimeLeftUntilNextBeat = m_MetronomeBeatsEveryThisMany_ms - 
                m_ApproxElapsedTime_ms;

            unsigned long const RequiredInterBlinkStateDecayDelay_ms = (TimeLeftUntilNextBeat > 20)?
                s_InitialInterBlinkStateDecayDelay_ms:s_FinalInterBlinkStateDecayDelay_ms;

            if (RequiredInterBlinkStateDecayDelay_ms != m_TimerInterval_ms)
            {
                KillTimer(m_hWnd, m_uidTimer);
                m_uidTimer = SetTimer(m_hWnd, s_IDDecayTimer, m_TimerInterval_ms = 
                    RequiredInterBlinkStateDecayDelay_ms, NULL);
            }
        }
    }
    return CallWindowProc((WNDPROC)m_lOriginalProc, m_hWnd, WM_TIMER, nIDEvent, lParam);
}
//--------------------------------------------------------------------------------------------------


long CMetBlinker::OnDestroy(unsigned long const nNULL, long const lNULL) 
{
    if (m_uidTimer)
        KillTimer(m_hWnd, m_uidTimer);
    m_uidTimer = 0;

    return CallWindowProc((WNDPROC)m_lOriginalProc, m_hWnd, WM_DESTROY, nNULL, lNULL);
}
//--------------------------------------------------------------------------------------------------
