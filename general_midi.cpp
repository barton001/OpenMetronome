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
#include "general_midi.h"

//////////////////////////////////////////////////////////////
// The table of strings describing the different instruments
// on track 10 of the General Midi Level 1 standard.  I got
// these names from the web at:
//
// http://www.midi.org/about-midi/gm/gm1sound.htm
//////////////////////////////////////////////////////////////

TCHAR AlphaInstrumentStrings[][GM1_TRACK10_NUM_VOICES] =
{
	_T("High Agogo"),          //_T("Agogo Agudo"),           
	_T("Low Agogo"),           //_T("Agogo Grave"),           
	_T("Acoustic Bass Drum"),  //_T("Bombo Acústico"),        
	_T("Ride Bell"),           //_T("Campana de Ride"),       
	_T("Hi Bongo"),            //_T("Bongo Agudo"),           
	_T("Low Bongo"),           //_T("Bongo Grave"),           
	_T("Cabasa"),              //_T("Cabasa"),                
	_T("hand clap"),           //_T("Palmada"),               
	_T("Claves"),              //_T("Claves"),                
	_T("Cowbell"),             //_T("Cencerro"),              
	_T("Mute Cuica"),          //_T("Cuica Tapada"),          
	_T("Open Cuica"),          //_T("Cuica Abierta"),         
	_T("Bass Drum 1"),         //_T("Bombo 1"),               
	_T("Mute Hi Conga"),       //_T("Conga Aguda Tapada"),    
	_T("Open Hi Conga"),       //_T("Conga Aguda Abierta"),   
	_T("Low Conga"),           //_T("Conga Grave"),           
	_T("Chinese Cymbal"),      //_T("Plato de Corte Chino"),  
	_T("Crash Cymbal 1"),      //_T("Plato de Corte 1"),      
	_T("Crash Cymbal 2"),      //_T("Plato de Corte 2"),      
	_T("Ride Cymbal 1"),       //_T("Plato Ride 1"),          
	_T("Ride Cymbal 2"),       //_T("Plato Ride 2"),          
	_T("Splash Cymbal"),       //_T("Plato de Corte Splash"), 
	_T("Short Guiro"),         //_T("Guiro Corto"),           
	_T("Long Guiro"),          //_T("Guiro Largo"),           
	_T("Closed Hi-hat"),       //_T("Charles Cerrado"),       
	_T("Open Hi-hat"),         //_T("Charles Abierto"),       
	_T("Pedal Hi-hat"),        //_T("Pedal de Charles"),      
	_T("Maracas"),             //_T("Maracas"),               
	_T("Side Stick"),          //_T("Aro de Caja"),           
	_T("Acoustic Snare"),      //_T("Caja Acústica"),         
	_T("Electric Snare"),      //_T("Caja Eléctrica"),        
	_T("Tambourine"),          //_T("Pandereta"),             
	_T("High Timbale"),        //_T("Timbal Agudo"),          
	_T("Low Timbale"),         //_T("Timbal Grave"),          
	_T("Low Floor Tom"),       //_T("Timbala de Suelo Grave"),
	_T("High Floor Tom"),      //_T("Timbala de Suelo Aguda"),
	_T("Low Tom"),             //_T("Timbal Grave"),          
	_T("Low-Mid tom"),         //_T("Timbal Grave-Medio"),    
	_T("Hi Mid Tom"),          //_T("Timbal Agudo-Medio"),    
	_T("High Tom"),            //_T("Timbal Agudo"),          
	_T("Mute Triangle"),       //_T("Triángulo Tapado"),      
	_T("Open Triangle"),       //_T("Triángulo Abierto"),     
	_T("Vibraslap"),           //_T("VibraSlap"),             
	_T("Short Whistle"),       //_T("Silbido Corto"),         
	_T("Long Whistle"),        //_T("Silbido Largo"),         
	_T("Hi Wood Block"),       //_T("Bloque de Madera Agudo"),
	_T("Low Wood Block")       //_T("Bloque de Madera Grave")
};

// the index into this list is the alphabetical order.  The value of each entry is the
// corresponding MIDI index.
int AlphaToMidi[GM1_TRACK10_NUM_VOICES] = 
{
	GM1_HIGH_AGOGO,
	GM1_LOW_AGOGO,
	GM1_ACOUSTIC_BASS_DRUM,
	GM1_RIDE_BELL,
	GM1_HI_BONGO,
	GM1_LOW_BONGO,
	GM1_CABASA,
	GM1_HAND_CLAP,
	GM1_CLAVES,
	GM1_COWBELL,
	GM1_MUTE_CUICA,
	GM1_OPEN_CUICA,
	GM1_BASS_DRUM_1,
	GM1_MUTE_HI_CONGA,
	GM1_OPEN_HI_CONGA,
	GM1_LOW_CONGA,
	GM1_CHINESE_CYMBAL,
	GM1_CRASH_CYMBAL_1,
	GM1_CRASH_CYMBAL_2,
	GM1_RIDE_CYMBAL_1,
	GM1_RIDE_CYMBAL_2,
	GM1_SPLASH_CYMBAL,
	GM1_SHORT_GUIRO,
	GM1_LONG_GUIRO,
	GM1_CLOSED_HI_HAT,
	GM1_OPEN_HI_HAT,
	GM1_PEDAL_HI_HAT,
	GM1_MARACAS,
	GM1_SIDE_STICK,
	GM1_ACOUSTIC_SNARE,
	GM1_ELECTRIC_SNARE,
	GM1_TAMBOURINE,
	GM1_HIGH_TIMBALE,
	GM1_LOW_TIMBALE,
	GM1_LOW_FLOOR_TOM,
	GM1_HIGH_FLOOR_TOM,
	GM1_LOW_TOM,
	GM1_LOW_MID_TOM,
	GM1_HI_MID_TOM,
	GM1_HIGH_TOM,
	GM1_MUTE_TRIANGLE,
	GM1_OPEN_TRIANGLE,
	GM1_VIBRASLAP,
	GM1_SHORT_WHISTLE,
	GM1_LONG_WHISTLE,
	GM1_HI_WOOD_BLOCK,
	GM1_LOW_WOOD_BLOCK
};

int MidiToAlpha[GM1_TRACK10_NUM_VOICES] = 
{
	ALPHA_ACOUSTIC_BASS_DRUM,
	ALPHA_BASS_DRUM_1,
	ALPHA_SIDE_STICK,
	ALPHA_ACOUSTIC_SNARE,
	ALPHA_HAND_CLAP,
	ALPHA_ELECTRIC_SNARE,
	ALPHA_LOW_FLOOR_TOM,
	ALPHA_CLOSED_HI_HAT,
	ALPHA_HIGH_FLOOR_TOM,
	ALPHA_PEDAL_HI_HAT,
	ALPHA_LOW_TOM,
	ALPHA_OPEN_HI_HAT,
	ALPHA_LOW_MID_TOM,
	ALPHA_HI_MID_TOM,
	ALPHA_CRASH_CYMBAL_1,
	ALPHA_HIGH_TOM,
	ALPHA_RIDE_CYMBAL_1,
	ALPHA_CHINESE_CYMBAL,
	ALPHA_RIDE_BELL,
	ALPHA_TAMBOURINE,
	ALPHA_SPLASH_CYMBAL,
	ALPHA_COWBELL,
	ALPHA_CRASH_CYMBAL_2,
	ALPHA_VIBRASLAP,
	ALPHA_RIDE_CYMBAL_2,
	ALPHA_HI_BONGO,
	ALPHA_LOW_BONGO,
	ALPHA_MUTE_HI_CONGA,
	ALPHA_OPEN_HI_CONGA,
	ALPHA_LOW_CONGA,
	ALPHA_HIGH_TIMBALE,
	ALPHA_LOW_TIMBALE,
	ALPHA_HIGH_AGOGO,
	ALPHA_LOW_AGOGO,
	ALPHA_CABASA,
	ALPHA_MARACAS,
	ALPHA_SHORT_WHISTLE,
	ALPHA_LONG_WHISTLE,
	ALPHA_SHORT_GUIRO,
	ALPHA_LONG_GUIRO,
	ALPHA_CLAVES,
	ALPHA_HI_WOOD_BLOCK,
	ALPHA_LOW_WOOD_BLOCK,
	ALPHA_MUTE_CUICA,
	ALPHA_OPEN_CUICA,
	ALPHA_MUTE_TRIANGLE,
	ALPHA_OPEN_TRIANGLE
};