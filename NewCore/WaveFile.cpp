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
// WaveFile.cpp: implementation of the CWaveFile class.
// Originally written by Michael Wolraich http://www.pocketpcdn.com/articles/pocketpcaudio.html
// But his version was too simple to I've modified it beyond recognition:)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveFile.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWaveFile::CWaveFile(LPCTSTR szFileName) :
	m_pFile(0),
	m_nLength(0),
    m_nStartOfSamples(0)
{   //Ref:http://www.borg.com/~jglatt/tech/wave.htm
    
    //Assumptions: "WAVE" follows "RIFF" and does not appear *between* the RIFF header and the *actual* WAVE   block "WAVE" header;
    //Assumptions: "fmt " follows "WAVE" and does not appear *between* the WAVE header and the *actual* format block "fmt " header;
    //Assumptions: "data" follows "fmt " and does not appear *between* the data header and the *actual* data   block "data" header;

    //!!!Notes to self: not sure what all this "rewind" rubbish is, it looks silly in retrospect!
    //                  Also, the "fact" and "PEAK" chunks I'm assuming will be fixed size (e.g., produced from "Audacity" wave editor).

    memset(&m_Format, 0, sizeof(m_Format));

	m_pFile = _tfopen(szFileName, TEXT("rb"));
	if (!m_pFile) return;
	
    char RIFF[4] = {0,0,0,0};
    if (1 != fread(RIFF, sizeof(RIFF), 1, m_pFile)) return;

    if (memcmp(RIFF, "RIFF", sizeof(RIFF)) != 0) return;

    long fileSize = 0; //Unused, but we need to skip over it
    if (1 != fread(&fileSize, sizeof(fileSize), 1, m_pFile)) return;

    char WAVE[4] = {0,0,0,0};
    while (memcmp(WAVE, "WAVE", sizeof(WAVE)) != 0)
    {
        if (1 != fread(WAVE, sizeof(WAVE), 1, m_pFile)) return;
        if (memcmp(WAVE, "WAVE", sizeof(WAVE)) != 0)
        {
            long const Offset = -(((long)sizeof(WAVE))-1);
            fseek(m_pFile, Offset, SEEK_CUR); //rewind and try again...
        }
    }

    char fmt_[4] = {0,0,0,0};
    while (memcmp(fmt_, "fmt ", sizeof(fmt_)) != 0)
    {
        if (1 != fread(fmt_, sizeof(fmt_), 1, m_pFile)) return;
        if (memcmp(fmt_, "fmt ", sizeof(fmt_)) != 0)
        {
            long const Offset = -(((long)sizeof(fmt_))-1);
            fseek(m_pFile, Offset, SEEK_CUR); //rewind and try again...
        }
    }

    long chunkSize = 0;
    if (1 != fread(&chunkSize, sizeof(chunkSize), 1, m_pFile)) return;

    unsigned long BytesToRead = sizeof(m_Format);
    if (1 != fread(&m_Format , BytesToRead, 1, m_pFile)) return;
    m_Format.cbSize = sizeof(m_Format);

    if (m_Format.wBitsPerSample != 16) return; //I'm having to hardcode 16bit samples for reasons (WCE seems to require em)

    long BytesToSkip = chunkSize - (long)BytesToRead;
	fseek(m_pFile, BytesToSkip, SEEK_CUR);

    char data[4] = {0,0,0,0};
    while (memcmp(data, "data", sizeof(data)) != 0)
    {
        if (1 != fread(data, sizeof(data), 1, m_pFile)) return;
        if ((memcmp(data, "fact", sizeof(data)) == 0) || (memcmp(data, "PEAK", sizeof(data)) == 0))
        {
            if (memcmp(data, "fact", sizeof(data)) == 0)
                fseek(m_pFile, 8, SEEK_CUR); //skip fact chunk
            else
                fseek(m_pFile, 20, SEEK_CUR); //skip PEAK chunk
        }
        else
            if (memcmp(data, "data", sizeof(data)) != 0)
                fseek(m_pFile, ((long)sizeof(data))-1, SEEK_CUR); //rewind and try again...
    }
	
    if (1 != fread(&m_nLength, sizeof(m_nLength), 1, m_pFile))
    {
        m_nLength = 0;
        return;
    }

    m_nStartOfSamples = ftell(m_pFile);
}


CWaveFile::~CWaveFile()
{
    if (m_pFile)
	    fclose(m_pFile);
}

long CWaveFile::ReadSamples(__int32* pData, long nLength, long SeekTo)
{
    fseek(m_pFile, m_nStartOfSamples+SeekTo, SEEK_SET);

    __int16 iVal = 0;
    for (long i = 0; i < nLength/2; ++i)
    {
        fread(&iVal, 1, 2, m_pFile);
        pData[i] = iVal;
    }
    return 1;
}
