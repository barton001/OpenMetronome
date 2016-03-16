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
#if !defined(AFX_BeatBox_WAV_H__99EB1287_1D7C_4AE9_B2E0_4A0211302ED6__INCLUDED_)
#define AFX_BeatBox_WAV_H__99EB1287_1D7C_4AE9_B2E0_4A0211302ED6__INCLUDED_
// BeatBox_WAV.h: interface for the CBeatBox_WAV class.
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>
#include "IBeatBox.h"
#include "mmsystem.h"
#include "WaveFile.h"


//See IBeatBox.h: Implements thet IBeatBox interface via output of WAV files representing the
//various MIDI voices.  The first WAV output device found is used.
//WAV files are expected to live in a "Samples" subdirectory of the main executable location,
//and to have filenames the same as the strings defining the MIDI voices, e.g., 
//"Acoustic Bass Drum.wav"; if the required WAV is not found, "ZZ_Default.wav" is used.
//"ZZ_Default.wav" currently *must* exist, and currently all WAVs must be of the same
//format as "ZZ_Default.wav" (and further, the format of the default-supplied ZZ_Default.wav
//cannot change).  See .cpp for details of WAV restrictions and the reasons for them.
//
// NOTE: YOU SHOULD ONLY EVER HAVE ONE INSTANCE OF THIS CLASS IN EXISTENCE AT ANY ONE TIME!
// This is because I've "cheated" somewhat in a few places in order to statically cache data
// in order to (hopefully) speed up performance on Windows Mobile devices.
//Specifically, I'm currently using mono uncompressed PCM WAV files 44.1kHz
//(16-bits-unsigned-int-samples) produced by "Audacity" (an open-source WAV editor).
class CBeatBox_WAV : public IBeatBox 
{
public:
	CBeatBox_WAV(std::vector<std::vector<long> > const & aInstrumentNums, //This contains indicies (0-8) into the three arrays that follow; it is itself indexed as aInstrumentNums[nBeat][nVoice], e.g., where "aInstrumentNums[n]" returns an array of voices to sound on beat "n" (this array of voices can (and usually will!) contain just one voice of course)
                 std::vector<int               > const & aInstruments,    //These are the 9 instruments visible down the screen          ;supplied reference must persist beyond the lifetime of this class's instance
                 std::vector<int               > const & aVolumes,        //These are the 9 instrument volumes visible down the screen   ;supplied reference must persist beyond the lifetime of this class's instance
                 std::vector<int               > const & aBeatSizes,      //These are the 9 instrument beat sizes visible down the screen;supplied reference must persist beyond the lifetime of this class's instance
                 unsigned long                   const   BeatsPerMinute,
				 unsigned long                   const   TempoMultiplier, // BHB
		/* BHB change next to to unsigned long
				 unsigned char                   const   BeatsPerBar,
                 unsigned char                   const   nPlayTheFirst_n_BeatsInBarAtAltTempo,
				 */
				 unsigned long                   const   BeatsPerBar,
				 unsigned long                   const   nPlayTheFirst_n_BeatsInBarAtAltTempo,
				 unsigned long                   const   AltBeatsPerMinute,
                 HWND                            const   hWndToSendBlinksAndErrorsTo);


	virtual ~CBeatBox_WAV(); //Audio/ Beat message output ceases on destruction


    virtual void Play(); //See IBeatBox
    virtual void Stop(); //See IBeatBox
    virtual void SetTempo(unsigned long const BeatsPerMinute) {if (m_BeatsPerMinute != BeatsPerMinute) {m_BeatsPerMinute = BeatsPerMinute; Create1MinOfSamples();}}; //See IBeatBox
    virtual void SetInstrumentVolume(unsigned long const nInstrumentNum, char const nVolume) {if (m_aVolumes[nInstrumentNum] != nVolume) {m_aVolumes[nInstrumentNum] = nVolume; Create1MinOfSamples();}}; //See IBeatBox
    virtual void SetInstrument      (unsigned long const nInstrumentNum, int  const nInstrument) {if (m_aInstruments[nInstrumentNum] != nInstrument) {m_aInstruments[nInstrumentNum] = nInstrument; Create1MinOfSamples();}}; //See IBeatBox
    virtual std::basic_string<TCHAR> GetLastErr() const {return m_strLastError;}; //See IBeatBox
    virtual void Export(LPCTSTR lpszFilename, unsigned long const NumLoops);
    virtual void Prep();


    static void GetUserVoices(std::vector<std::basic_string<TCHAR> > * pUserVoices);


private: //Constants
    unsigned long                           m_BeatsPerMinute ; //Copy of constructor param; set by SetTempo(...)
	unsigned long                           m_TempoMultiplier; // BHB - Copy of constructor param
    std::vector<std::vector<long> > const   m_aInstrumentNums; //Copy of constructor param
    std::vector<int               >         m_aInstruments   ; //Copy of constructor param
    std::vector<int               >         m_aVolumes       ; //Copy of constructor param
    std::vector<int               > const & m_aBeatSizes     ; //Copy of constructor param

    //std::vector<std::vector<__int32> >      m_aVoiceWAVBufs  ; //Indexed just like m_aInstruments


private:
    HANDLE m_hThread; //Handle to BeatNotificationThread_stub
    HANDLE m_hEvtPollPlayback; //Used to schedule blinks in BeatNotificationThread
    bool   m_bQuitThread; //Signals BeatNotificationThread to quit
    static DWORD WINAPI BeatNotificationThread_stub(LPVOID pvThis); //Just calls BeatNotificationThread
    void BeatNotificationThread();//Sends blinks to main app window;Actual audio out is handled by OS


private:
    //The class works by creating 1 minute's worth of WAV data from component samples, and then
    //instructing the OS to play it as a continous loop. It then polls the playback device (i.e., the
    //WAV API's WaveOut device) to see whereabouts in the loop it is at any given time) and, whenever
    //it detects the playback has "rolled over a beat", it sends a beat notification message to the
    //constructor-specified HWND.
    void InitialiseWaveOutDeviceUsingDefaultWAV();
    void PlayWAVAndSendInitialBeat(); //"Send initial beat" means send a blink to the main window
    void Create1MinOfSamples();
    bool PopulateVoiceWAVBufs(); //returns false if any wav's are of different or unusable formats


private:
	HWAVEOUT m_hwo; //Used for actual audio output
	WAVEHDR  m_whdr;//Used for actual audio output
    WAVEFORMATEX  m_fmtWAV; //Format of default WAV
    unsigned long const m_SequenceLength; //aInstrumentNums.size() as passed to constructor
    CWaveFile m_myWavDefault; //Default WAV file used when a specific voice sample cannot be found

    std::vector<std::basic_string<TCHAR> > m_aUserVoices;

private:
	HANDLE m_hPowerReq;
    HWND m_hWnd; //Used to implement GetLastErr and beat blinking
    std::basic_string<TCHAR> m_strLastError; //Used to implement GetLastError
    unsigned long m_SampleToLoopDuration_bytes;
    bool m_bClip;

private:
	/* BHB - change next two to unsigned long 
    unsigned char m_BeatsPerBar;
    unsigned char m_nPlayTheFirst_n_BeatsInBarAtAltTempo;
	*/
	unsigned long m_BeatsPerBar;
	unsigned long m_nPlayTheFirst_n_BeatsInBarAtAltTempo;
	unsigned long m_AltBeatsPerMinute;
};

#endif // !defined(AFX_BeatBox_WAV_H__99EB1287_1D7C_4AE9_B2E0_4A0211302ED6__INCLUDED_)
