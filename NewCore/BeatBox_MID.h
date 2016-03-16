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
#if !defined(AFX_BeatBox_MID_H__99EB1287_1D7C_4AE9_B2E0_4A0211302ED6__INCLUDED_)
#define AFX_BeatBox_MID_H__99EB1287_1D7C_4AE9_B2E0_4A0211302ED6__INCLUDED_
// BeatBox_MID.h: interface for the CBeatBox_MID class.
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IBeatBox.h"
#include "mmsystem.h"
#include <vector>


//See IBeatBox.h: Implements thet IBeatBox interface via the default MIDI device "MIDI_MAPPER"
class CBeatBox_MID : public IBeatBox 
{
public:
	CBeatBox_MID(std::vector<std::vector<long> > const & aInstrumentNums, //This contains indicies (0-8) into the three arrays that follow; it is itself indexed as aInstrumentNums[nBeat][nVoice], e.g., where "aInstrumentNums[n]" returns an array of voices to sound on beat "n" (this array of voices can (and usually will!) contain just one voice of course)
                 std::vector<int               > const & aInstruments,    //These are the 9 instruments visible down the screen          ;supplied reference must persist beyond the lifetime of this class's instance
                 std::vector<int               > const & aVolumes,        //These are the 9 instrument volumes visible down the screen   ;supplied reference must persist beyond the lifetime of this class's instance
                 std::vector<int               > const & aBeatSizes,      //These are the 9 instrument beat sizes visible down the screen;supplied reference must persist beyond the lifetime of this class's instance
                 unsigned long                   const   BeatsPerMinute,
				 unsigned long					 const   TempoMultiplier,	// BHB
                 HWND                            const   hWndToSendBlinksAndErrorsTo);
	
    
    virtual ~CBeatBox_MID(); //Audio/ Beat message output ceases on destruction


    virtual void Play(); //See IBeatBox
    virtual void Stop(); //See IBeatBox
    virtual void SetTempo(unsigned long const BeatsPerMinute); //See IBeatBox
    virtual void SetInstrumentVolume(unsigned long const nInstrumentNum, char const nVolume    ) {m_aVolumes    [nInstrumentNum] = nVolume    ;}; //See IBeatBox
    virtual void SetInstrument      (unsigned long const nInstrumentNum, int  const nInstrument) {m_aInstruments[nInstrumentNum] = nInstrument;}; //See IBeatBox
    virtual std::basic_string<TCHAR> GetLastErr() const {return m_strLastError;}; //See IBeatBox


private: //Constants
    unsigned long                           m_BeatsPerMinute ; //Copy of constructor param; set by SetTempo(...)
	unsigned long							m_TempoMultiplier; // BHB
    std::vector<std::vector<long> > const   m_aInstrumentNums; //Copy of constructor param
    std::vector<int               >         m_aInstruments   ; //Copy of constructor param
    std::vector<int               >         m_aVolumes       ; //Copy of constructor param
    std::vector<int               > const & m_aBeatSizes     ; //Copy of constructor param

    
private: //Stuff for the main beat thread    
    HANDLE m_hThread; //Handle to BeatNotificationThread_stub
    HANDLE m_hEvtPollPlayback; //Set by High-rest timer to schedule beats
    bool m_bQuitThread; //Signals BeatNotificationThread to quit
    static DWORD WINAPI BeatNotificationThread_stub(LPVOID pvThis); //Just calls BeatNotificationThread; saves me having to do pvThis-> everywhere
    void BeatNotificationThread(); //This is the bit that does the work!


private:
    UINT m_uTimerID; //Shedules beats
    unsigned long m_iSequence; //Used for m_aInstrumentNums[m_iSequence] to keep track of which beat we are on


private:
    HWND m_hWnd; //Used to implement GetLastErr and beat blinking
    std::basic_string<TCHAR> m_strLastError; //Used to implement GetLastError
};

#endif // !defined(AFX_BeatBox_MID_H__99EB1287_1D7C_4AE9_B2E0_4A0211302ED6__INCLUDED_)
