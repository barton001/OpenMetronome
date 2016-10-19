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
#if !defined(AFX_METRONOMEDLG_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
#define AFX_METRONOMEDLG_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_
// metronomeDlg.h : header file
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define MAX_SOUNDS    9   // this can't be any more than 9; it matches whatever is defined in the rc

#include <memory>
#include <mmsystem.h>
#include "resource.h"
#include "MetDefs.h"
#include "group_beat.h"
#include "NewCore\IBeatBox.h"
#include "MetBlinker.h"
#include "GroupEdit.h"
#include "BscToolTip.h"
#include "HotKeyDlg.h"
#include "BscBblDlg.h"


//Implements "Back-end" of IDD_METRONOME_DIALOG (see project's .rc file)
//Hence handles everything to do with the user-setup of the audio and blink-boxes to be output by
//the metronome.
//Does not implement the "load and save to registry" buttons (relegated to derived class)
//Does not handle drag-resize.
//Usage: See CBscBblDlg
class CMetronomeDlg : public CBscBblDlg
{
protected: //Member variables
    bool m_Playing;
	unsigned long m_blinking; //Set by check-box on-screen

    enum METRONOME_STYLES{metPlain, metMeasure, metGroup};
	METRONOME_STYLES m_MetronomeStyle;

/* BHB change from doubles to unsigned long
    double m_MaxBPM;
    double m_MinBPM;
    double m_IncBPM;
	*/

	unsigned long m_MaxBPM;
	unsigned long m_MinBPM;
	unsigned long m_IncBPM;

    unsigned long m_NumExp;
    unsigned long m_BPMeasure;
	unsigned long m_BPMinute ;
    std::vector<group_beat>    m_BeatData;

    //-1 for silence, or use AlphaInstrumentStrings[MidiToAlpha[m_midi_instrument[instrument_num]]] 
    //to get string description of MIDI instrument associated with Midi-voice dropdown
    //(indexed top-to-bottom)
    std::vector<int> m_midi_instrument;

    std::vector<int> m_midi_volume; //Integer values from volume sliders (indexed top-to-bottom) 
	std::vector<int> m_blink_size;  //Integer values from blink-size sliders (indexed top-to-bottom) 

    // The thread object that performs the actual ticking
    std::auto_ptr<IBeatBox> m_autopTicker;

	HANDLE m_hIcon;
    virtual void InitialiseToolTips();
	bool Play();	// BHB

private:
	void InitTickSoundLists(); //Initialises the drop-down boxes used to select midi voices

	// bool Play();	// BHB - move to protected to allow ProcessCmdLine() to start the metronome
	void Stop();

	bool ErrMsgBox(UINT stringTableID);


public:
    //Construction: See base-class documentation
	CMetronomeDlg(HINSTANCE hInstance = 0, //E.g., (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE)
                  HWND      hParent = 0);
    virtual ~CMetronomeDlg();

protected:
	HWND m_SoundCombo   [MAX_SOUNDS]; //HWNDs to the midi voice dropdowns (indexed top-to-bottom)
	HWND m_VolumeSlider [MAX_SOUNDS]; //HWNDs to the volume sliders (indexed top-to-bottom)
	HWND m_BlinkerSlider[MAX_SOUNDS]; //HWNDs to the blink-size sliders (indexed top-to-bottom)

    //Blink-size-indicators (indexed top-to-bottom)
	std::auto_ptr<CMetBlinker> m_autopBlinkerSliderDisplay[MAX_SOUNDS]; 
    std::auto_ptr<CMetBlinker> m_autopBeatBlinker; //The beat-blinker instance
    std::auto_ptr<CGroupEdit > m_autopGroupEdit;   //Edit box used to for custom beat patterns
	

protected:
    //Overridden to call OnSelectTickComboGeneric when reqd
    virtual long OnCommand(unsigned short const CtrlID, 
                           unsigned short const HiWordWparam, LPARAM const lParam);

    // to make sure nothing happens when the user presses <esc>
	virtual long OnCancel();
    virtual long OnOK();
    virtual long OnInitDialog(); //Overridden to initialise the dialog properly


protected: //Message handlers: see also base-class documentation
    DECLARE_MSG_HANDLER(OnClose        , CMetronomeDlg); 
    DECLARE_MSG_HANDLER(OnHScroll      , CMetronomeDlg); //Handles slider messages
    DECLARE_MSG_HANDLER(OnBeatOccurred , CMetronomeDlg); //Blinks the main blinker (if m_blinking)

    //Displays error message to user: used by BeatBox instance to say, e.g., "No MIDI device found"
    DECLARE_MSG_HANDLER(OnErrorOccurred, CMetronomeDlg); 

    DECLARE_MSG_HANDLER(OnCmdBPMIN, CMetronomeDlg); //Handles changes to Beat-per-minute edit-box
    DECLARE_MSG_HANDLER(OnCmdBPGRP, CMetronomeDlg); //Handles changes to custom-beat edit-box
    DECLARE_MSG_HANDLER(OnCmdBPMES, CMetronomeDlg); //Handles changes to beats-per-measure edit-box

    DECLARE_MSG_HANDLER(OnPlayButton   , CMetronomeDlg);
    DECLARE_MSG_HANDLER(OnRadioPlain   , CMetronomeDlg);
    DECLARE_MSG_HANDLER(OnRadioMeasure , CMetronomeDlg);
    DECLARE_MSG_HANDLER(OnRadioGroup   , CMetronomeDlg);
    DECLARE_MSG_HANDLER(OnBlinkCheck   , CMetronomeDlg);

    DECLARE_MSG_HANDLER(OnMenuHotkeys  , CMetronomeDlg);
    DECLARE_MSG_HANDLER(OnMenuAbout    , CMetronomeDlg);

    DECLARE_MSG_HANDLER(OnHotKey   , CMetronomeDlg); //Enable or disable blinking

#ifdef UNDER_CE
    DECLARE_MSG_HANDLER(OnSize   , CMetronomeDlg);
#endif
    virtual void OnChangeGroupEdit();
	virtual void OnChangeBPMeasureEdit();
	virtual void OnChangeBPMinuteEdit();
	virtual void OnKillfocusBpminuteEdit();
	virtual void OnSelectTickComboGeneric(WPARAM id, LPARAM hWnd); //OnChange for voice dropdowns


    virtual UINT SliderToBPM(UINT x);
    virtual UINT BPMToSlider(UINT x);


    HWND m_hComboEditChild;
    std::auto_ptr<CHotKeyDlg> m_autopHotKeyDlg;


    virtual bool BuildPlayerInstance();


private: //Tooltips: You don't need to do anything with these, they just need to exist!
    std::auto_ptr<CBscToolTip> m_autopTTRADIO_PLAIN    ;
    std::auto_ptr<CBscToolTip> m_autopTTRADIO_MEASURE  ;
    std::auto_ptr<CBscToolTip> m_autopTTRADIO_GROUP    ;
    std::auto_ptr<CBscToolTip> m_autopTTGROUP_EDIT     ;
    std::auto_ptr<CBscToolTip> m_autopTTBPMEASURE_EDIT ;
    std::auto_ptr<CBscToolTip> m_autopTTPLAY_BUTTON    ;
    std::auto_ptr<CBscToolTip> m_autopTTBPMINUTE_SLIDER;
    std::auto_ptr<CBscToolTip> m_autopTTBPMINUTE_EDIT  ;
    std::auto_ptr<CBscToolTip> m_autopTTBLINK_CHECK    ;
    std::auto_ptr<CBscToolTip> m_autopTTTICK_VOLUME_SLIDERs[MAX_SOUNDS];
    std::auto_ptr<CBscToolTip> m_autopTTBLINK_SIZE_SLIDERs [MAX_SOUNDS]; 
    std::auto_ptr<CBscToolTip> m_autopTTBLINK_SIZEs        [MAX_SOUNDS];       
    std::auto_ptr<CBscToolTip> m_autopTTTICK_COMBOs        [MAX_SOUNDS];
    bool m_bInitBPMSlider;
    bool m_bWAVIsClipping;
    std::basic_string<TCHAR> m_AppTitle;

    HMENU m_hMenu;

    std::basic_string<TCHAR> m_strReduceVoiceVolume;
    std::basic_string<TCHAR> m_strMismatchedParentheses;
    std::basic_string<TCHAR> m_strMaxVoicesPerBeat;
    std::basic_string<TCHAR> m_strAtLeastOneBeat;
    std::basic_string<TCHAR> m_strPlay;
    std::basic_string<TCHAR> m_strStop;

    static BOOL CALLBACK EnumChildProcCombo(HWND hChild, LPARAM lpThis);
};
#endif // !defined(AFX_METRONOMEDLG_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
