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
// metronomeDlg.cpp : implementation file
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef UNDER_CE
#include <aygshell.h>
#endif
#include <cmath> //log()
#include "resource.h"
#include "metronomeDlg.h"
#include "AboutDlg.h"
#include "general_midi.h"
#include "cBscFileVersion.h"
#include "math.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef USE_WEIRD_MIDI
#include "NewCore\BeatBox_MID.h"
#else
#include "NewCore\BeatBox_WAV.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                      Constants and Static Helpers
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Some functions to convert between the position of the slider and the value of m_BPMinute
static int round(double const & a) {return (int)(a + 0.5);}


//--------------------------------------------------------------------------------------------------
// I want an uneven scale on the tempo slider, so I'll use the log function
// to make it so that the low side of the scale has higher precision than
// the high side.  But using a straight log makes too much of a difference,
// so I need a log offset.  This'll make the difference in precision
// between the left and right sides of the slider to be less
static const int s_BPMSliderLogOffset = 128;
UINT CMetronomeDlg::SliderToBPM(UINT x) {return m_MinBPM + round(pow(2.0,((double)x)/(m_MaxBPM-m_MinBPM)))-s_BPMSliderLogOffset;}
UINT CMetronomeDlg::BPMToSlider(UINT x)
{                                                        //693.147... == LN_2/1000
    return round(log((double)((x - m_MinBPM) + s_BPMSliderLogOffset)) / (log(2.0)/(m_MaxBPM-m_MinBPM)));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Construction/ Destruction
//--------------------------------------------------------------------------------------------------
#ifndef LR_DEFAULTSIZE      
#define LR_DEFAULTSIZE 0x0040
#endif
CMetronomeDlg::CMetronomeDlg(HINSTANCE hInstance, HWND hParent) :
    CBscBblDlg(IDD_METRONOME_DIALOG, hInstance, hParent, false, true),

    m_hMenu(NULL),
    m_hComboEditChild(NULL),

    m_bInitBPMSlider(true),
    m_bWAVIsClipping(false),

    m_Playing(false),
    m_MetronomeStyle(metPlain),
    m_blinking(0),
    m_hIcon(LoadImage(m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON,0,0, LR_DEFAULTSIZE)),
    m_MinBPM(   0.0),
    m_MaxBPM(1000.0),
    m_IncBPM(  30.0),
    m_NumExp(    60),
    m_BPMeasure(0),
    m_BPMinute(0)
{
    m_BeatData       .resize(MAX_BPMEASURE);
    m_midi_instrument.resize(MAX_SOUNDS, 0);
    m_midi_volume    .resize(MAX_SOUNDS, 0);
    m_blink_size     .resize(MAX_SOUNDS, 0);

    //Message Map
    ADD_CMD_HANDLER(IDC_PLAY_BUTTON  , OnPlayButton  );
    ADD_CMD_HANDLER(IDC_RADIO_GROUP  , OnRadioGroup  );
    ADD_CMD_HANDLER(IDC_RADIO_MEASURE, OnRadioMeasure);
    ADD_CMD_HANDLER(IDC_RADIO_PLAIN  , OnRadioPlain  );
    ADD_CMD_HANDLER(IDC_BLINK_CHECK  , OnBlinkCheck  );
    ADD_CMD_HANDLER(IDM_ABOUTBOX         , OnMenuAbout);

    ADD_CMD_HANDLER(ID_SETTINGS_HOTKEYS  , OnMenuHotkeys);

    ADD_CMD_HANDLER(IDC_BPMINUTE_EDIT                           , OnCmdBPMIN     );

    ADD_CMD_HANDLER(IDC_GROUP_EDIT                              , OnCmdBPGRP     );
    ADD_CMD_HANDLER(IDC_BPMEASURE_EDIT                          , OnCmdBPMES     );
#ifdef UNDER_CE
    ADD_MSG_HANDLER(WM_SIZE                                     , OnSize);
#endif
    ADD_MSG_HANDLER(WM_CLOSE                                    , OnClose        );
    ADD_MSG_HANDLER(WM_HSCROLL                                  , OnHScroll      );
    ADD_MSG_HANDLER(UWM_BeatBox_BEAT_OCCURRED_wpBlinkSize_lpNULL, OnBeatOccurred );
    ADD_MSG_HANDLER(UWM_BeatBox_ERROR_OCCURRED_wpNULL_lpNULL    , OnErrorOccurred);
    ADD_MSG_HANDLER(WM_HOTKEY    , OnHotKey);
}
//--------------------------------------------------------------------------------------------------


CMetronomeDlg::~CMetronomeDlg()
{
    DestroyIcon((HICON)m_hIcon);
    DestroyMenu(m_hMenu);
    m_hIcon = NULL;
}
//--------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                  Message Handlers
//--------------------------------------------------------------------------------------------------
BOOL CALLBACK CMetronomeDlg::EnumChildProcCombo(HWND hChild, LPARAM lpThis)
{
    CMetronomeDlg* pThis = (CMetronomeDlg*) lpThis;
    pThis->m_hComboEditChild = hChild;
    return TRUE;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnInitDialog()
{
#ifndef UNDER_CE //Ce seems to require ONLY a maximize box, but we'll want a min box for regular windows
	SetWindowLong(m_hWnd, GWL_STYLE, (GetWindowLong(m_hWnd, GWL_STYLE)|WS_MINIMIZEBOX));
#endif
    CBscBblDlg::OnInitDialog();

#ifndef UNDER_CE //EnumChildWindows: does not exist in CE:/ Meh, I don't really care that much about this child window thing, it is only for if the user assignes "space" to a hotkey and then wants to type a space...
    //The combo has a child which is an editbox: don't want hotkeys if it has focus
    EnumChildWindows(GET_HWND(IDC_PRESET_COMBO), EnumChildProcCombo, (long)this);
#endif

    //Set title to include version number..
    std::basic_string<TCHAR> buf(WMET_APP_TITLE _T("              "));
    cBscFileVersion::Get((TCHAR*)(buf.c_str()+_tcslen(WMET_APP_TITLE)));
    m_AppTitle = buf; //Won't fit on Windows 7, let alone CE:( 
    SetWindowText(m_hWnd, WMET_APP_TITLE);

    m_strReduceVoiceVolume    .resize(256); LoadString(m_hInstance, IDS_REDUCE_VOICE_VOL   , &*m_strReduceVoiceVolume    .begin(), m_strReduceVoiceVolume    .length()); m_strReduceVoiceVolume    .resize(_tcslen(m_strReduceVoiceVolume    .c_str()));
    m_strMismatchedParentheses.resize(256); LoadString(m_hInstance, IDS_MISMATCHED_BRACKETS, &*m_strMismatchedParentheses.begin(), m_strMismatchedParentheses.length()); m_strMismatchedParentheses.resize(_tcslen(m_strMismatchedParentheses.c_str()));
    m_strMaxVoicesPerBeat     .resize(256); LoadString(m_hInstance, IDS_MAX_VOICES_PER_BEAT, &*m_strMaxVoicesPerBeat     .begin(), m_strMaxVoicesPerBeat     .length()); m_strMaxVoicesPerBeat     .resize(_tcslen(m_strMaxVoicesPerBeat     .c_str()));
    m_strAtLeastOneBeat       .resize(256); LoadString(m_hInstance, IDS_AT_LEAST_1_BEAT    , &*m_strAtLeastOneBeat       .begin(), m_strAtLeastOneBeat       .length()); m_strAtLeastOneBeat       .resize(_tcslen(m_strAtLeastOneBeat       .c_str()));
    m_strPlay                 .resize(256); LoadString(m_hInstance, IDS_PLAY, &*m_strPlay                 .begin(), m_strPlay                 .length()); m_strPlay                 .resize(_tcslen(m_strPlay                 .c_str()));
    m_strStop                 .resize(256); LoadString(m_hInstance, IDS_STOP, &*m_strStop                 .begin(), m_strStop                 .length()); m_strStop                 .resize(_tcslen(m_strStop                 .c_str()));

    //Our equivalent of DDX_Control...
    m_autopGroupEdit   =(std::auto_ptr<CGroupEdit >)new CGroupEdit (GET_HWND(IDC_GROUP_EDIT    ));
    m_autopBeatBlinker =(std::auto_ptr<CMetBlinker>)new CMetBlinker(1,GET_HWND(IDC_BEAT_INDICATOR));

    {for (unsigned long i = 0; i < MAX_SOUNDS; ++i)
    {
        m_autopBlinkerSliderDisplay[i] = (std::auto_ptr<CMetBlinker>) new 
            CMetBlinker(0, GET_HWND(IDC_BLINK_SIZE_1 + i ));
    }}

    {for(unsigned long i = 0; i < MAX_SOUNDS; i++)
    {
        m_SoundCombo    [i]= GET_HWND(IDC_TICK_COMBO_1+i);
        m_VolumeSlider  [i]= GET_HWND(IDC_TICK_VOLUME_SLIDER1+i);
        m_BlinkerSlider [i]= GET_HWND(IDC_BLINK_SIZE_SLIDER1+i);

        //Set volume sliders to go from 0 to 127 (the min and max velocity for a midi note)
        ::SendMessage(m_VolumeSlider[i], TBM_SETRANGE, TRUE, MAKELONG(0, 127));

        //Set blinker size sliders to go from 0 to (MAX_SOUNDS -1) (the number of size icons I have)
        ::SendMessage(m_BlinkerSlider[i], TBM_SETRANGE, TRUE, MAKELONG(0, MAX_SOUNDS - 1));
    }}

    //Sort out menu

    // Add "About..." menu item to system menu.
#ifdef UNDER_CE
    HMENU hSysMenu = NULL;//Don't want this in CE!  GetSystemMenu(m_hWnd, FALSE); //CE compiler won't let me use :: prefix!
    SHINITDLGINFO shido;//Make our dialog appear fullscreen without obscuring the softkey-bar
    ZeroMemory(&shido, sizeof(shido));
    shido.dwMask = SHIDIM_FLAGS;
    shido.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;// | SHIDIF_DONEBUTTON;

    shido.hDlg = m_hWnd;
    SHInitDialog(&shido);

    SHMENUBARINFO mbi; //The softkey menu bar is an ugly white surrounding the keypad icon unless we do this
    ZeroMemory(&mbi, sizeof(SHMENUBARINFO));

    mbi.cbSize     = sizeof(SHMENUBARINFO);
    mbi.hwndParent = m_hWnd;
    mbi.nToolBarId = IDR_MENUBAR; //http://msdn2.microsoft.com/en-us/library/aa453678.aspx http://msdn2.microsoft.com/en-us/library/ms838268.aspx
    mbi.hInstRes   = m_hInstance;
    mbi.dwFlags    = SHCMBF_HMENU;//SHCMBF_HIDESIPBUTTON;

    SHCreateMenuBar(&mbi);

#else
    HMENU hSysMenu = ::GetSystemMenu(m_hWnd, FALSE);

    m_hMenu = LoadMenu (m_hInstance, MAKEINTRESOURCE(IDR_MENUBAR));

    MENUITEMINFO mi;
    ZeroMemory(&mi, sizeof(mi));
    std::basic_string<TCHAR> strTmpMenu; strTmpMenu.resize(32);
    mi.cbSize = sizeof(mi);
    mi.fMask = MIIM_TYPE;
    mi.fType = MFT_STRING;
    mi.dwTypeData = &*strTmpMenu.begin();
    mi.cch = strTmpMenu.length();

    GetMenuItemInfo(m_hMenu, ID_SETTINGS_HOTKEYS, FALSE, &mi);
    strTmpMenu = _T("&") + strTmpMenu;
    mi.dwTypeData = (char*)strTmpMenu.c_str();
    SetMenuItemInfo(m_hMenu, ID_SETTINGS_HOTKEYS, FALSE, &mi);

    GetMenuItemInfo(m_hMenu, IDM_ABOUTBOX       , FALSE, &mi);
    strTmpMenu = _T("&") + strTmpMenu;
    mi.dwTypeData = (char*)strTmpMenu.c_str();
    SetMenuItemInfo(m_hMenu, IDM_ABOUTBOX, FALSE, &mi);

    SetMenu(m_hWnd, m_hMenu);
#endif

    //Set icon for this dialog.
    SendMessage(m_hWnd, WM_SETICON, ICON_BIG  , (LPARAM)m_hIcon);
    SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)m_hIcon);

    InitTickSoundLists();
    InitialiseToolTips();

    // Set the range for the BPMeasure spinner.
    ::SendMessage(GET_HWND(IDC_BPMEASURE_SPIN ), UDM_SETRANGE, 0, MAKELONG(MAX_BPMEASURE, 1));

    // Set the range for the BPMinute slider.  The value in the BPMinute edit box will be 
    //2^(<slider value> / m_MaxBPM).  Thus the edit box's value will range from 4 to m_MaxBPM (assuming 
    //a slider range of 2000 to 9966)
    ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETRANGEMIN, TRUE, BPMToSlider(m_MinBPM));
    ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETRANGEMAX, TRUE, BPMToSlider(m_MaxBPM));

    m_autopHotKeyDlg = (std::auto_ptr<CHotKeyDlg>) new CHotKeyDlg(m_hInstance, m_hWnd);

    return 1;  // return TRUE  unless you set the focus to a control
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnPlayButton(unsigned long const nIgnore1, long const nIgnore2) 
{
    if(!m_Playing)
        Play();
    else
        Stop();
    
    return 1;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnRadioGroup(unsigned long const nIgnore1, long const nIgnore2) 
{
    m_MetronomeStyle = metGroup;

    ::EnableWindow(GET_HWND(IDC_BPMEASURE_EDIT), FALSE);
    ::EnableWindow(GET_HWND(IDC_BPMEASURE_SPIN), FALSE);
    ::EnableWindow(GET_HWND(IDC_GROUP_EDIT    ), TRUE );
    
    OnChangeGroupEdit();

    return 1;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnRadioMeasure(unsigned long const nIgnore1, long const nIgnore2) 
{
    m_MetronomeStyle = metMeasure;

    ::EnableWindow(GET_HWND(IDC_BPMEASURE_EDIT), TRUE );
    ::EnableWindow(GET_HWND(IDC_BPMEASURE_SPIN), TRUE );
    ::EnableWindow(GET_HWND(IDC_GROUP_EDIT    ), FALSE);
    
    OnChangeBPMeasureEdit();

//    Already done in OnChangeBPMeasureEdit();    
//    if (m_Playing)
//        Play(); //Need to force this to restart now...

    return 1;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnRadioPlain(unsigned long const nIgnore1, long const nIgnore2) 
{
    m_MetronomeStyle = metPlain;

    ::EnableWindow(GET_HWND(IDC_BPMEASURE_EDIT), FALSE);
    ::EnableWindow(GET_HWND(IDC_BPMEASURE_SPIN), FALSE);
    ::EnableWindow(GET_HWND(IDC_GROUP_EDIT    ), FALSE);
    
    for(int i = 0; i < (int)m_BPMeasure; i++)
        m_BeatData[i].set_single_value(0);

    if (m_Playing)
        Play(); //Need to force this to restart now...

    return 1;
}
//--------------------------------------------------------------------------------------------------


void CMetronomeDlg::OnChangeGroupEdit() 
{
    // If we're currently Playing, Stop so that invalid strings don't screw things up
    if(m_Playing)
        Stop();
}
//--------------------------------------------------------------------------------------------------


void CMetronomeDlg::OnChangeBPMeasureEdit() 
{
    std::basic_string<TCHAR> strBuf = GetWindowText(IDC_BPMEASURE_EDIT);

    m_BPMeasure = _ttoi(strBuf.c_str());
    
    // initialize things so that it's Playing a measure with a strong first beat
    m_BeatData[0].set_single_value(1);
    for(int i = 1; i < (int)m_BPMeasure; i++)
        m_BeatData[i].set_single_value(0);

    if (m_Playing)
        Play();
}
//--------------------------------------------------------------------------------------------------


void CMetronomeDlg::OnChangeBPMinuteEdit() 
{
    std::basic_string<TCHAR> strBuf = GetWindowText(IDC_BPMINUTE_EDIT);

    unsigned long NewBPM = _ttoi(strBuf.c_str());
    
    if ((m_BPMinute != NewBPM) || m_bInitBPMSlider)
    {
        m_bInitBPMSlider = false;
        m_BPMinute = NewBPM;

        // Make sure that it's more than 3 so that we don't get an 
        // overflow on the number of microseconds per quarter note
        // setting in the midi output (we only have 24 bits to work 
        // with).

        if(m_BPMinute < 4)
            m_BPMinute = 4;
        else if(m_BPMinute > m_MaxBPM)
            m_BPMinute = m_MaxBPM;

        ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETPOS, TRUE, BPMToSlider(m_BPMinute));

        if(m_Playing)
        {
            if (m_autopTicker.get())
                m_autopTicker->SetTempo(m_BPMinute);
        }
    }
}
//--------------------------------------------------------------------------------------------------


void CMetronomeDlg::OnKillfocusBpminuteEdit()
{
    TCHAR bob[10];
    _stprintf(bob, _T("%d"), m_BPMinute);
        ::SetWindowText(GET_HWND(IDC_BPMINUTE_EDIT), bob);
}
//--------------------------------------------------------------------------------------------------


void CMetronomeDlg::OnSelectTickComboGeneric(WPARAM id, LPARAM hWnd) 
{
    long nIndex = ::SendMessage((HWND)hWnd, CB_GETCURSEL, 0, 0);

    if ((nIndex >= 0) && (nIndex < GM1_TRACK10_NUM_VOICES))
        m_midi_instrument[LOWORD(id)-IDC_TICK_COMBO_1] = AlphaToMidi[nIndex];
    else
        m_midi_instrument[LOWORD(id)-IDC_TICK_COMBO_1] = (nIndex-GM1_TRACK10_NUM_VOICES) + 65536;

    if (m_Playing)
    {
        if (m_autopTicker.get())
            m_autopTicker->SetInstrument(LOWORD(id)-IDC_TICK_COMBO_1, m_midi_instrument[LOWORD(id)-IDC_TICK_COMBO_1]);
    }
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnHScroll(unsigned long const nSBCodeAndnPos, long const hwndScrollBar) 
{
    bool unknown_slider = true;
    if(((HWND)hwndScrollBar) == GET_HWND(IDC_BPMINUTE_SLIDER))
    {
        TCHAR bob[10];
        int value = SliderToBPM(::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_GETPOS, 0, 0));
        _stprintf(bob, _T("%d"), value);
        ::SetWindowText(GET_HWND(IDC_BPMINUTE_EDIT), bob);
        unknown_slider = false;
    }
    else // check for volume sliders
    {
        {for(int i = 0; i < MAX_SOUNDS; i++)
        {
            if(((HWND)hwndScrollBar) == m_VolumeSlider[i])
            {
                m_midi_volume[i] = ::SendMessage(m_VolumeSlider[i], TBM_GETPOS, 0, 0);
                if (m_autopTicker.get() != NULL)
                    m_autopTicker->SetInstrumentVolume(i, m_midi_volume[i]);
                unknown_slider = false;
            }
            else
            {
                if(((HWND)hwndScrollBar) == m_BlinkerSlider[i])
                {
                    m_blink_size[i] = ::SendMessage(m_BlinkerSlider[i], TBM_GETPOS, 0, 0);
                    m_autopBlinkerSliderDisplay[i]->SetState(m_blink_size[i]);
                    unknown_slider = false;
                }
            }
        }}
    }

    // If this isn't one of our sliders, call the default OnHScroll()
    return ((unknown_slider)?0:1);
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnBlinkCheck(unsigned long const nIgnore1, long const nIgnore2) 
{
    m_blinking = (::SendMessage(GET_HWND(IDC_BLINK_CHECK), BM_GETCHECK, 0, 0) == BST_CHECKED);
    return 1;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnMenuHotkeys(unsigned long const nIgnore1, long const nIgnore2) 
{
    if (m_autopHotKeyDlg->DoModal() == IDOK)
    {
        long BPMPos = ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_GETPOS, 0, 0);
        long BPM = SliderToBPM(BPMPos);

        Stop();

        m_MinBPM = m_autopHotKeyDlg->GetMinBPM();
        m_MaxBPM = m_autopHotKeyDlg->GetMaxBPM();
        m_IncBPM = m_autopHotKeyDlg->GetIncBPM();
        m_NumExp = m_autopHotKeyDlg->GetNumExp();

        ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETRANGEMIN, TRUE, BPMToSlider(m_MinBPM));
        ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETRANGEMAX, TRUE, BPMToSlider(m_MaxBPM));

        BPMPos = BPMToSlider(BPM);
        ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETPOS, TRUE, BPMPos);
        OnHScroll(0, (long)GET_HWND(IDC_BPMINUTE_SLIDER));
    }
    
    return 1;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnMenuAbout(unsigned long const nIgnore1, long const nIgnore2) 
{
    CAboutDlg dlgAbout(m_hInstance, m_hWnd);
    dlgAbout.DoModal();
    
    return 1;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnHotKey(unsigned long const nHotKeyCmdMap, long const fuModifiersLO_uVirtKeyHI) 
{
    if ((GetFocus() != GET_HWND(IDC_GROUP_EDIT)) && (GetFocus() != m_hComboEditChild))
    {
        switch (nHotKeyCmdMap)
        {
        case CHotKeyDlg::e_TEMPO_DN:
            {
                long BPMPos = ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_GETPOS, 0, 0);
                long BPM = SliderToBPM(BPMPos);
                BPM -= m_IncBPM;
                if (BPM < m_MinBPM)
                    BPM = m_MinBPM;
                BPMPos = BPMToSlider(BPM);
                ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETPOS, TRUE, BPMPos);
                OnHScroll(0, (long)GET_HWND(IDC_BPMINUTE_SLIDER));
                break;
            }
        case CHotKeyDlg::e_TEMPO_UP:
            {
                long BPMPos = ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_GETPOS, 0, 0);
                long BPM = SliderToBPM(BPMPos);
                BPM += m_IncBPM;
                if (BPM > m_MaxBPM)
                    BPM = m_MaxBPM;
                BPMPos = BPMToSlider(BPM);
                ::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETPOS, TRUE, BPMPos);
                OnHScroll(0, (long)GET_HWND(IDC_BPMINUTE_SLIDER));
                break;
            }
        case CHotKeyDlg::e_PLAY     :{::PostMessage(m_hWnd, WM_COMMAND, IDC_PLAY_BUTTON  , 0); break;}
        case CHotKeyDlg::e_STRAIGHT :{::PostMessage(m_hWnd, WM_COMMAND, IDC_RADIO_PLAIN  , 0); ::SendMessage(GET_HWND(IDC_RADIO_PLAIN  ), BM_SETCHECK, BST_CHECKED  , 0); ::SendMessage(GET_HWND(IDC_RADIO_MEASURE), BM_SETCHECK, BST_UNCHECKED, 0); ::SendMessage(GET_HWND(IDC_RADIO_GROUP  ), BM_SETCHECK, BST_UNCHECKED, 0); break;}
        case CHotKeyDlg::e_SIMPLE   :{::PostMessage(m_hWnd, WM_COMMAND, IDC_RADIO_MEASURE, 0); ::SendMessage(GET_HWND(IDC_RADIO_PLAIN  ), BM_SETCHECK, BST_UNCHECKED, 0); ::SendMessage(GET_HWND(IDC_RADIO_MEASURE), BM_SETCHECK, BST_CHECKED  , 0); ::SendMessage(GET_HWND(IDC_RADIO_GROUP  ), BM_SETCHECK, BST_UNCHECKED, 0); break;}
    //###:    case CHotKeyDlg::e_TAP      {::PostMessage(m_hWnd, WM_COMMAND, , 0); break;}
        default:
            {
                break;
            }
        }
    }
    else
        PostMessage(GetFocus(), WM_KEYDOWN, HIWORD(fuModifiersLO_uVirtKeyHI), ((LOWORD(fuModifiersLO_uVirtKeyHI)&MOD_ALT)?1<<29:0));
    //else, don't want to do hotkeys when an editbox has focus

    return 1;
}
//--------------------------------------------------------------------------------------------------


#ifdef UNDER_CE //Catch the top-left "x" press, which in CE doesn't shut the window
long CMetronomeDlg::OnSize(unsigned long const nWhatIsGoingOn, long const nIgnore2) 
{
    if (nWhatIsGoingOn == SIZE_MINIMIZED)
    {
        PostMessage(m_hWnd, WM_CLOSE, 0, 0);
        PostMessage(m_hWnd, WM_QUIT, 0, 0);
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
#endif


long CMetronomeDlg::OnOK() 
{
#ifdef UNDER_CE
    PostMessage(m_hWnd, WM_CLOSE, 0, 0); //In WCE we want to terminate the app if they press escape/ choose cancel
    PostMessage(m_hWnd, WM_QUIT, 0, 0); //In WCE we want to terminate the app if they press the "x" (which == OK)
    return 0;
#else
    OnPlayButton(0,0);
    return 1;
#endif
}
//--------------------------------------------------------------------------------------------------


// Do nothing if the user presses escape
long CMetronomeDlg::OnCancel( )
{
#ifdef UNDER_CE
    PostMessage(m_hWnd, WM_CLOSE, 0, 0); //In WCE we want to terminate the app if they press escape/ choose cancel
    PostMessage(m_hWnd, WM_QUIT, 0, 0); //In WCE we want to terminate the app if they press the "x" (which == OK)
    return 0;
#else
    return 1;
#endif
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnClose(unsigned long const nIgnore1, long const nIgnore2) 
{
    m_autopTicker = (std::auto_ptr<IBeatBox>) NULL; //Kill ticker before proceeding to close window

    m_autopGroupEdit               = (std::auto_ptr<CGroupEdit >) NULL;
    m_autopBeatBlinker             = (std::auto_ptr<CMetBlinker>) NULL;
    m_autopBlinkerSliderDisplay[0] = (std::auto_ptr<CMetBlinker>) NULL;
    m_autopBlinkerSliderDisplay[1] = (std::auto_ptr<CMetBlinker>) NULL;
    m_autopBlinkerSliderDisplay[2] = (std::auto_ptr<CMetBlinker>) NULL;
    m_autopBlinkerSliderDisplay[3] = (std::auto_ptr<CMetBlinker>) NULL;
    m_autopBlinkerSliderDisplay[4] = (std::auto_ptr<CMetBlinker>) NULL;
    m_autopBlinkerSliderDisplay[5] = (std::auto_ptr<CMetBlinker>) NULL;
    m_autopBlinkerSliderDisplay[6] = (std::auto_ptr<CMetBlinker>) NULL;
    m_autopBlinkerSliderDisplay[7] = (std::auto_ptr<CMetBlinker>) NULL;
    m_autopBlinkerSliderDisplay[8] = (std::auto_ptr<CMetBlinker>) NULL;

    DestroyWindow(m_hWnd);
#ifdef UNDER_CE
    return 0;
#else
    return 1;
#endif
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnBeatOccurred(unsigned long const wpBlinkSize, long const bWAVIsClipping)
{
    double const BPS = (((double)m_BPMinute)/60.0);
    double const BeatEveryThisMany_ms = m_MaxBPM/BPS;
    if (m_blinking)
    {
        m_autopBeatBlinker->Beat((const unsigned long)
            (BeatEveryThisMany_ms+0.5), (CMetBlinker::BLINKER_STATES)wpBlinkSize);
    }

    if (m_bWAVIsClipping != (bWAVIsClipping?true:false))
    {
        m_bWAVIsClipping = bWAVIsClipping?true:false;
        if (m_bWAVIsClipping)
            SetWindowText(m_hWnd, m_strReduceVoiceVolume.c_str());
        else
            SetWindowText(m_hWnd, WMET_APP_TITLE);
    }

    return 1;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnErrorOccurred(unsigned long const nIgnore1, long const nIgnore2)
{
    if (m_autopTicker.get() != NULL)
        MessageBox(m_hWnd,m_autopTicker->GetLastErr().c_str(),NULL,MB_ICONEXCLAMATION|MB_OK);
    return 1;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnCommand(unsigned short const CtrlID, unsigned short const HiWordWparam, 
                                                           LPARAM         const lParam)
{
    long bRet = CBscBblDlg::OnCommand(CtrlID, HiWordWparam, lParam);
    if ((HiWordWparam == CBN_SELCHANGE) && (CtrlID >= IDC_TICK_COMBO_1) && (CtrlID <= IDC_TICK_COMBO_9))
        OnSelectTickComboGeneric((HiWordWparam<<16)|CtrlID, lParam);
    return bRet;
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnCmdBPMIN(unsigned long const idCmdMsg, long const Whatever)
{
    if  (idCmdMsg == EN_KILLFOCUS)
        OnKillfocusBpminuteEdit ();
    if  (idCmdMsg == EN_CHANGE)
        OnChangeBPMinuteEdit    ();

    return 0; //Ensure default action is taken...
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnCmdBPGRP(unsigned long const idCmdMsg, long const Whatever)
{
    if  (idCmdMsg == EN_CHANGE)
        OnChangeGroupEdit();

    return 0; //Ensure default action is taken...
}
//--------------------------------------------------------------------------------------------------


long CMetronomeDlg::OnCmdBPMES(unsigned long const idCmdMsg, long const Whatever)
{
    if  (idCmdMsg == EN_CHANGE)
        OnChangeBPMeasureEdit();

    return 0; //Ensure default action is taken...
}
//--------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                  Helper Functions
//--------------------------------------------------------------------------------------------------


void CMetronomeDlg::InitTickSoundLists()
{
    std::vector<std::basic_string<TCHAR> > aUserVoices;
#ifndef USE_WEIRD_MIDI
    CBeatBox_WAV::GetUserVoices(&aUserVoices);
#endif

    std::basic_string<TCHAR> buf;
    {for(unsigned long i = 0; i < GM1_TRACK10_NUM_VOICES; i++)
    {
        buf.resize(256); LoadString(m_hInstance, IDS_MIDI_ALPHA_BASE + i, &*buf.begin(), buf.length()); buf.resize(_tcslen(buf.c_str()));
        {for(unsigned long j = 0; j < MAX_SOUNDS; j++)
        {
            ::SendMessage(m_SoundCombo[j], CB_ADDSTRING, 0, (long)buf.c_str());
        }}
    }}

    {for(unsigned long i = 0; i < aUserVoices.size(); i++)
    {
        {for(unsigned long j = 0; j < MAX_SOUNDS; j++)
        {
            ::SendMessage(m_SoundCombo[j], CB_ADDSTRING, 0, (long)aUserVoices[i].c_str());
        }}
    }}
}
//--------------------------------------------------------------------------------------------------


void CMetronomeDlg::InitialiseToolTips()
{
    m_autopTTRADIO_PLAIN     = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_RADIO_PLAIN    , m_hWnd, m_hInstance, IDS_TIP_RADIO_PLAIN    );
    m_autopTTRADIO_MEASURE   = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_RADIO_MEASURE  , m_hWnd, m_hInstance, IDS_TIP_RADIO_MEASURE  );
    m_autopTTRADIO_GROUP     = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_RADIO_GROUP    , m_hWnd, m_hInstance, IDS_TIP_RADIO_GROUP    );
    m_autopTTGROUP_EDIT      = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_GROUP_EDIT     , m_hWnd, m_hInstance, 
#ifdef USE_WEIRD_MIDI
        IDS_TIP_GROUP_EDIT        );
#else
        IDS_TIP_GROUP_EDIT_EXT_WAV);
#endif
    m_autopTTBPMEASURE_EDIT  = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_BPMEASURE_EDIT , m_hWnd, m_hInstance, IDS_TIP_BPMEASURE_EDIT );
    m_autopTTPLAY_BUTTON     = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_PLAY_BUTTON    , m_hWnd, m_hInstance, IDS_TIP_StartString);
    m_autopTTBPMINUTE_SLIDER = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_BPMINUTE_SLIDER, m_hWnd, m_hInstance, IDS_TIP_BPMINUTE_SLIDER);
    m_autopTTBPMINUTE_EDIT   = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_BPMINUTE_EDIT  , m_hWnd, m_hInstance, IDS_TIP_BPMINUTE_EDIT  );
    m_autopTTBLINK_CHECK     = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_BLINK_CHECK    , m_hWnd, m_hInstance, IDS_TIP_BLINK_CHECK    );

    {for(unsigned long i = 0; i < MAX_SOUNDS; i++)
    {
        m_autopTTTICK_VOLUME_SLIDERs[i] = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_TICK_VOLUME_SLIDER1 + i, m_hWnd, m_hInstance, IDS_TIP_VolumeSlider  );
        m_autopTTBLINK_SIZE_SLIDERs [i] = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_BLINK_SIZE_SLIDER1  + i, m_hWnd, m_hInstance, IDS_TIP_BlinkerSlider );
        m_autopTTBLINK_SIZEs        [i] = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_BLINK_SIZE_1        + i, m_hWnd, m_hInstance, IDS_TIP_BlinkerSliderD);
        m_autopTTTICK_COMBOs        [i] = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_TICK_COMBO_1        + i, m_hWnd, m_hInstance, IDS_TIP_SoundCombo    );
    }}
}
//--------------------------------------------------------------------------------------------------


bool CMetronomeDlg::BuildPlayerInstance()
{
    CGroupEdit::analyse_groups_error ag_error;
    
    // If we're in group mode, make sure the goup edit box has a valid string
    std::basic_string<TCHAR> strAdvancedCfg;
    if (::SendMessage(GET_HWND(IDC_RADIO_GROUP), BM_GETCHECK, 0, 0) == BST_CHECKED)
    {
        if ((ag_error = m_autopGroupEdit->Parse(&m_BeatData, &m_BPMeasure, &strAdvancedCfg)) != CGroupEdit::ag_success)
        {
            switch(ag_error)
            {
            case CGroupEdit::ag_misplaced_paren:
            case CGroupEdit::ag_unclosed_paren:
                MessageBox(m_hWnd,
                    m_strMismatchedParentheses.c_str(),
                    NULL, MB_ICONEXCLAMATION|MB_OK);
                break;

            case CGroupEdit::ag_substring_overrun:
                MessageBox(m_hWnd,m_strMaxVoicesPerBeat.c_str(), NULL, MB_ICONEXCLAMATION|MB_OK);
                break;

            case CGroupEdit::ag_no_beats:
                MessageBox(m_hWnd,
                    m_strAtLeastOneBeat.c_str(),
                    NULL, MB_ICONEXCLAMATION|MB_OK);
                break;
            }

            return false;
        }
    }

    unsigned long nLocInMeasure = 0;

    std::vector<std::vector<long                     > > alInstrumentNums;
    std::vector<std::vector<long                     > > alVolumes;
    std::vector<std::vector<long                     > > alBeatSizes;

    for (unsigned long iBeatQueueLoop = 0; iBeatQueueLoop < m_BPMeasure; ++iBeatQueueLoop)
    {
        if(m_BeatData[nLocInMeasure].is_single_value())
        {
            int const instrument_num = m_BeatData[nLocInMeasure].get_single_value();

            std::vector<long                     > atmplVolumes  ;
            std::vector<long                     > atmplBeatSizes;
            std::vector<long                     > atmplInstrumentNums;

            if (instrument_num >= 0)
            {
                atmplVolumes  .push_back(m_midi_volume[instrument_num]             );
                atmplBeatSizes.push_back(m_blink_size[instrument_num]              );
                atmplInstrumentNums.push_back(instrument_num);
            }
            else
            {
                atmplVolumes  .push_back(0);
                atmplBeatSizes.push_back(0);
                atmplInstrumentNums.push_back(-1);
            }

            alVolumes  .push_back(atmplVolumes  );
            alInstrumentNums  .push_back(atmplInstrumentNums);
            alBeatSizes.push_back(atmplBeatSizes);

        }
        else 
        {//!!!This if-else is a bit wierd, you'd think it could be done with a single block of code.
            int multi_length = m_BeatData[nLocInMeasure].get_multiple_length();
            std::vector<long                     > atmplVolumes  ;
            std::vector<long                     > atmplBeatSizes;
            std::vector<long                     > atmplInstrumentNums;
            for(int i = 0; i < multi_length; i++)
            {
                int const instrument_num = m_BeatData[nLocInMeasure].get_multiple_value(i);
                //std::basic_string<TCHAR> const strInstrument = (instrument_num >= 0)?
                //    AlphaInstrumentStrings[MidiToAlpha[m _midi_instrument[instrument_num]]]:_T("");

                if (instrument_num >= 0)
                {
                    atmplVolumes  .push_back(m_midi_volume[instrument_num]             );
                    atmplBeatSizes.push_back(m_blink_size[instrument_num]              );
                    atmplInstrumentNums.push_back(instrument_num);
                }
                else
                {
                    atmplVolumes  .push_back(0);
                    atmplBeatSizes.push_back(0);
                    atmplInstrumentNums.push_back(-1);
                }
            }
            alVolumes  .push_back(atmplVolumes  );
            alInstrumentNums  .push_back(atmplInstrumentNums);
            alBeatSizes.push_back(atmplBeatSizes);
        }
        ++nLocInMeasure;
    }
    nLocInMeasure = 0;

    m_autopTicker = (std::auto_ptr<IBeatBox>)NULL;
    if (alInstrumentNums.size())
    {
#ifdef USE_WEIRD_MIDI
    m_autopTicker = (std::auto_ptr<IBeatBox>) new CBeatBox_MID(
#else //USE_WEIRD_WAV


    unsigned short BeatsPerBar = 0;
    unsigned short nPlayTheFirst_n_BeatsInBarAtAltTempo = 0;
    long AltBeatsPerMinute = 0;
    if (strAdvancedCfg.length() > 2)
    {
        const TCHAR * pTerm = strAdvancedCfg.c_str();
        nPlayTheFirst_n_BeatsInBarAtAltTempo = _tcstoul(++pTerm, (TCHAR**)&pTerm, 10);
        BeatsPerBar = _tcstoul(++pTerm, (TCHAR**)&pTerm, 10);
        AltBeatsPerMinute = _tcstoul(++pTerm, (TCHAR**)&pTerm, 10);
        if (strAdvancedCfg.find(_T('%')) != std::string::npos)
            AltBeatsPerMinute = m_BPMinute+(AltBeatsPerMinute*(m_BPMinute/100.0));
        AltBeatsPerMinute = abs(AltBeatsPerMinute);
    }

    m_autopTicker = (std::auto_ptr<IBeatBox>) new CBeatBox_WAV(
#endif
        alInstrumentNums, m_midi_instrument, m_midi_volume, m_blink_size, m_BPMinute, 
#ifndef USE_WEIRD_MIDI
        BeatsPerBar,
        nPlayTheFirst_n_BeatsInBarAtAltTempo,
        AltBeatsPerMinute,
#endif
        m_hWnd);
    }

    return true;
}
//--------------------------------------------------------------------------------------------------


bool CMetronomeDlg::Play() 
{
    bool bRet = BuildPlayerInstance();
    if (!bRet) return bRet;

    // update the tooltip
    m_autopTTPLAY_BUTTON     = (std::auto_ptr<CBscToolTip>) NULL;
    m_autopTTPLAY_BUTTON     = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_PLAY_BUTTON    , m_hWnd, m_hInstance, IDS_TIP_StopString);

    // And, finally, set this behemoth Playing!
    if (m_autopTicker.get() != NULL)
    {
        m_autopTicker->Play();

        m_Playing = true;
        ::SetWindowText(GET_HWND(IDC_PLAY_BUTTON), m_strStop.c_str());
    }

    return true;
}
//--------------------------------------------------------------------------------------------------


void CMetronomeDlg::Stop() 
{
    if (m_autopTicker.get())
        m_autopTicker->Stop();

    m_Playing = false;
    ::SetWindowText(GET_HWND(IDC_PLAY_BUTTON), m_strPlay.c_str());
    ::SetWindowText(m_hWnd, WMET_APP_TITLE);
    m_bWAVIsClipping = false;
    
    m_autopBeatBlinker->SetState(CMetBlinker::bsSmallest); // Turn off the blinker

    // update the tooltip for the Play button
    m_autopTTPLAY_BUTTON     = (std::auto_ptr<CBscToolTip>) NULL;
    m_autopTTPLAY_BUTTON     = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_PLAY_BUTTON    , m_hWnd, m_hInstance, IDS_TIP_StartString);
}
//--------------------------------------------------------------------------------------------------
