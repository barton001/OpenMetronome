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
#include "stdafx.h"
#include "HotKeyDlg.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//--------------------------------------------------------------------------------------------------
CHotKeyDlg::CHotKeyDlg(HINSTANCE hInstance, HWND hParent) :
    m_HotKeyVKeyTEMPO_UP(0), m_HotKeyFlagTEMPO_UP(0),
    m_HotKeyVKeyTEMPO_DN(0), m_HotKeyFlagTEMPO_DN(0),
    m_HotKeyVKeyPLAY    (0), m_HotKeyFlagPLAY    (0),
    m_HotKeyVKeySTRAIGHT(0), m_HotKeyFlagSTRAIGHT(0),
    m_HotKeyVKeySIMPLE  (0), m_HotKeyFlagSIMPLE  (0),
    m_HotKeyVKeyTAP     (0), m_HotKeyFlagTAP     (0),
    m_MinBPM(0),
    m_MaxBPM(1000),
    m_IncBPM(30),
    m_NumExp(60),
    CBscBblDlg(IDD_HOTKEY_MAP, hInstance, hParent, false, true)
{
    ADD_CMD_HANDLER(IDC_BUTTON_X_TEMPO_UP, OnDeleteHotkeyTempoUp );
    ADD_CMD_HANDLER(IDC_BUTTON_X_TEMPO_DN, OnDeleteHotkeyTempoDn );
    ADD_CMD_HANDLER(IDC_BUTTON_X_PLAY    , OnDeleteHotkeyPlay    );
    ADD_CMD_HANDLER(IDC_BUTTON_X_STRAIGHT, OnDeleteHotkeyStraight);
    ADD_CMD_HANDLER(IDC_BUTTON_X_SIMPLE  , OnDeleteHotkeySimple  );
    ADD_CMD_HANDLER(IDC_BUTTON_X_TAP     , OnDeleteHotkeyTap  );
}
//--------------------------------------------------------------------------------------------------

    
long CHotKeyDlg::OnInitDialog()
{
    long lRet = CBscBblDlg::OnInitDialog();

#ifdef UNDER_CE
    SetWindowPos(GET_HWND(IDC_STATIC_FRAME_HOTKEYS), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
    SetWindowPos(GET_HWND(IDC_STATIC_FRAME_BPM    ), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
    SetWindowPos(GET_HWND(IDC_STATIC_FRAME_WAV_EXP), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
#endif

    m_autopCatchTEMPO_UP = (std::auto_ptr<CEditboxCatchKeypress >) NULL;
    m_autopCatchTEMPO_DN = (std::auto_ptr<CEditboxCatchKeypress >) NULL;
    m_autopCatchPLAY     = (std::auto_ptr<CEditboxCatchKeypress >) NULL;
    m_autopCatchSTRAIGHT = (std::auto_ptr<CEditboxCatchKeypress >) NULL;
    m_autopCatchSIMPLE   = (std::auto_ptr<CEditboxCatchKeypress >) NULL;
    m_autopCatchTAP      = (std::auto_ptr<CEditboxCatchKeypress >) NULL;
    m_autopCatchTEMPO_UP = (std::auto_ptr<CEditboxCatchKeypress >) new CEditboxCatchKeypress (e_TEMPO_UP, m_HotKeyFlagTEMPO_UP, m_HotKeyVKeyTEMPO_UP, GET_HWND(IDC_EDIT_HOTKEY_TEMPO_UP));
    m_autopCatchTEMPO_DN = (std::auto_ptr<CEditboxCatchKeypress >) new CEditboxCatchKeypress (e_TEMPO_DN, m_HotKeyFlagTEMPO_DN, m_HotKeyVKeyTEMPO_DN, GET_HWND(IDC_EDIT_HOTKEY_TEMPO_DN));
    m_autopCatchPLAY     = (std::auto_ptr<CEditboxCatchKeypress >) new CEditboxCatchKeypress (e_PLAY    , m_HotKeyFlagPLAY    , m_HotKeyVKeyPLAY    , GET_HWND(IDC_EDIT_HOTKEY_PLAY    ));
    m_autopCatchSTRAIGHT = (std::auto_ptr<CEditboxCatchKeypress >) new CEditboxCatchKeypress (e_STRAIGHT, m_HotKeyFlagSTRAIGHT, m_HotKeyVKeySTRAIGHT, GET_HWND(IDC_EDIT_HOTKEY_STRAIGHT));
    m_autopCatchSIMPLE   = (std::auto_ptr<CEditboxCatchKeypress >) new CEditboxCatchKeypress (e_SIMPLE  , m_HotKeyFlagSIMPLE  , m_HotKeyVKeySIMPLE  , GET_HWND(IDC_EDIT_HOTKEY_SIMPLE  ));
    m_autopCatchTAP      = (std::auto_ptr<CEditboxCatchKeypress >) new CEditboxCatchKeypress (e_TAP     , m_HotKeyFlagTAP     , m_HotKeyVKeyTAP     , GET_HWND(IDC_EDIT_HOTKEY_TAP     ));

    TCHAR buf[12]; 
    _ultot(m_MinBPM, buf, 10); SetWindowText(GET_HWND(IDC_EDIT_BPM_MIN), buf);
    _ultot(m_MaxBPM, buf, 10); SetWindowText(GET_HWND(IDC_EDIT_BPM_MAX), buf);
    _ultot(m_IncBPM, buf, 10); SetWindowText(GET_HWND(IDC_EDIT_BPM_INC), buf);
    _ultot(m_NumExp, buf, 10); SetWindowText(GET_HWND(IDC_EDIT_WAV_EXP), buf);

    return 0;
}
//--------------------------------------------------------------------------------------------------


long CHotKeyDlg::OnCancel()
{
    CBscBblDlg::OnCancel();
    return 1;
}
//--------------------------------------------------------------------------------------------------


long CHotKeyDlg::OnOK()
{
    CBscBblDlg::OnOK();

    m_HotKeyVKeyTEMPO_UP = m_autopCatchTEMPO_UP->GetVKey(); m_HotKeyFlagTEMPO_UP = m_autopCatchTEMPO_UP->GetFlag(); UnregisterHotKey(GetParent(m_hWnd), e_TEMPO_UP); RegisterHotKey(GetParent(m_hWnd), e_TEMPO_UP, m_HotKeyFlagTEMPO_UP, m_HotKeyVKeyTEMPO_UP);
    m_HotKeyVKeyTEMPO_DN = m_autopCatchTEMPO_DN->GetVKey(); m_HotKeyFlagTEMPO_DN = m_autopCatchTEMPO_DN->GetFlag(); UnregisterHotKey(GetParent(m_hWnd), e_TEMPO_DN); RegisterHotKey(GetParent(m_hWnd), e_TEMPO_DN, m_HotKeyFlagTEMPO_DN, m_HotKeyVKeyTEMPO_DN);
    m_HotKeyVKeyPLAY     = m_autopCatchPLAY    ->GetVKey(); m_HotKeyFlagPLAY     = m_autopCatchPLAY    ->GetFlag(); UnregisterHotKey(GetParent(m_hWnd), e_PLAY    ); RegisterHotKey(GetParent(m_hWnd), e_PLAY    , m_HotKeyFlagPLAY    , m_HotKeyVKeyPLAY    );
    m_HotKeyVKeySTRAIGHT = m_autopCatchSTRAIGHT->GetVKey(); m_HotKeyFlagSTRAIGHT = m_autopCatchSTRAIGHT->GetFlag(); UnregisterHotKey(GetParent(m_hWnd), e_STRAIGHT); RegisterHotKey(GetParent(m_hWnd), e_STRAIGHT, m_HotKeyFlagSTRAIGHT, m_HotKeyVKeySTRAIGHT);
    m_HotKeyVKeySIMPLE   = m_autopCatchSIMPLE  ->GetVKey(); m_HotKeyFlagSIMPLE   = m_autopCatchSIMPLE  ->GetFlag(); UnregisterHotKey(GetParent(m_hWnd), e_SIMPLE  ); RegisterHotKey(GetParent(m_hWnd), e_SIMPLE  , m_HotKeyFlagSIMPLE  , m_HotKeyVKeySIMPLE  );
    m_HotKeyVKeyTAP      = m_autopCatchTAP     ->GetVKey(); m_HotKeyFlagTAP      = m_autopCatchTAP     ->GetFlag(); UnregisterHotKey(GetParent(m_hWnd), e_TAP     ); RegisterHotKey(GetParent(m_hWnd), e_TAP     , m_HotKeyFlagTAP     , m_HotKeyVKeyTAP     );
 
    m_MinBPM = _tcstoul(GetWindowText(IDC_EDIT_BPM_MIN).c_str(), NULL, 10);
    m_MaxBPM = _tcstoul(GetWindowText(IDC_EDIT_BPM_MAX).c_str(), NULL, 10);
    m_IncBPM = _tcstoul(GetWindowText(IDC_EDIT_BPM_INC).c_str(), NULL, 10);
    m_NumExp = _tcstoul(GetWindowText(IDC_EDIT_WAV_EXP).c_str(), NULL, 10);

    return 1;
}
//--------------------------------------------------------------------------------------------------


unsigned long CHotKeyDlg::GetHotKeyVKeyTEMPO_UP() {return m_HotKeyVKeyTEMPO_UP;} unsigned long CHotKeyDlg::GetHotKeyFlagTEMPO_UP() {return m_HotKeyFlagTEMPO_UP;}
unsigned long CHotKeyDlg::GetHotKeyVKeyTEMPO_DN() {return m_HotKeyVKeyTEMPO_DN;} unsigned long CHotKeyDlg::GetHotKeyFlagTEMPO_DN() {return m_HotKeyFlagTEMPO_DN;}
unsigned long CHotKeyDlg::GetHotKeyVKeyPLAY    () {return m_HotKeyVKeyPLAY    ;} unsigned long CHotKeyDlg::GetHotKeyFlagPLAY    () {return m_HotKeyFlagPLAY    ;}
unsigned long CHotKeyDlg::GetHotKeyVKeySTRAIGHT() {return m_HotKeyVKeySTRAIGHT;} unsigned long CHotKeyDlg::GetHotKeyFlagSTRAIGHT() {return m_HotKeyFlagSTRAIGHT;}
unsigned long CHotKeyDlg::GetHotKeyVKeySIMPLE  () {return m_HotKeyVKeySIMPLE  ;} unsigned long CHotKeyDlg::GetHotKeyFlagSIMPLE  () {return m_HotKeyFlagSIMPLE  ;}
unsigned long CHotKeyDlg::GetHotKeyVKeyTAP     () {return m_HotKeyVKeyTAP     ;} unsigned long CHotKeyDlg::GetHotKeyFlagTAP     () {return m_HotKeyFlagTAP     ;}
void CHotKeyDlg::SetHotKeyTEMPO_UP(unsigned long const HotKeyFlagTEMPO_UP, unsigned long const HotKeyVKeyTEMPO_UP) {m_HotKeyVKeyTEMPO_UP = HotKeyVKeyTEMPO_UP; m_HotKeyFlagTEMPO_UP = HotKeyFlagTEMPO_UP;}
void CHotKeyDlg::SetHotKeyTEMPO_DN(unsigned long const HotKeyFlagTEMPO_DN, unsigned long const HotKeyVKeyTEMPO_DN) {m_HotKeyVKeyTEMPO_DN = HotKeyVKeyTEMPO_DN; m_HotKeyFlagTEMPO_DN = HotKeyFlagTEMPO_DN;}
void CHotKeyDlg::SetHotKeyPLAY    (unsigned long const HotKeyFlagPLAY    , unsigned long const HotKeyVKeyPLAY    ) {m_HotKeyVKeyPLAY     = HotKeyVKeyPLAY    ; m_HotKeyFlagPLAY     = HotKeyFlagPLAY    ;}
void CHotKeyDlg::SetHotKeySTRAIGHT(unsigned long const HotKeyFlagSTRAIGHT, unsigned long const HotKeyVKeySTRAIGHT) {m_HotKeyVKeySTRAIGHT = HotKeyVKeySTRAIGHT; m_HotKeyFlagSTRAIGHT = HotKeyFlagSTRAIGHT;}
void CHotKeyDlg::SetHotKeySIMPLE  (unsigned long const HotKeyFlagSIMPLE  , unsigned long const HotKeyVKeySIMPLE  ) {m_HotKeyVKeySIMPLE   = HotKeyVKeySIMPLE  ; m_HotKeyFlagSIMPLE   = HotKeyFlagSIMPLE  ;}
void CHotKeyDlg::SetHotKeyTAP     (unsigned long const HotKeyFlagTAP     , unsigned long const HotKeyVKeyTAP     ) {m_HotKeyVKeyTAP      = HotKeyVKeyTAP     ; m_HotKeyFlagTAP      = HotKeyFlagTAP     ;}
//--------------------------------------------------------------------------------------------------


void CHotKeyDlg::SetBPMThesholds(unsigned long const Min, unsigned long const Max, unsigned long const Inc, unsigned long const Exp)
{
    m_MinBPM = Min;
    m_MaxBPM = Max;
    m_IncBPM = Inc;
    m_NumExp = Exp;
    TCHAR buf[12]; 
    _ultot(m_MinBPM, buf, 10); SetWindowText(GET_HWND(IDC_EDIT_BPM_MIN), buf);
    _ultot(m_MaxBPM, buf, 10); SetWindowText(GET_HWND(IDC_EDIT_BPM_MAX), buf);
    _ultot(m_IncBPM, buf, 10); SetWindowText(GET_HWND(IDC_EDIT_BPM_INC), buf);
    _ultot(m_NumExp, buf, 10); SetWindowText(GET_HWND(IDC_EDIT_WAV_EXP), buf);
}


//--------------------------------------------------------------------------------------------------


long CHotKeyDlg::OnDeleteHotkeyTempoUp (unsigned long const nIgnore1, long const nIgnore2) {UnregisterHotKey(GetParent(m_hWnd), e_TEMPO_UP); m_autopCatchTEMPO_UP->Clear(); return 1;}
long CHotKeyDlg::OnDeleteHotkeyTempoDn (unsigned long const nIgnore1, long const nIgnore2) {UnregisterHotKey(GetParent(m_hWnd), e_TEMPO_DN); m_autopCatchTEMPO_DN->Clear(); return 1;}
long CHotKeyDlg::OnDeleteHotkeyPlay    (unsigned long const nIgnore1, long const nIgnore2) {UnregisterHotKey(GetParent(m_hWnd), e_PLAY    ); m_autopCatchPLAY    ->Clear(); return 1;}
long CHotKeyDlg::OnDeleteHotkeyStraight(unsigned long const nIgnore1, long const nIgnore2) {UnregisterHotKey(GetParent(m_hWnd), e_STRAIGHT); m_autopCatchSTRAIGHT->Clear(); return 1;}
long CHotKeyDlg::OnDeleteHotkeySimple  (unsigned long const nIgnore1, long const nIgnore2) {UnregisterHotKey(GetParent(m_hWnd), e_SIMPLE  ); m_autopCatchSIMPLE  ->Clear(); return 1;}
long CHotKeyDlg::OnDeleteHotkeyTap     (unsigned long const nIgnore1, long const nIgnore2) {UnregisterHotKey(GetParent(m_hWnd), e_TAP     ); m_autopCatchTAP     ->Clear(); return 1;}
