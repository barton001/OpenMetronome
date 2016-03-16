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
#if !defined(AFX_HotKeyDLG_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
#define AFX_HotKeyDLG_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <memory>
#include "resource.h"
#include "BscBblDlg.h"
#include "EditboxCatchKeypress.h"


//See also base-class.
//This implements the "HotKey box", and allows the user to click on the WeridMet homepage link
//(which pops up the homepage: neat!)
class CHotKeyDlg : public CBscBblDlg
{
public:
    enum HotkeyIDs{e_TEMPO_UP = 1, e_TEMPO_DN = 2, e_PLAY     = 3, e_STRAIGHT = 4, e_SIMPLE   = 5, e_TAP      = 6};
	CHotKeyDlg(HINSTANCE hInstance = 0, HWND hParent   = 0);


    virtual unsigned long GetHotKeyVKeyTEMPO_UP(); virtual unsigned long GetHotKeyFlagTEMPO_UP();
    virtual unsigned long GetHotKeyVKeyTEMPO_DN(); virtual unsigned long GetHotKeyFlagTEMPO_DN();
    virtual unsigned long GetHotKeyVKeyPLAY    (); virtual unsigned long GetHotKeyFlagPLAY    ();
    virtual unsigned long GetHotKeyVKeySTRAIGHT(); virtual unsigned long GetHotKeyFlagSTRAIGHT();
    virtual unsigned long GetHotKeyVKeySIMPLE  (); virtual unsigned long GetHotKeyFlagSIMPLE  ();
    virtual unsigned long GetHotKeyVKeyTAP     (); virtual unsigned long GetHotKeyFlagTAP     ();
    virtual void SetHotKeyTEMPO_UP(unsigned long const HotKeyFlagTEMPO_UP, unsigned long const HotKeyVKeyTEMPO_UP);
    virtual void SetHotKeyTEMPO_DN(unsigned long const HotKeyFlagTEMPO_DN, unsigned long const HotKeyVKeyTEMPO_DN);
    virtual void SetHotKeyPLAY    (unsigned long const HotKeyFlagPLAY    , unsigned long const HotKeyVKeyPLAY    );
    virtual void SetHotKeySTRAIGHT(unsigned long const HotKeyFlagSTRAIGHT, unsigned long const HotKeyVKeySTRAIGHT);
    virtual void SetHotKeySIMPLE  (unsigned long const HotKeyFlagSIMPLE  , unsigned long const HotKeyVKeySIMPLE  );
    virtual void SetHotKeyTAP     (unsigned long const HotKeyFlagTAP     , unsigned long const HotKeyVKeyTAP     );


    virtual void SetBPMThesholds(unsigned long const Min, unsigned long const Max, unsigned long const Inc, unsigned long const Exp);
    virtual unsigned long GetMinBPM() {return m_MinBPM;};
    virtual unsigned long GetMaxBPM() {return m_MaxBPM;};
    virtual unsigned long GetIncBPM() {return m_IncBPM;};
    virtual unsigned long GetNumExp() {return m_NumExp;};


protected:
    virtual long OnInitDialog();
    virtual long OnOK();         //Analogouse to CDialog::OnOK: Override to do your own OnOK handler
    virtual long OnCancel();     //Analogouse to CDialog::OnCancel: Override as above...
    DECLARE_MSG_HANDLER(OnDeleteHotkeyTempoUp , CHotKeyDlg);
    DECLARE_MSG_HANDLER(OnDeleteHotkeyTempoDn , CHotKeyDlg);
    DECLARE_MSG_HANDLER(OnDeleteHotkeyPlay    , CHotKeyDlg);
    DECLARE_MSG_HANDLER(OnDeleteHotkeyStraight, CHotKeyDlg);
    DECLARE_MSG_HANDLER(OnDeleteHotkeySimple  , CHotKeyDlg);
    DECLARE_MSG_HANDLER(OnDeleteHotkeyTap     , CHotKeyDlg);


private:
    std::auto_ptr<CEditboxCatchKeypress> m_autopCatchTEMPO_UP; unsigned long m_HotKeyVKeyTEMPO_UP; unsigned long m_HotKeyFlagTEMPO_UP;
    std::auto_ptr<CEditboxCatchKeypress> m_autopCatchTEMPO_DN; unsigned long m_HotKeyVKeyTEMPO_DN; unsigned long m_HotKeyFlagTEMPO_DN;
    std::auto_ptr<CEditboxCatchKeypress> m_autopCatchPLAY    ; unsigned long m_HotKeyVKeyPLAY    ; unsigned long m_HotKeyFlagPLAY    ;
    std::auto_ptr<CEditboxCatchKeypress> m_autopCatchSTRAIGHT; unsigned long m_HotKeyVKeySTRAIGHT; unsigned long m_HotKeyFlagSTRAIGHT;
    std::auto_ptr<CEditboxCatchKeypress> m_autopCatchSIMPLE  ; unsigned long m_HotKeyVKeySIMPLE  ; unsigned long m_HotKeyFlagSIMPLE  ;
    std::auto_ptr<CEditboxCatchKeypress> m_autopCatchTAP     ; unsigned long m_HotKeyVKeyTAP     ; unsigned long m_HotKeyFlagTAP     ;

    unsigned long m_MinBPM;
    unsigned long m_MaxBPM;
    unsigned long m_IncBPM;
    unsigned long m_NumExp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HotKeyDLG_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
