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
// MetronomeDlg_RegPersist.cpp : implementation file
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShellAPI.h" //ShellExecute, requires shell32.lib
#include "commdlg.h"  //GetOpenFileName
#include "general_midi.h"
#include "MetronomeDlg_RegPersist.h"

// BHB - Begin: needed for ProcessCmdLine()
#include "ProcessCmdLine.h"
// BHB - End: needed for ProcessCmdLine()


#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//--------------------------------------------------------------------------------------------------
//!!!All of the registry keys should be defined here, to prevent the "Load" function accidentally
//using a different key-name to the save function (e.g., via a typo)
static TCHAR const s_WeirdMetRegKeyRoot[] = _T("Software\\BHBSoftware\\Open Metronome");
// BHB - Replace with my instrument selections - was set to {GM1_SIDE_STICK,GM1_BASS_DRUM_1,GM1_LOW_WOOD_BLOCK};
long  const CMetronomeDlg_RegPersist::s_DefaultInstruments[MAX_SOUNDS] =                                    
	{ GM1_LOW_WOOD_BLOCK,GM1_HI_WOOD_BLOCK,GM1_CLOSED_HI_HAT,GM1_ACOUSTIC_SNARE,GM1_BASS_DRUM_1,GM1_SPLASH_CYMBAL };


////////////////////////////////////////////////////////////////////////////////////////////////////
// CMetronomeDlg_RegPersist dialog
CMetronomeDlg_RegPersist::CMetronomeDlg_RegPersist(HINSTANCE hInstance, HWND hParent) : 
    CMetronomeDlg(hInstance, hParent)
{
    ADD_CMD_HANDLER(IDC_SAVE_PRESET_BUTTON  , OnSavePresetButton  );
    ADD_CMD_HANDLER(IDC_DELETE_PRESET_BUTTON, OnDeletePresetButton);
    ADD_CMD_HANDLER(IDC_EXPORT_PRESET_BUTTON, OnExportPresetButton);
    ADD_CMD_HANDLER(IDC_PRESET_COMBO        , OnCmdPresetCombo    );
}


/////////////////////////////////////////////////////////////////////////////
// CMetronomeDlg_RegPersist message handlers


static HKEY OpenKey(LPCTSTR preset_name, bool const bWriteElseRead)
{
    HKEY hkRet = NULL;

    HKEY  hkey = 0;
    DWORD dwDisposition = 0;

    std::basic_string<TCHAR> key_name(s_WeirdMetRegKeyRoot);
    if(preset_name != NULL)
    {
        key_name += _T("\\");
        key_name += preset_name;
    }

    RegCreateKeyEx(HKEY_CURRENT_USER, key_name.c_str(), 0, NULL, 0, 
        bWriteElseRead?KEY_WRITE:KEY_READ, NULL, &hkRet, &dwDisposition);

    return hkRet;
}


long CMetronomeDlg_RegPersist::OnInitDialog()
{
    HKEY hkey = OpenKey(NULL, false);

    if(hkey)
    {
        // Load the max beats per minute; need to do before oninitdialog!
        DWORD    dwType, dwSize;
        dwType = REG_DWORD;
        UINT BPM = 1000;
        dwSize = sizeof(BPM);
        if (ERROR_SUCCESS == RegQueryValueEx(hkey, _T("MinBPMinute"), 0, &dwType, (PBYTE)&BPM, &dwSize))
            m_MinBPM = BPM;
        if (ERROR_SUCCESS == RegQueryValueEx(hkey, _T("MaxBPMinute"), 0, &dwType, (PBYTE)&BPM, &dwSize))
            m_MaxBPM = BPM;
        if (ERROR_SUCCESS == RegQueryValueEx(hkey, _T("IncBPMinute"), 0, &dwType, (PBYTE)&BPM, &dwSize))
            m_IncBPM = BPM;
        if (ERROR_SUCCESS == RegQueryValueEx(hkey, _T("NumWavLoopsToExport"), 0, &dwType, (PBYTE)&BPM, &dwSize))
            m_NumExp = BPM;
        RegCloseKey(hkey);
    }
#ifdef UNDER_CE
    EnableWindow(GET_HWND(IDC_EXPORT_PRESET_BUTTON), FALSE); //No regedit app in WinCE
#endif

    CMetronomeDlg::OnInitDialog();

    m_strPresetOverwrite    .resize(256); LoadString(m_hInstance, IDS_PRESET_OVERWRITE     , &*m_strPresetOverwrite    .begin(), m_strPresetOverwrite    .length()); m_strPresetOverwrite    .resize(_tcslen(m_strPresetOverwrite    .c_str()));
    m_strPresetAlreadyExists.resize(256); LoadString(m_hInstance, IDS_PRESET_ALREADY_EXISTS, &*m_strPresetAlreadyExists.begin(), m_strPresetAlreadyExists.length()); m_strPresetAlreadyExists.resize(_tcslen(m_strPresetAlreadyExists.c_str()));
    m_strPresetSaveError    .resize(256); LoadString(m_hInstance, IDS_PRESET_SAVE_FAIL     , &*m_strPresetSaveError    .begin(), m_strPresetSaveError    .length()); m_strPresetSaveError    .resize(_tcslen(m_strPresetSaveError    .c_str()));

    m_autopHotKeyDlg->SetBPMThesholds(m_MinBPM, m_MaxBPM, m_IncBPM, m_NumExp);

    LoadSettings();

    return 1;  // return TRUE  unless you set the focus to a control
}


bool CMetronomeDlg_RegPersist::SaveSettings(LPCTSTR preset_name)
{
    bool bRet = false;

    HKEY hkey = OpenKey(preset_name, true);

    if(hkey)
    {
        RegSetValueEx(hkey, _T("BPMinute"), 0, REG_DWORD, (PBYTE)&m_BPMinute, sizeof(m_BPMinute));

        DWORD dVal = 0;

		// BHB - Don't save values from the hotkeys dialog with each preset; I think there should be a single global value set
		if (preset_name == NULL) {

			dVal = m_MinBPM; RegSetValueEx(hkey, _T("MinBPMinute"), 0, REG_DWORD, (PBYTE)&dVal, sizeof(dVal));
			dVal = m_MaxBPM; RegSetValueEx(hkey, _T("MaxBPMinute"), 0, REG_DWORD, (PBYTE)&dVal, sizeof(dVal));
			dVal = m_IncBPM; RegSetValueEx(hkey, _T("IncBPMinute"), 0, REG_DWORD, (PBYTE)&dVal, sizeof(dVal));
			dVal = m_NumExp; RegSetValueEx(hkey, _T("NumWavLoopsToExport"), 0, REG_DWORD, (PBYTE)&dVal, sizeof(dVal));

			unsigned long const HotKeyVKeyTEMPO_UP = m_autopHotKeyDlg->GetHotKeyVKeyTEMPO_UP(); unsigned long const HotKeyFlagTEMPO_UP = m_autopHotKeyDlg->GetHotKeyFlagTEMPO_UP();
			unsigned long const HotKeyVKeyTEMPO_DN = m_autopHotKeyDlg->GetHotKeyVKeyTEMPO_DN(); unsigned long const HotKeyFlagTEMPO_DN = m_autopHotKeyDlg->GetHotKeyFlagTEMPO_DN();
			unsigned long const HotKeyVKeyPLAY = m_autopHotKeyDlg->GetHotKeyVKeyPLAY(); unsigned long const HotKeyFlagPLAY = m_autopHotKeyDlg->GetHotKeyFlagPLAY();
			unsigned long const HotKeyVKeySTRAIGHT = m_autopHotKeyDlg->GetHotKeyVKeySTRAIGHT(); unsigned long const HotKeyFlagSTRAIGHT = m_autopHotKeyDlg->GetHotKeyFlagSTRAIGHT();
			unsigned long const HotKeyVKeySIMPLE = m_autopHotKeyDlg->GetHotKeyVKeySIMPLE(); unsigned long const HotKeyFlagSIMPLE = m_autopHotKeyDlg->GetHotKeyFlagSIMPLE();
			unsigned long const HotKeyVKeyTAP = m_autopHotKeyDlg->GetHotKeyVKeyTAP(); unsigned long const HotKeyFlagTAP = m_autopHotKeyDlg->GetHotKeyFlagTAP();

			RegSetValueEx(hkey, _T("HotKeyVKeyTEMPO_UP"), 0, REG_DWORD, (PBYTE)&HotKeyVKeyTEMPO_UP, sizeof(HotKeyVKeyTEMPO_UP)); RegSetValueEx(hkey, _T("HotKeyFlagTEMPO_UP"), 0, REG_DWORD, (PBYTE)&HotKeyFlagTEMPO_UP, sizeof(HotKeyFlagTEMPO_UP));
			RegSetValueEx(hkey, _T("HotKeyVKeyTEMPO_DN"), 0, REG_DWORD, (PBYTE)&HotKeyVKeyTEMPO_DN, sizeof(HotKeyVKeyTEMPO_DN)); RegSetValueEx(hkey, _T("HotKeyFlagTEMPO_DN"), 0, REG_DWORD, (PBYTE)&HotKeyFlagTEMPO_DN, sizeof(HotKeyFlagTEMPO_DN));
			RegSetValueEx(hkey, _T("HotKeyVKeyPLAY"), 0, REG_DWORD, (PBYTE)&HotKeyVKeyPLAY, sizeof(HotKeyVKeyPLAY)); RegSetValueEx(hkey, _T("HotKeyFlagPLAY"), 0, REG_DWORD, (PBYTE)&HotKeyFlagPLAY, sizeof(HotKeyFlagPLAY));
			RegSetValueEx(hkey, _T("HotKeyVKeySTRAIGHT"), 0, REG_DWORD, (PBYTE)&HotKeyVKeySTRAIGHT, sizeof(HotKeyVKeySTRAIGHT)); RegSetValueEx(hkey, _T("HotKeyFlagSTRAIGHT"), 0, REG_DWORD, (PBYTE)&HotKeyFlagSTRAIGHT, sizeof(HotKeyFlagSTRAIGHT));
			RegSetValueEx(hkey, _T("HotKeyVKeySIMPLE"), 0, REG_DWORD, (PBYTE)&HotKeyVKeySIMPLE, sizeof(HotKeyVKeySIMPLE)); RegSetValueEx(hkey, _T("HotKeyFlagSIMPLE"), 0, REG_DWORD, (PBYTE)&HotKeyFlagSIMPLE, sizeof(HotKeyFlagSIMPLE));
			RegSetValueEx(hkey, _T("HotKeyVKeyTAP"), 0, REG_DWORD, (PBYTE)&HotKeyVKeyTAP, sizeof(HotKeyVKeyTAP)); RegSetValueEx(hkey, _T("HotKeyFlagTAP"), 0, REG_DWORD, (PBYTE)&HotKeyFlagTAP, sizeof(HotKeyFlagTAP));
		}

        //Need to re-load the number of beats per measure from the text box back into m_BPMeasure,
        //otherwise we'll write out the number of beats in the custom measure!
        std::basic_string<TCHAR> strBuf = GetWindowText(IDC_BPMEASURE_EDIT);
        int const temp_bpmeasure = _ttoi(strBuf.c_str());
        RegSetValueEx(hkey, _T("BPMeasure"), 0, REG_DWORD, (PBYTE)&temp_bpmeasure, sizeof(m_BPMeasure));
        
        RegSetValueEx(hkey, _T("Metronome Style"), 0, REG_DWORD, (PBYTE )&m_MetronomeStyle, sizeof(m_MetronomeStyle));
        
        {for(char i = 0; i < MAX_SOUNDS; i++)
        {
            TCHAR soundname[] = _T("Sound x");
            soundname[6] = '0' + i;
            RegSetValueEx(hkey, soundname, 0, REG_DWORD, (PBYTE )&(m_midi_instrument[i]), sizeof(int));

            TCHAR volname[] = _T("Volume x");
            volname[7] = '0' + i;
            RegSetValueEx(hkey, volname, 0, REG_DWORD, (PBYTE)&(m_midi_volume[i]), sizeof(int));

            TCHAR blinkname[] = _T("Blinker x");
            blinkname[8] = '0' + i;
            RegSetValueEx(hkey, blinkname, 0, REG_DWORD, (PBYTE)&(m_blink_size[i]), sizeof(int));
        }}

        std::basic_string<TCHAR> strCustomMeasure = GetWindowText(IDC_GROUP_EDIT);
        RegSetValueEx(hkey, _T("Custom Measure"), 0, REG_SZ, (PBYTE)strCustomMeasure.c_str(), strCustomMeasure.length()*sizeof(TCHAR));

        RegSetValueEx(hkey, _T("Blinking"), 0, REG_DWORD, (PBYTE)&m_blinking, sizeof(m_blinking));

		// BHB - Added the window location settings to this if block. No reason to save with individual presets.
		if (preset_name == NULL) {
			RECT    window_rect = {0,0,0,0};
			GetWindowRect(m_hWnd, &window_rect);

			RegSetValueEx(hkey, _T("WindowTop"),    0, REG_DWORD, (PBYTE)&window_rect.top,    sizeof(LONG));
			RegSetValueEx(hkey, _T("WindowBottom"), 0, REG_DWORD, (PBYTE)&window_rect.bottom, sizeof(LONG));
			RegSetValueEx(hkey, _T("WindowLeft"),   0, REG_DWORD, (PBYTE)&window_rect.left,   sizeof(LONG));
			RegSetValueEx(hkey, _T("WindowRight"),  0, REG_DWORD, (PBYTE)&window_rect.right,  sizeof(LONG));

            // write out the index of the preset that's currently selected
            int preset_num = ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_GETCURSEL, 0, 0);
            RegSetValueEx(hkey, _T("Preset"), 0, REG_DWORD, (PBYTE)&preset_num, sizeof(DWORD));
        }

        // close the registry key now that we're done
        RegCloseKey(hkey);

        bRet = true;
    }

    return bRet;
}

void CMetronomeDlg_RegPersist::LoadSettings(LPCTSTR preset_name)
{
    TCHAR    stringbuf[MAX_BPMEASURE + 1];
    DWORD    dwType, dwSize;
    RECT    window_rect;

    HKEY hkey = OpenKey(preset_name, false);

    if(hkey)
    {
        // Load the beats per minute
        dwType = REG_DWORD;
        dwSize = sizeof(m_BPMinute);
        m_BPMinute = 200;
        RegQueryValueEx(hkey, _T("BPMinute"), 0, &dwType, (PBYTE)&m_BPMinute, &dwSize);

		// BHB - Don't keep separate hotkey settings for each preset; I think hotkeys should be global
		if (preset_name == NULL) {
			unsigned long HotKeyVKeyTEMPO_UP = 0; unsigned long HotKeyFlagTEMPO_UP = 0;
			unsigned long HotKeyVKeyTEMPO_DN = 0; unsigned long HotKeyFlagTEMPO_DN = 0;
			unsigned long HotKeyVKeyPLAY = 0; unsigned long HotKeyFlagPLAY = 0;
			unsigned long HotKeyVKeySTRAIGHT = 0; unsigned long HotKeyFlagSTRAIGHT = 0;
			unsigned long HotKeyVKeySIMPLE = 0; unsigned long HotKeyFlagSIMPLE = 0;
			unsigned long HotKeyVKeyTAP = 0; unsigned long HotKeyFlagTAP = 0;
			RegQueryValueEx(hkey, _T("HotKeyVKeyTEMPO_UP"), 0, &dwType, (PBYTE)&HotKeyVKeyTEMPO_UP, &dwSize); RegQueryValueEx(hkey, _T("HotKeyFlagTEMPO_UP"), 0, &dwType, (PBYTE)&HotKeyFlagTEMPO_UP, &dwSize); RegisterHotKey(m_hWnd, CHotKeyDlg::e_TEMPO_UP, HotKeyFlagTEMPO_UP, HotKeyVKeyTEMPO_UP); m_autopHotKeyDlg->SetHotKeyTEMPO_UP(HotKeyFlagTEMPO_UP, HotKeyVKeyTEMPO_UP);
			RegQueryValueEx(hkey, _T("HotKeyVKeyTEMPO_DN"), 0, &dwType, (PBYTE)&HotKeyVKeyTEMPO_DN, &dwSize); RegQueryValueEx(hkey, _T("HotKeyFlagTEMPO_DN"), 0, &dwType, (PBYTE)&HotKeyFlagTEMPO_DN, &dwSize); RegisterHotKey(m_hWnd, CHotKeyDlg::e_TEMPO_DN, HotKeyFlagTEMPO_DN, HotKeyVKeyTEMPO_DN); m_autopHotKeyDlg->SetHotKeyTEMPO_DN(HotKeyFlagTEMPO_DN, HotKeyVKeyTEMPO_DN);
			RegQueryValueEx(hkey, _T("HotKeyVKeyPLAY"), 0, &dwType, (PBYTE)&HotKeyVKeyPLAY, &dwSize); RegQueryValueEx(hkey, _T("HotKeyFlagPLAY"), 0, &dwType, (PBYTE)&HotKeyFlagPLAY, &dwSize); RegisterHotKey(m_hWnd, CHotKeyDlg::e_PLAY, HotKeyFlagPLAY, HotKeyVKeyPLAY); m_autopHotKeyDlg->SetHotKeyPLAY(HotKeyFlagPLAY, HotKeyVKeyPLAY);
			RegQueryValueEx(hkey, _T("HotKeyVKeySTRAIGHT"), 0, &dwType, (PBYTE)&HotKeyVKeySTRAIGHT, &dwSize); RegQueryValueEx(hkey, _T("HotKeyFlagSTRAIGHT"), 0, &dwType, (PBYTE)&HotKeyFlagSTRAIGHT, &dwSize); RegisterHotKey(m_hWnd, CHotKeyDlg::e_STRAIGHT, HotKeyFlagSTRAIGHT, HotKeyVKeySTRAIGHT); m_autopHotKeyDlg->SetHotKeySTRAIGHT(HotKeyFlagSTRAIGHT, HotKeyVKeySTRAIGHT);
			RegQueryValueEx(hkey, _T("HotKeyVKeySIMPLE"), 0, &dwType, (PBYTE)&HotKeyVKeySIMPLE, &dwSize); RegQueryValueEx(hkey, _T("HotKeyFlagSIMPLE"), 0, &dwType, (PBYTE)&HotKeyFlagSIMPLE, &dwSize); RegisterHotKey(m_hWnd, CHotKeyDlg::e_SIMPLE, HotKeyFlagSIMPLE, HotKeyVKeySIMPLE); m_autopHotKeyDlg->SetHotKeySIMPLE(HotKeyFlagSIMPLE, HotKeyVKeySIMPLE);
			RegQueryValueEx(hkey, _T("HotKeyVKeyTAP"), 0, &dwType, (PBYTE)&HotKeyVKeyTAP, &dwSize); RegQueryValueEx(hkey, _T("HotKeyFlagTAP"), 0, &dwType, (PBYTE)&HotKeyFlagTAP, &dwSize); RegisterHotKey(m_hWnd, CHotKeyDlg::e_TAP, HotKeyFlagTAP, HotKeyVKeyTAP); m_autopHotKeyDlg->SetHotKeyTAP(HotKeyFlagTAP, HotKeyVKeyTAP);
		}
        _itot(m_BPMinute, stringbuf, 10);
        ::SetWindowText(GET_HWND(IDC_BPMINUTE_EDIT), stringbuf);
 		// BHB: Bugfix - Tempo slider should update whenever BPMinute changes
		::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETPOS, TRUE, BPMToSlider(m_BPMinute)); // BHB
        OnKillfocusBpminuteEdit();

        // Load the beats per measure
        dwType = REG_DWORD;
        dwSize = sizeof(m_BPMeasure);
        m_BPMeasure = 4;
        RegQueryValueEx(hkey, _T("BPMeasure"), 0, &dwType, (PBYTE)&m_BPMeasure, &dwSize);

        _itot(m_BPMeasure, stringbuf, 10);
        ::SetWindowText(GET_HWND(IDC_BPMEASURE_EDIT), stringbuf);
        
        for(char i = 0; i < MAX_SOUNDS; i++)
        {
            // Load which midi voices to use
            dwSize = sizeof(int);
            TCHAR soundname[] = _T("Sound x");
            soundname[6] = '0' + i;
            m_midi_instrument[i] = s_DefaultInstruments[i];
            RegQueryValueEx(hkey, soundname, 0, &dwType, (PBYTE )&(m_midi_instrument[i]), &dwSize);

            if (m_midi_instrument[i] < 65536)
                ::SendMessage(m_SoundCombo[i], CB_SETCURSEL, MidiToAlpha[m_midi_instrument[i]&0xFFFF], 0);
            else
                ::SendMessage(m_SoundCombo[i], CB_SETCURSEL, GM1_TRACK10_NUM_VOICES+m_midi_instrument[i]-65536, 0);

            // Load the volume of the voices
            dwSize = sizeof(int);
            TCHAR volname[] = _T("Volume x");
            volname[7] = '0' + i;
            m_midi_volume[i] = 127;
            RegQueryValueEx(hkey, volname, 0, &dwType, (PBYTE )&(m_midi_volume[i]), &dwSize);

            ::SendMessage(m_VolumeSlider[i], TBM_SETPOS, TRUE, m_midi_volume[i]);

            // Load the size of the blinker display for the voices
            dwSize = sizeof(int);
            TCHAR blinkname[] = _T("Blinker x");
            blinkname[8] = '0' + i;
            // Only load a default value if this is a startup load
            if(preset_name == NULL)
                m_blink_size[i] = MAX_SOUNDS - 1;  // Make it the biggest dot
            RegQueryValueEx(hkey, blinkname, 0, &dwType, (PBYTE )&(m_blink_size[i]), &dwSize);

            ::SendMessage(m_BlinkerSlider[i], TBM_SETPOS, TRUE, m_blink_size[i]);
            m_autopBlinkerSliderDisplay[i]->SetState(m_blink_size[i]);
        }

        // Load the custom measure string
        dwType = REG_SZ;
        dwSize = MAX_BPMEASURE + 1;
		// BHB - default to my rock beat example (former value was "31212012121")
		_tcscpy(stringbuf, _T("[2*](35)0(34)0(35)5(34)0"));
		RegQueryValueEx(hkey, _T("Custom Measure"), 0, &dwType, (PBYTE)stringbuf, &dwSize);
        ::SetWindowText(*m_autopGroupEdit.get(), stringbuf);
        
        // Load which mode to use (plain, measure, or custom)
        dwType = REG_DWORD;
        dwSize = sizeof(m_MetronomeStyle);
        m_MetronomeStyle = metPlain;
        RegQueryValueEx(hkey, _T("Metronome Style"), 0, &dwType, (PBYTE )&m_MetronomeStyle, &dwSize);

        if(m_MetronomeStyle == metPlain) 
        {
            ::SendMessage(GET_HWND(IDC_RADIO_PLAIN  ), BM_SETCHECK, BST_CHECKED  , 0);
            ::SendMessage(GET_HWND(IDC_RADIO_MEASURE), BM_SETCHECK, BST_UNCHECKED, 0);
            ::SendMessage(GET_HWND(IDC_RADIO_GROUP  ), BM_SETCHECK, BST_UNCHECKED, 0);
            OnRadioPlain(0,0);
        }
        else if(m_MetronomeStyle == metMeasure)
        {
            ::SendMessage(GET_HWND(IDC_RADIO_PLAIN  ), BM_SETCHECK, BST_UNCHECKED, 0);
            ::SendMessage(GET_HWND(IDC_RADIO_MEASURE), BM_SETCHECK, BST_CHECKED  , 0);
            ::SendMessage(GET_HWND(IDC_RADIO_GROUP  ), BM_SETCHECK, BST_UNCHECKED, 0);
            OnRadioMeasure(0,0);
        }
        else
        {
            ::SendMessage(GET_HWND(IDC_RADIO_PLAIN  ), BM_SETCHECK, BST_UNCHECKED, 0);
            ::SendMessage(GET_HWND(IDC_RADIO_MEASURE), BM_SETCHECK, BST_UNCHECKED, 0);
            ::SendMessage(GET_HWND(IDC_RADIO_GROUP  ), BM_SETCHECK, BST_CHECKED  , 0);
            OnRadioGroup(0,0);
        }

        // Load the blinking state
        dwType = REG_DWORD;
        dwSize = sizeof(m_blinking);
        m_blinking = true;
        RegQueryValueEx(hkey, _T("Blinking"), 0, &dwType, (PBYTE)&m_blinking, &dwSize);

        ::SendMessage(GET_HWND(IDC_BLINK_CHECK), BM_SETCHECK, m_blinking?BST_CHECKED:BST_UNCHECKED, 0);
        OnBlinkCheck(0,0);

        // If this is the main call to load settings (not one to load a
        // preset), then load all the preset names and set the preset
        // combo box to have the correct one selected
        if(preset_name == NULL)
        {
            // load the presets
            int a = 0;
            unsigned long StringBufLength = sizeof(stringbuf)/sizeof(stringbuf[0]);
            while(ERROR_SUCCESS == RegEnumKeyEx(hkey, a, stringbuf, &StringBufLength, NULL, NULL, NULL, NULL))
            {
                ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_ADDSTRING, 0, (long)(LPCTSTR)stringbuf);
                StringBufLength = sizeof(stringbuf)/sizeof(stringbuf[0]); //Need to reset stringbuf length (was overwritten by RegEnumKeyEx
                a++;
            }

            // Load the index of the preset that's currently selected
            // and select it
            dwType = REG_DWORD;
            dwSize = sizeof(DWORD);
            int preset_num = 0;
            RegQueryValueEx(hkey, _T("Preset"), 0, &dwType, (PBYTE)&preset_num, &dwSize);

            ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_SETCURSEL, preset_num, 0);
            if(preset_num != -1)
            {
                ::EnableWindow(GET_HWND(IDC_DELETE_PRESET_BUTTON), TRUE);
                ::EnableWindow(GET_HWND(IDC_SAVE_PRESET_BUTTON), FALSE);
            }
            else
            {
                ::EnableWindow(GET_HWND(IDC_DELETE_PRESET_BUTTON), FALSE);
                ::EnableWindow(GET_HWND(IDC_SAVE_PRESET_BUTTON), TRUE);
            }

			ProcessCmdLine();	// BHB -- Adjust settings based on any command line options

            // Save the dimensions/location of the window
            dwType = REG_DWORD;
            dwSize = sizeof(LONG);

            GetWindowRect(m_hWnd, &window_rect); // so we have some default values

            RegQueryValueEx(hkey, _T("WindowTop"),    0, &dwType, (PBYTE)&window_rect.top,    &dwSize);

            RegQueryValueEx(hkey, _T("WindowBottom"), 0, &dwType, (PBYTE)&window_rect.bottom, &dwSize);

            RegQueryValueEx(hkey, _T("WindowLeft"),   0, &dwType, (PBYTE)&window_rect.left,   &dwSize);

            RegQueryValueEx(hkey, _T("WindowRight"),  0, &dwType, (PBYTE)&window_rect.right,  &dwSize);

            // If the window is off the screen then fix the problem
            if (window_rect.left   >= GetSystemMetrics(SM_CXSCREEN) || 
                window_rect.top    >= GetSystemMetrics(SM_CYSCREEN) ||
                window_rect.right  <  0 ||
                window_rect.bottom <  0)
            {
                // Move the window to the upper left corner of the screen
                window_rect.bottom -= window_rect.top;
                window_rect.right  -= window_rect.left;
                window_rect.top     = 0;
                window_rect.left    = 0;
            }

            MoveWindow(m_hWnd, window_rect.left, window_rect.top, window_rect.right-window_rect.left, window_rect.bottom-window_rect.top, TRUE);
        }

        // Close the regisry now that we're done
        RegCloseKey(hkey);
    }
}


void CMetronomeDlg_RegPersist::OnSelChangePresetCombo() 
{
    std::basic_string<TCHAR> stringbuf;

    int index = ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_GETCURSEL, 0, 0);

    stringbuf.resize(::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_GETLBTEXTLEN, 0, 0) + 1);

    // get the currently selected item's string and pass that to LoadSettings()
    ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_GETLBTEXT, index, (long)&*stringbuf.begin());
    LoadSettings(stringbuf.c_str());

    ::EnableWindow(GET_HWND(IDC_SAVE_PRESET_BUTTON), FALSE);
    ::EnableWindow(GET_HWND(IDC_DELETE_PRESET_BUTTON), TRUE);

    ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_SETCURSEL, index, 0);
}


void CMetronomeDlg_RegPersist::OnEditChangePresetCombo() 
{
    // if there's text in the edit box, enable the save button
    ::EnableWindow(GET_HWND(IDC_SAVE_PRESET_BUTTON), ::SendMessage(GET_HWND(IDC_PRESET_COMBO), WM_GETTEXTLENGTH, 0, 0) != 0);
    
    // Set the save button to be the default, so if the user presses
    // <enter>  then the preset will be saved.
    //Um, that's a bit odd, tbh, if we usually have enter doing play/pause SetDefID(IDC_SAVE_PRESET_BUTTON);
}


void CMetronomeDlg_RegPersist::OnSelectTickComboGeneric(WPARAM id, LPARAM hWnd) {CMetronomeDlg::OnSelectTickComboGeneric(id, hWnd); SetNullPreset();}

void CMetronomeDlg_RegPersist::OnKillfocusBpminuteEdit() {CMetronomeDlg::OnKillfocusBpminuteEdit(); SetNullPreset();}
void CMetronomeDlg_RegPersist::OnChangeGroupEdit      () {CMetronomeDlg::OnChangeGroupEdit      (); SetNullPreset();}
void CMetronomeDlg_RegPersist::OnChangeBPMeasureEdit  () {CMetronomeDlg::OnChangeBPMeasureEdit  (); SetNullPreset();}
void CMetronomeDlg_RegPersist::OnChangeBPMinuteEdit   () {CMetronomeDlg::OnChangeBPMinuteEdit   (); SetNullPreset(); ::EnableWindow(GET_HWND(IDC_SAVE_PRESET_BUTTON), TRUE);}

void CMetronomeDlg_RegPersist::InitialiseToolTips()
{
    CMetronomeDlg::InitialiseToolTips();
    
    m_autopTTSAVE_PRESET_BUTTON   = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_SAVE_PRESET_BUTTON  , m_hWnd, m_hInstance, IDS_TIP_SAVE_PRESET_BUTTON  );
    m_autopTTDELETE_PRESET_BUTTON = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_DELETE_PRESET_BUTTON, m_hWnd, m_hInstance, IDS_TIP_DELETE_PRESET_BUTTON);
    m_autopTTEXPORT_PRESET_BUTTON = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_EXPORT_PRESET_BUTTON, m_hWnd, m_hInstance, IDS_TIP_EXPORT_PRESET_BUTTON);
    m_autopTTPRESET_COMBO         = (std::auto_ptr<CBscToolTip>) new CBscToolTip(IDC_PRESET_COMBO        , m_hWnd, m_hInstance, IDS_TIP_PRESET_COMBO        );
}


long CMetronomeDlg_RegPersist::OnClose(unsigned long const nIgnore1, long const nIgnore2) 
{
    SaveSettings();

    UnregisterHotKey(m_hWnd, CHotKeyDlg::e_TEMPO_UP);
    UnregisterHotKey(m_hWnd, CHotKeyDlg::e_TEMPO_DN);
    UnregisterHotKey(m_hWnd, CHotKeyDlg::e_PLAY    );
    UnregisterHotKey(m_hWnd, CHotKeyDlg::e_STRAIGHT);
    UnregisterHotKey(m_hWnd, CHotKeyDlg::e_SIMPLE  );
    UnregisterHotKey(m_hWnd, CHotKeyDlg::e_TAP     );

    CMetronomeDlg::OnClose(0,0);
    return 1;
}


long CMetronomeDlg_RegPersist::OnHScroll(unsigned long const nSBCodeAndnPos, long const hwndScrollBar)
{
    CMetronomeDlg::OnHScroll(nSBCodeAndnPos, hwndScrollBar);
    SetNullPreset();
    ::EnableWindow(GET_HWND(IDC_SAVE_PRESET_BUTTON), TRUE);
    return 1;
}


long CMetronomeDlg_RegPersist::OnSavePresetButton(unsigned long const nIgnore1, long const nIgnore2) 
{
    TCHAR *stringbuf, *key_name;
    int string_size = ::SendMessage(GET_HWND(IDC_PRESET_COMBO), WM_GETTEXTLENGTH, 0, 0) + 1;
    bool load_preset = true;
    bool add_to_list = true;
    HKEY hkey;

    stringbuf = new TCHAR[string_size];
    ::SendMessage(GET_HWND(IDC_PRESET_COMBO), WM_GETTEXT, string_size, (long)stringbuf);

    key_name = new TCHAR[string_size + _tcslen(s_WeirdMetRegKeyRoot) + 1];
    _tcscpy(key_name, s_WeirdMetRegKeyRoot);
    _tcscat(key_name, _T("\\"));
    _tcscat(key_name, stringbuf);
    
    // Try to open the registry key
    if(RegOpenKeyEx(HKEY_CURRENT_USER, key_name, 0, KEY_READ, &hkey) == ERROR_SUCCESS)
    {
        // The key already exists.  Ask if the user really wants to overwrite it
        // do the actual asking with a message box
        if(MessageBox(m_hWnd, m_strPresetOverwrite.c_str(), m_strPresetAlreadyExists.c_str(),
            MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL) == IDNO)
        {
            load_preset = false;
        }
        else
        {
            add_to_list = false;
        }

        // Close the key
        RegCloseKey(hkey);
    }

    if(load_preset)
    {
        if(!SaveSettings(stringbuf))
        {
            MessageBox(m_hWnd, 
                m_strPresetSaveError.c_str(),
                NULL, 
                MB_OK | MB_ICONERROR | MB_APPLMODAL);
        }
        else
        {
            int entry_index = CB_ERR;
            if (add_to_list)
                // Add the string to the list box
                entry_index = ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_ADDSTRING, 0, (long)(LPCTSTR)stringbuf);
            else
                // Figure out the index of this already-existing entry
                entry_index = ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_FINDSTRINGEXACT, 0, (long)stringbuf);

            // Make the selection box realize that it should have that entry selected.
            ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_SETCURSEL, entry_index, 0);

            ::EnableWindow(GET_HWND(IDC_SAVE_PRESET_BUTTON), FALSE);
            ::EnableWindow(GET_HWND(IDC_DELETE_PRESET_BUTTON), TRUE);
        }
    }

    delete[] key_name;
    delete[] stringbuf;

    return 1;
}


long CMetronomeDlg_RegPersist::OnCmdPresetCombo(unsigned long const idCmdMsg, long const Whatever) 
{
    if  (idCmdMsg == CBN_SELCHANGE)
        OnSelChangePresetCombo ();
    if  (idCmdMsg == CBN_EDITCHANGE)
        OnEditChangePresetCombo();
    return 1;
}


long CMetronomeDlg_RegPersist::OnDeletePresetButton(unsigned long const nIgnore1, long const nIgnore2) 
{
    TCHAR *stringbuf, *key_name;
    int string_size = ::SendMessage(GET_HWND(IDC_PRESET_COMBO), WM_GETTEXTLENGTH, 0, 0) + 1;
    bool load_preset = false;

    stringbuf = new TCHAR[string_size];
    ::SendMessage(GET_HWND(IDC_PRESET_COMBO), WM_GETTEXT, string_size, (long)stringbuf);

    key_name = new TCHAR[string_size + _tcslen(s_WeirdMetRegKeyRoot) + 1];
    _tcscpy(key_name, s_WeirdMetRegKeyRoot);
    _tcscat(key_name, _T("\\"));
    _tcscat(key_name, stringbuf);
    
    RegDeleteKey(HKEY_CURRENT_USER, key_name);

    int const nCurSel = ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_GETCURSEL, 0, 0);
    ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_DELETESTRING, nCurSel, 0);
    SetNullPreset();

    delete[] key_name;
    delete[] stringbuf;

    return 1;
}


long CMetronomeDlg_RegPersist::OnRadioPlain           (unsigned long const nIgnore1, long const nIgnore2)
{
    CMetronomeDlg::OnRadioPlain(0,0);
    SetNullPreset();

    return 1;
}


long CMetronomeDlg_RegPersist::OnExportPresetButton(unsigned long const nIgnore1, long const nIgnore2) 
{
    OPENFILENAME ofn;
    TCHAR szE[] = _T("/e \"");
    TCHAR szR[] = _T("\" \"HKEY_CURRENT_USER\\Software\\BHBSoftware\\Open Metronome\"");
    TCHAR szWholeFile[MAX_PATH + sizeof(szE) + sizeof(szR)] = _T("");       // buffer for file name
    TCHAR szFile[255];       // buffer for file name
    TCHAR title[255];        //buffer for file title

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFileTitle = title;
    
    //
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = (TCHAR)'\0';
    ofn.lpstrDefExt = _T("REG");
    ofn.lpstrFileTitle[0] = (TCHAR)'\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.nMaxFileTitle = sizeof(title);

#ifdef USE_WEIRD_MIDI
    ofn.lpstrFilter = _T("Registry Files (*.REG)\0*.REG\0\0");
#else //USE_WEIRD_WAV
    ofn.lpstrFilter = _T("Wave Sound (*.WAV)\0*.WAV\0Registry Files (*.REG)\0*.REG\0\0");
#endif

    ofn.nFilterIndex = 1;
    
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if(GetSaveFileName(&ofn) == IDOK) 
    {
        std::basic_string<TCHAR> const strFilename(ofn.lpstrFile);
        if (_tcsstr(_tcsupr(ofn.lpstrFile), _T(".WAV"))-ofn.lpstrFile == strFilename.length()-4)
        {
            if (BuildPlayerInstance())
            {
                if (m_autopTicker.get())
                {
                    m_autopTicker->Export(strFilename.c_str(), m_NumExp);
                    m_autopTicker = (std::auto_ptr<IBeatBox>) NULL;
                }
            }
        }
#ifndef UNDER_CE
        else
        {
            _tcscpy(szWholeFile, szE);
            _tcscat(szWholeFile, strFilename.c_str());
            _tcscat(szWholeFile, szR);
	        ShellExecute(this->m_hWnd, _T("open"), _T("regedit"), szWholeFile, 0, SW_SHOWNORMAL);
        }
#endif
    }
    return 0;
}
