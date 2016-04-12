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
// BeatBox_MID.cpp: implementation of the CBeatBox_MID class.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\general_midi.h"
#include "BeatBox_MID.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                                                 Main Audio Output/BlinkMsg Thread
//--------------------------------------------------------------------------------------------------
DWORD WINAPI CBeatBox_MID::BeatNotificationThread_stub(LPVOID pvThis)
{
    CBeatBox_MID * pThis = reinterpret_cast<CBeatBox_MID*>(pvThis);
    pThis->BeatNotificationThread();
    return 1;
}
//--------------------------------------------------------------------------------------------------


void CBeatBox_MID::BeatNotificationThread()
{
    //Open default MIDI Out device
	HMIDIOUT hmo; //This handles the actual MIDI IO

    MMRESULT midi_result = midiOutOpen(&hmo, MIDI_MAPPER, NULL, 0, CALLBACK_NULL);
	ErrorCheck(midi_result == MMSYSERR_NOERROR, _T("Cannot open default MIDI Device! Unable to produce audio output."), true); //!!!:Project-Global: grep Errorcheck, m_strLastError and translate the strings into Spanish

    while (!m_bQuitThread)
    {

		long maxBlnk = 0;
		// Loop through all the voices that play on this beat
        for (unsigned long i = 0; i < m_aInstrumentNums[m_iSequence].size(); ++i)
        {
            long const index      = m_aInstrumentNums[m_iSequence][i];
            if(index >= 0)
            {
                long const instrument = TO_MIDI(m_aInstruments[index]);
                long const volume     = m_aVolumes[index];

			    int const midi_event = 
				      (volume		<< 16)
				    | (instrument	<< 8 )
				    | 0x00000099;		// 9 = note on; 9 = channel 10 (percussion)
			    int const midi_result = midiOutShortMsg(hmo, midi_event); //!!!:check out midiStreamOut or midiOutLongMsg: I might be able to buffer up, say, 1mins-worth of MIDI data and let the OS/hardware handle scheduling
                if (!m_bQuitThread)
                {
            	    if(midi_result != MMSYSERR_NOERROR)
                    {
                        m_bQuitThread = true;
                        m_strLastError = _T("Unable to sound MIDI voice; beat thread terminating...");
                        PostMessage(m_hWnd, UWM_BeatBox_ERROR_OCCURRED_wpNULL_lpNULL, NULL, NULL);
                    }
                }

                long const Blnk = m_aBeatSizes[m_aInstrumentNums[m_iSequence][i]]; // get voice's blink size
				if (Blnk > maxBlnk) maxBlnk = Blnk;  // get largest blink for all simultaneous voices				
            }
            //else this is a silent note, so don't play it
        }
		// Trigger the blinker
		if (!m_bQuitThread && maxBlnk)
			::PostMessage(m_hWnd, UWM_BeatBox_BEAT_OCCURRED_wpBlinkSize_lpNULL, maxBlnk, 0);

		++m_iSequence;
        m_iSequence = (m_iSequence)%(m_aInstrumentNums.size());

        if (!m_bQuitThread)
        {
#ifdef _DEBUG
			// Set timer for next beat
			double const BPS = (((double)m_BeatsPerMinute * m_TempoMultiplier) / 60.0);
			double const BeatEveryThisMany_ms = 1000.0 / BPS;
			m_uTimerID = timeSetEvent(__max(((unsigned long)(BeatEveryThisMany_ms + 0.5)), 1), 1, (LPTIMECALLBACK)m_hEvtPollPlayback,
				0, TIME_CALLBACK_EVENT_SET | TIME_ONESHOT);
			ErrorCheck((m_uTimerID != NULL), _T("Unable to set beat timer! Metronome will not beat"), true);
#endif

			// Wait for beat timer
            if (WAIT_FAILED == WaitForSingleObject(m_hEvtPollPlayback, INFINITE))
            {
                m_bQuitThread = true;
                m_strLastError = _T("Failed to wait for beat timer event to go off; beat thread terminating...");
                PostMessage(m_hWnd, UWM_BeatBox_ERROR_OCCURRED_wpNULL_lpNULL, NULL, NULL);
            }
        }
	}

    if (hmo)
    {
        midiOutReset(hmo);
		midiOutClose(hmo);
        hmo = 0;
    }
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                                                          Construction/Destruction
//--------------------------------------------------------------------------------------------------
CBeatBox_MID::CBeatBox_MID(std::vector<std::vector<long> > const & aInstrumentNums,
                           std::vector<int> const & aInstruments,
                           std::vector<int> const & aVolumes,
                           std::vector<int> const & aBeatSizes,
                           unsigned long    const   BeatsPerMinute,
						   unsigned long	const	TempoMultiplier,
                           HWND             const   hWndToSendBlinksAndErrorsTo) : 
    m_hWnd(hWndToSendBlinksAndErrorsTo),
    m_bQuitThread(false), m_hThread(NULL),
    m_hEvtPollPlayback(CreateEvent(NULL, FALSE, FALSE, NULL)),
    m_aInstrumentNums(aInstrumentNums),
    m_aInstruments(aInstruments),
    m_aVolumes(aVolumes),
    m_aBeatSizes(aBeatSizes),
    m_BeatsPerMinute(BeatsPerMinute),
	m_TempoMultiplier(TempoMultiplier),
    m_iSequence(0),
    m_uTimerID(NULL)
{
    ErrorCheck((TIMERR_NOERROR == timeBeginPeriod(1)), _T("Performance Warning: System Timer Resolution Too Low!"), false);
        
	// Make sure there's at least one MIDI device
    ErrorCheck((midiOutGetNumDevs() > 0), _T("No MIDI devices found! Unable to produce audio output."), true);
}
//--------------------------------------------------------------------------------------------------


CBeatBox_MID::~CBeatBox_MID()
{
    m_bQuitThread = true;

    Stop(); //timeKillEvent(m_uTimerID);

    if (m_hThread)
    {
        SetEvent(m_hEvtPollPlayback);
        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
        m_hThread = 0;
    }

    if (m_hEvtPollPlayback)
    {
        CloseHandle(m_hEvtPollPlayback);
        m_hEvtPollPlayback = 0;
    }

    timeEndPeriod(1);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                                                                    Public Members
//--------------------------------------------------------------------------------------------------


void CBeatBox_MID::Play()
{
    if (!m_hThread)
    {
        DWORD dwThreadID = 0;
        m_hThread = CreateThread(NULL, NULL, BeatNotificationThread_stub, this, NULL, &dwThreadID);
    }
#ifndef _DEBUG
    SetTempo(m_BeatsPerMinute); //Stops current timer (if any); Starts new timer
#endif
}
//--------------------------------------------------------------------------------------------------


void CBeatBox_MID::Stop()
{
#ifdef _DEBUG
	m_bQuitThread = true;  // tells beatbox thread to exit on next beat loop
	if (m_hThread)
	{
		//SetEvent(m_hEvtPollPlayback);
		WaitForSingleObject(m_hThread, INFINITE);	// wait for thread to complete
		CloseHandle(m_hThread);
		m_hThread = 0;
	}
#else
    if (m_uTimerID!=NULL)
        timeKillEvent(m_uTimerID);
    m_uTimerID = NULL;
#endif
}
//--------------------------------------------------------------------------------------------------


void CBeatBox_MID::SetTempo(unsigned long const BeatsPerMinute)
{
	m_BeatsPerMinute = BeatsPerMinute;

#ifdef _DEBUG
	return;  // tempo automatically modified within beatbox
#endif

    Stop();

	double const BPS = (((double)m_BeatsPerMinute * m_TempoMultiplier)/60.0);
    double const BeatEveryThisMany_ms = 1000.0/BPS;

	// All beats are the same timing, so let Windows automatically generate the timer events
	m_uTimerID = timeSetEvent(__max(((unsigned long)(BeatEveryThisMany_ms + 0.5)), 1), 1, (LPTIMECALLBACK)m_hEvtPollPlayback,
		0, TIME_CALLBACK_EVENT_SET | TIME_PERIODIC);

    ErrorCheck((m_uTimerID!=NULL), _T("Unable to set beat timer! Metronome will not beat"), true);

}
//--------------------------------------------------------------------------------------------------
