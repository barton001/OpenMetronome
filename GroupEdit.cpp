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
// GroupEdit.cpp : implementation file
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupEdit.h"

#ifdef _DEBUG
#ifdef _AFX
#define new DEBUG_NEW
#endif
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//--------------------------------------------------------------------------------------------------
CGroupEdit::CGroupEdit(HWND hWnd, bool const bDeferCreation) : 
    CBscSubclassWnd(hWnd, true)
{
    ADD_MSG_HANDLER_SC(WM_CHAR, OnChar);

    if (!bDeferCreation)
        CreateIt();
}
//--------------------------------------------------------------------------------------------------


CGroupEdit::~CGroupEdit()
{
}
//--------------------------------------------------------------------------------------------------


long CGroupEdit::OnChar(unsigned long const nChar, long const lKeyData)
{
	if ((nChar >= '0' && nChar <= '9') ||
		nChar == '(' ||
		nChar == ')' ||
		nChar == '[' || //Advanced syntax
		nChar == ']' || //Advanced syntax 
		nChar == '*' || // BHB - Advanced syntax
		nChar == ' ' ||
		nChar == 0x18 || //Cut
		nChar == 0x16 || //Paste
		nChar == 0x03 || //Copy
		nChar == 0x1a || //Undo
		nChar == '@' || //Advanced syntax 
		nChar == '+' || //Advanced syntax 
		nChar == '-' || //Advanced syntax 
		nChar == '%' || //Advanced syntax 
		nChar == ((char)'//') || //Advanced syntax 
		nChar == '\b')    // backspace
		return CallWindowProc((WNDPROC)m_lOriginalProc, m_hWnd, WM_CHAR, nChar, lKeyData);
	else
		return 0;
}

//--------------------------------------------------------------------------------------------------


// returns true if it's a valid character for the group string
enum group_char_type {gct_invalid = 0, gct_numeral, gct_open_paren, gct_close_paren, gct_space};
static group_char_type GetGroupCharacterType(TCHAR character)
{
	if (character >= _T('0') && character <= _T('0') + MAX_SOUNDS)
		return gct_numeral;
	else if (character == _T('('))
		return gct_open_paren;
	else if (character == _T(')'))
		return gct_close_paren;
	else if (character == _T(' '))
		return gct_space;
	else return gct_invalid;
}
//--------------------------------------------------------------------------------------------------


// returns an error if the group string is invalid
CGroupEdit::analyse_groups_error CGroupEdit::Parse(std::vector<group_beat> * pBeatData, unsigned long * pBPMeasure, std::basic_string<TCHAR> * pstrAdvancedCfg)
{
	int string_length = ::GetWindowTextLength(m_hWnd);
    std::basic_string<TCHAR> stringbuf;
    stringbuf.resize(string_length + 2, 0);
	::GetWindowText(m_hWnd, &*stringbuf.begin(), string_length+1);
    std::string::size_type itSqareBracket1 = stringbuf.find('[');
    if (itSqareBracket1 != std::string::npos)
    {
        if (itSqareBracket1 != 0)
            return ag_misplaced_paren;
        std::string::size_type itSqareBracket2 = stringbuf.find(']');
        if (itSqareBracket1 == std::string::npos)
            return ag_unclosed_paren;
        *pstrAdvancedCfg = stringbuf.substr(0, itSqareBracket2+1);
        stringbuf = stringbuf.substr(itSqareBracket2+1);
        string_length = stringbuf.length();
        //!!!:check for "@", "//", ordering etc.
    }

    int num_beats = 0, next_string_index = 0, substring_len = 0;

	// remove all the spaces from the string.
    for (unsigned long i = 0; i < stringbuf.length(); ++i)
    {
        if (stringbuf[i] == _T(' '))
            stringbuf.erase(i, 1);
    }

	while(stringbuf.c_str()[next_string_index] != _T('\0'))
	{
		//NB the next statement relies on characteristics of ascii: numbers are in order from 0 to 9
		group_char_type char_type = GetGroupCharacterType(stringbuf.c_str()[next_string_index]);
		
		//process this beat
		switch(char_type)
		{
		case gct_numeral:
			//If it's a numeral, subtract the character '1' from each letter to give the index into
            //the instrument array.  Thus, the string _T("123" is mapped to 0,1,2. This way '0'
            //ends up being -1, which will indicate a silent beat.

            if (num_beats >= ((int)pBeatData->size()))
                pBeatData->resize(pBeatData->size()*2);
			(*pBeatData)[num_beats++].set_single_value(stringbuf.c_str()[next_string_index++] - _T('1'));
			break;

		case gct_open_paren:
			//This is the start of a substring.  Find the end & pass into the beat data array

			next_string_index++; // the start of the substring

			// find the end of the substring by finding the first non-numeric character
			substring_len = _tcsspn(stringbuf.c_str() + next_string_index, _T("0123456789"));

			// if this substring isn't finished with a close paren or it's too long then return error
			if(substring_len > MAX_GROUP_SUBSTRING)
				return ag_substring_overrun;
			if(GetGroupCharacterType(stringbuf.c_str()[next_string_index + substring_len]) != gct_close_paren)
				return ag_unclosed_paren;

			// replace the close paren with 0 to truncate the string
			stringbuf.begin()[next_string_index + substring_len] = 0;
            if (num_beats >= ((int)pBeatData->size()))
                pBeatData->resize(pBeatData->size()*2);
            (*pBeatData)[num_beats++].set_multiple_value(stringbuf.c_str() + next_string_index);

			next_string_index += substring_len + 1;
			break;

		case gct_space:
			next_string_index++;
			break;

		case gct_invalid:
		case gct_close_paren:

			// we should never encounter anything other than a numeral, a space, or an open paren in 
			// this loop, so return failure if we do.
			return ag_misplaced_paren;
		}
	}

	if(0 == num_beats)
	{
		return ag_no_beats;
	}

	*pBPMeasure = num_beats;

	return ag_success;
}
//--------------------------------------------------------------------------------------------------
