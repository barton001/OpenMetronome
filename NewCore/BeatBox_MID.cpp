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

const double MS_PER_MINUTE = 60000.0;

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
	double NextBeatDelay_ms;
	unsigned int beatnum = 0;


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
				long const volume     = (long)(m_aVolumes[index] * m_MasterVolume);

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

        if (!m_bQuitThread)
        {
			// Trigger the blinker (only if blink size is visible and this is a downbeat)
			if (maxBlnk && (beatnum == 0))
				::PostMessage(m_hWnd, UWM_BeatBox_BEAT_OCCURRED_wpBlinkSize_lpNULL, maxBlnk, 0);
			beatnum = ++beatnum % m_TempoMultiplier;
			if (m_AltBeatsPerMinute) {
				unsigned long n_beatInMeasure = m_iSequence % m_BeatsPerBar;
				// Set timer for next beat
				if ((n_beatInMeasure + 1) < m_nPlayTheFirst_n_BeatsInBarAtAltTempo) {
					NextBeatDelay_ms = m_NextAltBeatDelay_ms;
				}
				else if (((n_beatInMeasure + 1) == m_nPlayTheFirst_n_BeatsInBarAtAltTempo) &&
					(m_iSequence < m_aInstrumentNums.size()))
				{
					// First non-alt-tempo beat. Calculate delay so it falls where it would if no alt-tempo.
					NextBeatDelay_ms = m_FirstOnBeatDelay_ms;
				}
				else {
					NextBeatDelay_ms = m_NextBeatDelay_ms;
				}
				m_uTimerID = timeSetEvent((unsigned long)(__max((NextBeatDelay_ms + 0.5), 1.0)), 1, (LPTIMECALLBACK)m_hEvtPollPlayback,
						0, TIME_CALLBACK_EVENT_SET | TIME_ONESHOT);
				ErrorCheck((m_uTimerID != NULL), _T("Unable to set beat timer! Metronome will not beat"), true);
			}

			// Wait for beat timer
            if (WAIT_FAILED == WaitForSingleObject(m_hEvtPollPlayback, INFINITE))
            {
                m_bQuitThread = true;
                m_strLastError = _T("Failed to wait for beat timer event to go off; beat thread terminating...");
                PostMessage(m_hWnd, UWM_BeatBox_ERROR_OCCURRED_wpNULL_lpNULL, NULL, NULL);
            }
        }
		++m_iSequence;
		m_iSequence = (m_iSequence) % (m_aInstrumentNums.size());
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
						   float			const   MasterVolume,
						   unsigned long	const	TempoMultiplier,
						   unsigned long	const   BeatsPerBar,
						   unsigned long	const   nPlayTheFirst_n_BeatsInBarAtAltTempo,
						   unsigned long	const   AltBeatsPerMinute,
                           HWND             const   hWndToSendBlinksAndErrorsTo) : 
    m_hWnd(hWndToSendBlinksAndErrorsTo),
    m_bQuitThread(false), m_hThread(NULL),
    m_hEvtPollPlayback(CreateEvent(NULL, FALSE, FALSE, NULL)),
    m_aInstrumentNums(aInstrumentNums),
    m_aInstruments(aInstruments),
    m_aVolumes(aVolumes),
    m_aBeatSizes(aBeatSizes),
    m_BeatsPerMinute(BeatsPerMinute),
	m_MasterVolume(MasterVolume),
	m_TempoMultiplier(TempoMultiplier),
	m_BeatsPerBar(BeatsPerBar),
	m_nPlayTheFirst_n_BeatsInBarAtAltTempo(nPlayTheFirst_n_BeatsInBarAtAltTempo),
	m_AltBeatsPerMinute(AltBeatsPerMinute),
    m_iSequence(0),
    m_uTimerID(NULL)
{
    ErrorCheck((TIMERR_NOERROR == timeBeginPeriod(1)), _T("Performance Warning: System Timer Resolution Too Low!"), false);
	m_NextBeatDelay_ms = MS_PER_MINUTE / (m_BeatsPerMinute * m_TempoMultiplier);
	if (m_AltBeatsPerMinute) {
		m_NextAltBeatDelay_ms = MS_PER_MINUTE / (m_AltBeatsPerMinute * m_TempoMultiplier);
		// First non-alt-tempo beat. Calculate delay so it falls where it would if no alt-tempo.
		m_FirstOnBeatDelay_ms = (MS_PER_MINUTE * m_nPlayTheFirst_n_BeatsInBarAtAltTempo) /
			(m_BeatsPerMinute * m_TempoMultiplier);
		// Subtract amount of time already used up by the alt-tempo beats
		m_FirstOnBeatDelay_ms -= (MS_PER_MINUTE * (m_nPlayTheFirst_n_BeatsInBarAtAltTempo - 1)) /
			(m_AltBeatsPerMinute * m_TempoMultiplier);
	}

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

    SetTempo(m_BeatsPerMinute); //Stops current timer (if any); Starts new timer

}
//--------------------------------------------------------------------------------------------------


void CBeatBox_MID::Stop()
{
	// Stop the next timer event 
	if (m_uTimerID != NULL)
		timeKillEvent(m_uTimerID);
	m_uTimerID = NULL;
}
//--------------------------------------------------------------------------------------------------


void CBeatBox_MID::SetTempo(unsigned long const BeatsPerMinute)
{
	// In case this is an on-the-fly tempo adjustment using the tempo hotkeys or slider,
	//  make a proportional adjustment to the alternate tempo so we keep the same feel.
	if (m_AltBeatsPerMinute) {
		m_AltBeatsPerMinute = (unsigned long)round(m_AltBeatsPerMinute * ((double)BeatsPerMinute / m_BeatsPerMinute));
		// Recalculate the delays between beats
		m_NextAltBeatDelay_ms = MS_PER_MINUTE / (m_AltBeatsPerMinute * m_TempoMultiplier);
		m_FirstOnBeatDelay_ms = (MS_PER_MINUTE * m_nPlayTheFirst_n_BeatsInBarAtAltTempo) /
			(BeatsPerMinute * m_TempoMultiplier);
		// Subtract amount of time already used up by the alt-tempo beats
		m_FirstOnBeatDelay_ms -= (MS_PER_MINUTE * (m_nPlayTheFirst_n_BeatsInBarAtAltTempo - 1)) /
			(m_AltBeatsPerMinute * m_TempoMultiplier);
	}
	m_BeatsPerMinute = BeatsPerMinute;	// update our base tempo
	m_NextBeatDelay_ms = MS_PER_MINUTE / (m_BeatsPerMinute * m_TempoMultiplier);

	// If non-varying tempo, we just kick off a periodic timer here.  Otherwise the timer
	//  will be set for each beat within the beat notification thread.
	if (m_AltBeatsPerMinute == 0) {

		Stop();	// stop the current periodic timer

		// All beats are the same timing, so let Windows automatically generate the timer events
		m_uTimerID = timeSetEvent((unsigned long)(__max((m_NextBeatDelay_ms + 0.5), 1.0)), 1, (LPTIMECALLBACK)m_hEvtPollPlayback,
			0, TIME_CALLBACK_EVENT_SET | TIME_PERIODIC);

		ErrorCheck((m_uTimerID != NULL), _T("Unable to set beat timer! Metronome will not beat"), true);
	}
}
//--------------------------------------------------------------------------------------------------

void CBeatBox_MID::SetVolume(float MasterVolume)
{
	m_MasterVolume = MasterVolume;
}