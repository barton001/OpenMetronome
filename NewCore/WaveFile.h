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

// WaveFile.h: interface for the CWaveFile class.
// Originally written by Michael Wolraich http://www.pocketpcdn.com/articles/pocketpcaudio.html
//////////////////////////////////////////////////////////////////////

#include "mmsystem.h"

#if !defined(AFX_WAVEFILE_H__3CC6B20B_2473_4C12_8BA0_E78B388A19D7__INCLUDED_)
#define AFX_WAVEFILE_H__3CC6B20B_2473_4C12_8BA0_E78B388A19D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//Very simple WAV file reader, makes a number of assumptions about the format being read
//that may not necessarily be the case!
//Specifically, I'm currently using mono uncompressed PCM WAV files 44.1kHz
//(16-bits-unsigned-int-samples) produced by "Audacity" (an open-source WAV editor).
//This class is does work with that format.
//See .cpp for details.  hopefully the class is self explanatory.
class CWaveFile  
{
public:
	CWaveFile(LPCTSTR szFileName = 0);
	virtual ~CWaveFile();

    //Seek to is relative to start of samples, not start of file
    //See also main class comment:
    //The samples are used to populate a buffer of __int32, regardless of the orignal sample-format:
    //This is an application-specific thing: my application likes to play __int32 samples; it can't
    //handle many other sample-formats. 
	long ReadSamples(__int32* pData, long nLength, long SeekTo = 0);

    long GetLength() {return m_nLength;} //(Bytes) Returns zero if constructor failed

    WAVEFORMATEX* GetWaveFormat()	{return (&m_Format);}//Returns 0 struct if it could not be read

protected:
	FILE* m_pFile;
	long m_nLength;
	WAVEFORMATEX m_Format;
    long m_nStartOfSamples;
};

#endif // !defined(AFX_WAVEFILE_H__3CC6B20B_2473_4C12_8BA0_E78B388A19D7__INCLUDED_)
