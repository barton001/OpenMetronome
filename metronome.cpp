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
// metronome.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <tchar.h>
#include <memory>
#include "metronome.h"
#include "MetronomeDlg_RegPersist_DragSize.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef  _UNICODE
int APIENTRY wWinMain( //Should be a better way of doing this, but _tWinMain does not seem to be working in CE:/
#else
int APIENTRY WinMain(
#endif
                       HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow)
{
    INITCOMMONCONTROLSEX icc = {0,0};
    icc.dwSize = sizeof(icc);
#ifndef UNDER_CE //Not needed in CE?
    icc.dwICC = ICC_USEREX_CLASSES; //And add spin back in!
#endif
    InitCommonControlsEx(&icc);

    std::auto_ptr<CMetronomeDlg_RegPersist> autopDlg = 
        (std::auto_ptr<CMetronomeDlg_RegPersist>) new
#ifdef UNDER_CE
        CMetronomeDlg_RegPersist(hInstance, 0);
#else
        CMetronomeDlg_RegPersist_DragSize(hInstance, 0);
#endif
	int nResponse = autopDlg->DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
    autopDlg = (std::auto_ptr<CMetronomeDlg_RegPersist>) NULL;
    
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
