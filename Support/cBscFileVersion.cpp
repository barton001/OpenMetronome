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
// FileVersion.cpp: implementation of the NFileVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <memory>
#include <vector>
//#include "cBscCatch.h" //Open-source-land can't have this:) Also slows down PPCs, apparently
#include "cBscFileVersion.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//--------------------------------------------------------------------------------------------------


void cBscFileVersion::Get(TCHAR * pBuf, LPCTSTR strFilename)
{
    _tcscpy(pBuf, _T("#.#u"));
    //cBscTry
    {
        TCHAR strFilenameToUse[MAX_PATH];

        bool bFilenameOk = false;
        if (strFilename)
        {
            if (strFilename[0])
            {
                bFilenameOk = true;
                _tcscpy(strFilenameToUse, strFilename);
            }
        }
        if (!bFilenameOk)
	        GetModuleFileName(NULL, strFilenameToUse, MAX_PATH);

        unsigned long dwArg = 0;
        unsigned long dwInfoSize = GetFileVersionInfoSize(strFilenameToUse, &dwArg);

        if (dwInfoSize > 0)    
        {   //Making it a buff of ulong is 4 times too big, but WCE on ARM explodes otherwise:/
            std::vector<unsigned long> lpBuff; 
            lpBuff.resize(dwInfoSize);
    
            if(0 != GetFileVersionInfo(strFilenameToUse, 0, dwInfoSize, &*lpBuff.begin()))
            {
                VS_FIXEDFILEINFO *vInfo = 0;
    
                unsigned int uInfoSize = 0;
    
                if(0 != VerQueryValue(&*lpBuff.begin(), _T("\\"), (LPVOID*)&vInfo, &uInfoSize))
                {
                    if(0 != uInfoSize)
                    {
                        TCHAR MajorVersion[3];
                        TCHAR MinorVersion[3];
                        _stprintf(MajorVersion, _T("%d"), HIWORD(vInfo->dwFileVersionMS));
                        _stprintf(MinorVersion, _T("%02d"), LOWORD(vInfo->dwFileVersionMS));
                        TCHAR Modifier[8] = {0x00};
                        if (HIWORD(vInfo->dwFileVersionLS))
                        {
                            switch(HIWORD(vInfo->dwFileVersionLS))
                            {
                            case 1:{_tcscpy(Modifier, _T("A#."));break;}
                            case 2:{_tcscpy(Modifier, _T("B#."));break;}
                            case 3:{_tcscpy(Modifier, _T("rc#"));break;}
                            default:{break;}
                            }
                            TCHAR Tmp[3];
                            _stprintf(Tmp, _T("%02d"), LOWORD(vInfo->dwFileVersionLS));
                            _tcscat(Modifier, Tmp);
                        }
                        else
                            if (LOWORD(vInfo->dwFileVersionLS))
                                _tcscpy(Modifier, _T("u# "));

                        *pBuf = 0x00;
                        _tcscat(pBuf, MajorVersion);
                        _tcscat(pBuf, _T("."));
                        _tcscat(pBuf, MinorVersion);
                        _tcscat(pBuf, Modifier);
                    }    
                }    
            }
        }
    }
    //cBscCatchLogAndRethrow(_T("Get(") << strFilename << _T(")"));
}
//--------------------------------------------------------------------------------------------------
