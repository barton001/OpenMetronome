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
// IBeatBox.h: interface for the IBeatBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBeatBox_H__99EB1287_1D7C_4AE9_B2E0_4A0211302ED6__INCLUDED_)
#define AFX_IBeatBox_H__99EB1287_1D7C_4AE9_B2E0_4A0211302ED6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mmsystem.h"
#include <vector>


static unsigned long const UWM_BeatBox_BEAT_OCCURRED_wpBlinkSize_lpNULL = (WM_APP + 1); //wParam is a CMetBlinker::BLINKER_STATES, see MetBlinker.h; lParam is NULL
static unsigned long const UWM_BeatBox_ERROR_OCCURRED_wpNULL_lpNULL     = (WM_APP + 2); //WPARAM and LPARAM both NULL


#define ErrorCheck(cmd, ErrorString, ReturnOnError) if (!cmd) {m_strLastError = ErrorString; ::PostMessage(m_hWnd, UWM_BeatBox_ERROR_OCCURRED_wpNULL_lpNULL, NULL, NULL); if (ReturnOnError) return;}


//Interface to common methods of providing metronome audio output, and notification of when a beat
//has occurred (e.g., for a visual indication of each beat).
//Usage:
//1) Create your instance
//2) Use Play() and Stop() as required to start/ stop audio & beat-notification:
//      Beat notification is via the UWM_BeatBox_BEAT_OCCURRED_wpBlinkSize_lpNULL, see above
//3) The tempo and instrument volume can be changed while the audio is playing (some implementations
//   may stop-start the audio to achieve this, but from a class-caller's PoV this shouldn't matter).
//
//If an error is encountered (e.g., there is no audio-output hardware detected), the
//UWM_BeatBox_ERROR_OCCURRED_wpNULL_lpNULL message is sent ot the main application window:
//GetLastErr can then be called to provide a human-readable description of the error condition.
class IBeatBox
{
public:
    IBeatBox() {};
    virtual ~IBeatBox() {}; //Audio/ Beat message output ceases on destruction


    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual void SetTempo(unsigned long const BeatsPerMinute) = 0;
	virtual void SetVolume(float MasterVolume) = 0;

    virtual void Export(LPCTSTR lpszFilename, unsigned long const NumLoops) {}; //NULL Default instance, a bit cheaty! Not planning to implement this for MIDI!

    //nInstrumentNum is application-specific: for the WeirdMetronome app, it is the 0-8 index
    //relating to the 9 instrument-voice-"slots" (ordered top-to-bottom) shown on-screen.
    virtual void SetInstrumentVolume(unsigned long const nInstrumentNum, char const nVolume) = 0;
    virtual void SetInstrument      (unsigned long const nInstrumentNum, int  const nInstrument) = 0;
    virtual std::basic_string<TCHAR> GetLastErr() const = 0;
};

#endif // !defined(AFX_IBeatBox_H__99EB1287_1D7C_4AE9_B2E0_4A0211302ED6__INCLUDED_)
