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
#ifndef __GENERAL_MIDI_H
#define __GENERAL_MIDI_H

///////////////////////////////////////////////////////////////
// The instruments in the general midi track 10 instrument set
// I got this list from:
//
// http://www.midi.org/about-midi/gm/gm1sound.htm
///////////////////////////////////////////////////////////////

enum GM1Track10Enum
{
	GM1_ACOUSTIC_BASS_DRUM	= 0,
	GM1_BASS_DRUM_1			= 1,
	GM1_SIDE_STICK			= 2,
	GM1_ACOUSTIC_SNARE		= 3,
	GM1_HAND_CLAP			= 4,
	GM1_ELECTRIC_SNARE		= 5,
	GM1_LOW_FLOOR_TOM		= 6,
	GM1_CLOSED_HI_HAT		= 7,
	GM1_HIGH_FLOOR_TOM		= 8,
	GM1_PEDAL_HI_HAT		= 9,
	GM1_LOW_TOM				= 10,
	GM1_OPEN_HI_HAT			= 11,
	GM1_LOW_MID_TOM			= 12,
	GM1_HI_MID_TOM			= 13,
	GM1_CRASH_CYMBAL_1		= 14,
	GM1_HIGH_TOM			= 15,
	GM1_RIDE_CYMBAL_1		= 16,
	GM1_CHINESE_CYMBAL		= 17,
	GM1_RIDE_BELL			= 18,
	GM1_TAMBOURINE			= 19,
	GM1_SPLASH_CYMBAL		= 20,
	GM1_COWBELL				= 21,
	GM1_CRASH_CYMBAL_2		= 22,
	GM1_VIBRASLAP			= 23,
	GM1_RIDE_CYMBAL_2		= 24,
	GM1_HI_BONGO			= 25,
	GM1_LOW_BONGO			= 26,
	GM1_MUTE_HI_CONGA		= 27,
	GM1_OPEN_HI_CONGA		= 28,
	GM1_LOW_CONGA			= 29,
	GM1_HIGH_TIMBALE		= 30,
	GM1_LOW_TIMBALE			= 31,
	GM1_HIGH_AGOGO			= 32,
	GM1_LOW_AGOGO			= 33,
	GM1_CABASA				= 34,
	GM1_MARACAS				= 35,
	GM1_SHORT_WHISTLE		= 36,
	GM1_LONG_WHISTLE		= 37,
	GM1_SHORT_GUIRO			= 38,
	GM1_LONG_GUIRO			= 39,
	GM1_CLAVES				= 40,
	GM1_HI_WOOD_BLOCK		= 41,
	GM1_LOW_WOOD_BLOCK		= 42,
	GM1_MUTE_CUICA			= 43,
	GM1_OPEN_CUICA			= 44,
	GM1_MUTE_TRIANGLE		= 45,
	GM1_OPEN_TRIANGLE		= 46
};

enum AlphabeticalTrack10Enum
{
	ALPHA_HIGH_AGOGO			= 0,
	ALPHA_LOW_AGOGO				= 1,
	ALPHA_ACOUSTIC_BASS_DRUM	= 2,
	ALPHA_RIDE_BELL				= 3,
	ALPHA_HI_BONGO				= 4,
	ALPHA_LOW_BONGO				= 5,
	ALPHA_CABASA				= 6,
	ALPHA_HAND_CLAP				= 7,
	ALPHA_CLAVES				= 8,
	ALPHA_COWBELL				= 9,
	ALPHA_MUTE_CUICA			= 10,
	ALPHA_OPEN_CUICA			= 11,
	ALPHA_BASS_DRUM_1			= 12,
	ALPHA_MUTE_HI_CONGA			= 13,
	ALPHA_OPEN_HI_CONGA			= 14,
	ALPHA_LOW_CONGA				= 15,
	ALPHA_CHINESE_CYMBAL		= 16,
	ALPHA_CRASH_CYMBAL_1		= 17,
	ALPHA_CRASH_CYMBAL_2		= 18,
	ALPHA_RIDE_CYMBAL_1			= 19,
	ALPHA_RIDE_CYMBAL_2			= 20,
	ALPHA_SPLASH_CYMBAL			= 21,
	ALPHA_SHORT_GUIRO			= 22,
	ALPHA_LONG_GUIRO			= 23,
	ALPHA_CLOSED_HI_HAT			= 24,
	ALPHA_OPEN_HI_HAT			= 25,
	ALPHA_PEDAL_HI_HAT			= 26,
	ALPHA_MARACAS				= 27,
	ALPHA_SIDE_STICK			= 28,
	ALPHA_ACOUSTIC_SNARE		= 29,
	ALPHA_ELECTRIC_SNARE		= 30,
	ALPHA_TAMBOURINE			= 31,
	ALPHA_HIGH_TIMBALE			= 32,
	ALPHA_LOW_TIMBALE			= 33,
	ALPHA_LOW_FLOOR_TOM			= 34,
	ALPHA_HIGH_FLOOR_TOM		= 35,
	ALPHA_LOW_TOM				= 36,
	ALPHA_LOW_MID_TOM			= 37,
	ALPHA_HI_MID_TOM			= 38,
	ALPHA_HIGH_TOM				= 39,
	ALPHA_MUTE_TRIANGLE			= 40,
	ALPHA_OPEN_TRIANGLE			= 41,
	ALPHA_VIBRASLAP				= 42,
	ALPHA_SHORT_WHISTLE			= 43,
	ALPHA_LONG_WHISTLE			= 44,
	ALPHA_HI_WOOD_BLOCK			= 45,
	ALPHA_LOW_WOOD_BLOCK		= 46
};

// THe number of possible voices in the general midi track 10 set
#define GM1_TRACK10_NUM_VOICES (GM1_OPEN_TRIANGLE - GM1_ACOUSTIC_BASS_DRUM + 1)

// The first instrument is actually at 35, not 0, so this offset has to 
// be added the constants before the instrument # is or'd into 
// the midi "note on" event
#define GM1_TRACK10_OFFSET 35
#define TO_MIDI(x) (x + GM1_TRACK10_OFFSET)

extern TCHAR AlphaInstrumentStrings[][GM1_TRACK10_NUM_VOICES];
extern int  AlphaToMidi[GM1_TRACK10_NUM_VOICES];
extern int  MidiToAlpha[GM1_TRACK10_NUM_VOICES];

#endif // __GENERAL_MIDI_H

