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
#if !defined(AFX_MetronomeDlg_RegPersist_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
#define AFX_MetronomeDlg_RegPersist_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_
// MetronomeDlg_RegPersist.h : header file
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "MetronomeDlg.h"


//See also base-class.
//Specialises base-class to allow everything about the metronome (selected beats-per-minute etc)
//persist between successive runs of the application, and also to allow the user to save and recall
//specific configurations of interest.
//The last window size and position are retained and restored; if the last window position read on
//startup is found to be off-screen, it is ignored, and the dialog is positioned in the top-left
//Uses the registry to achieve this.
//Implements the "Save" etc preset controls at the bottom of the main dialog.
class CMetronomeDlg_RegPersist : public CMetronomeDlg
{
protected: //Member variables
    static long  const s_DefaultInstruments[MAX_SOUNDS];

    
    virtual bool SaveSettings(LPCTSTR preset_name = NULL); //Save current setup
	virtual void LoadSettings(LPCTSTR preset_name = NULL); //Recall last-used (i.e., last-saved) setup

	virtual void SetNullPreset() //Current setup has changed since last save: remove current preset
	{
		::SendMessage(::GetDlgItem(m_hWnd, IDC_PRESET_COMBO), CB_SETCURSEL, -1, 0);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_DELETE_PRESET_BUTTON), FALSE);
	};
    
    virtual void InitialiseToolTips(); //Overridden to provide tooltips for "preset" buttons etc

public:
    // Construction : See base-class
	CMetronomeDlg_RegPersist(HINSTANCE hInstance = 0, HWND hParent = 0);


protected: //Overridden handlers to load last preset on startup, and to notice if the operator
           //changes anything since the presets were loaded.  Also handlers are provided for the
           //new controls

    virtual long OnInitDialog();
    virtual long OnClose(unsigned long const nIgnore1, long const nIgnore2);
    virtual long OnHScroll(unsigned long const nSBCodeAndnPos, long const hwndScrollBar);
    virtual long OnRadioPlain(unsigned long const nIgnore1, long const nIgnore2);
    virtual void OnChangeGroupEdit();
	virtual void OnChangeBPMeasureEdit();
	virtual void OnChangeBPMinuteEdit();
	virtual void OnKillfocusBpminuteEdit();
	virtual void OnSelChangePresetCombo();
	virtual void OnEditChangePresetCombo();
	virtual void OnSelectTickComboGeneric(WPARAM id, LPARAM hWnd);
    DECLARE_MSG_HANDLER(OnSavePresetButton  , CMetronomeDlg_RegPersist);
    DECLARE_MSG_HANDLER(OnDeletePresetButton, CMetronomeDlg_RegPersist);
    DECLARE_MSG_HANDLER(OnExportPresetButton, CMetronomeDlg_RegPersist);
    DECLARE_MSG_HANDLER(OnCmdPresetCombo    , CMetronomeDlg_RegPersist);


private: //See equivalent section in base-class
    std::auto_ptr<CBscToolTip> m_autopTTSAVE_PRESET_BUTTON  ;
    std::auto_ptr<CBscToolTip> m_autopTTDELETE_PRESET_BUTTON;
    std::auto_ptr<CBscToolTip> m_autopTTEXPORT_PRESET_BUTTON;
    std::auto_ptr<CBscToolTip> m_autopTTPRESET_COMBO        ;

    std::basic_string<TCHAR> m_strPresetOverwrite    ;
    std::basic_string<TCHAR> m_strPresetAlreadyExists;
    std::basic_string<TCHAR> m_strPresetSaveError    ;
};                    

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MetronomeDlg_RegPersist_H__E8300326_0843_11D6_91F5_004033D19F0F__INCLUDED_)
