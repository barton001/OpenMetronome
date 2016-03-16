// Begin code for using TCLAP for command line processing.  Alternatively, there's
// a simple standalone function below (commented out).
//
#include <string>
using namespace std;
#include <iostream>
#include <algorithm>

#ifdef _WINDOWS
#define TCLAP_NAMESTARTSTRING "/"
#define TCLAP_FLAGSTARTSTRING "/"
#endif

#include <tclap/CmdLine.h>

// Override TCLAP's output functions so we display messages using MessageBox
// dialogs instead of just writing to stdout/stderr.
namespace TCLAP {

class TclapMessageBox : public StdOutput
{
	public:
		virtual void failure(CmdLineInterface& c, ArgException& e)
		{ 
			string text = c.getProgramName() + "\nError processing command line: ";
			text += e.error() + "\n" + e.argId();		
			MessageBox(NULL, _T(text.c_str()), _T("Error"), MB_OK);
			exit(1);
		}

		virtual void usage(CmdLineInterface& c)
		{
			string text = c.getProgramName() + " options:\n";
			list<Arg*> args = c.getArgList();
			for (ArgListIterator it = args.begin(); it != args.end(); it++) {
				// Don't show the auto-added options ('//', '/version', etc)
				if ((*it)->longID().substr(0,2) == "//") break;
				text += (*it)->longID();
				text += "  (";
				text += (*it)->getDescription();
				text += ")\n";
			}
			MessageBox(NULL, _T(text.c_str()), _T("Usage"), MB_OK);
		}

		virtual void version(CmdLineInterface& c)
		{
			string text = "Please select the 'About' menu item for version information.";
			MessageBox(NULL, _T(text.c_str()), _T("Info"), MB_OK);
		}
};  // end class TclapMessageBox

}	// end namespace TCLAP

// BHB: Begin ProcessCmdLine (TCLAP version)

void CMetronomeDlg_RegPersist::ProcessCmdLine(void) {
    TCHAR    stringbuf[MAX_BPMEASURE + 1];
	TCLAP::TclapMessageBox useMessageBox;
	try {
		TCLAP::CmdLine cmd("Open Metronome", ':', "");
		cmd.setOutput(&useMessageBox);
		// Add switches (in reverse of order you want them listed in the usage text)
		TCLAP::SwitchArg startArg("S", "start", "start metronome", cmd, false);
		TCLAP::SwitchArg noblinkArg("B","noblink","disable visual beat display", cmd, true);
		TCLAP::SwitchArg blinkArg("b","blink","enable visual beat display", cmd, false);
		TCLAP::ValueArg<string> customArg("c","custom","custom beat string", false, "", "string", cmd);
		TCLAP::ValueArg<int> simpleArg("m","measure","simple measure number of beats", false, -1, "int", cmd);
		TCLAP::SwitchArg straightArg("s","straight", "straight metronome mode", cmd, false);
		TCLAP::ValueArg<string> presetArg("p", "preset", "existing preset to select", false, "", "string", cmd);
		TCLAP::ValueArg<int> tempoArg("t", "tempo", "tempo in beats per minute", false, -1, "int", cmd);
		cmd.parse(__argc, __argv);
		// Check if user requested a particular preset 
		if (presetArg.isSet()) {
			const char *presetName = presetArg.getValue().c_str();
			int entry_index = ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_FINDSTRINGEXACT, 0, (long)presetName);
			if (entry_index < 0) {
				MessageBox(NULL, _T("Preset name not found"), _T("Error"), MB_OK);
			} else {
				// Load the requested preset
				LoadSettings(presetName);
				// Make the selection box realize that it should have that entry selected.
				::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_SETCURSEL, entry_index, 0);
			}
		}
		// Check for tempo argument next, allowing it to override tempo in preset
		if (tempoArg.isSet()) {
			unsigned long newTempo = (unsigned long)abs(tempoArg.getValue());
			if (newTempo >= m_MinBPM && newTempo <= m_MaxBPM) {
				m_BPMinute = newTempo;
				// Update GUI with new value
				_itot(m_BPMinute, stringbuf, 10);
				::SetWindowText(GET_HWND(IDC_BPMINUTE_EDIT), stringbuf);
				::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETPOS, TRUE, BPMToSlider(m_BPMinute));
			}
		}
		// Check for straight metronome mode
		if (straightArg.isSet()) {
			m_MetronomeStyle = metPlain;
		}
		// Check for simple measure length
		if (simpleArg.isSet()) {
			int beatsPerMeasure = simpleArg.getValue();
			if ((beatsPerMeasure > 0) && (beatsPerMeasure < 100)) {
				m_BPMeasure = beatsPerMeasure;
				_itot(m_BPMeasure, stringbuf, 10);
				::SetWindowText(GET_HWND(IDC_BPMEASURE_EDIT), stringbuf);
				m_MetronomeStyle = metMeasure;
			}
		}
		// Check for custom string
		if (customArg.isSet()) {
			::SetWindowText(*m_autopGroupEdit.get(), customArg.getValue().c_str());
			m_MetronomeStyle = metGroup;
		}
		// Update GUI for selected operating mode
		if(m_MetronomeStyle == metPlain) 
        {
            ::SendMessage(GET_HWND(IDC_RADIO_PLAIN  ), BM_SETCHECK, BST_CHECKED  , 0);
            ::SendMessage(GET_HWND(IDC_RADIO_MEASURE), BM_SETCHECK, BST_UNCHECKED, 0);
            ::SendMessage(GET_HWND(IDC_RADIO_GROUP  ), BM_SETCHECK, BST_UNCHECKED, 0);
            OnRadioPlain(0,0);
        }
        else if(m_MetronomeStyle == metMeasure)
        {
            ::SendMessage(GET_HWND(IDC_RADIO_PLAIN  ), BM_SETCHECK, BST_UNCHECKED, 0);
            ::SendMessage(GET_HWND(IDC_RADIO_MEASURE), BM_SETCHECK, BST_CHECKED  , 0);
            ::SendMessage(GET_HWND(IDC_RADIO_GROUP  ), BM_SETCHECK, BST_UNCHECKED, 0);
            OnRadioMeasure(0,0);
        }
        else
        {
            ::SendMessage(GET_HWND(IDC_RADIO_PLAIN  ), BM_SETCHECK, BST_UNCHECKED, 0);
            ::SendMessage(GET_HWND(IDC_RADIO_MEASURE), BM_SETCHECK, BST_UNCHECKED, 0);
            ::SendMessage(GET_HWND(IDC_RADIO_GROUP  ), BM_SETCHECK, BST_CHECKED  , 0);
            OnRadioGroup(0,0);
        }
		// Check for blink on/off switches
		if (blinkArg.isSet() || noblinkArg.isSet()) {
			if (blinkArg.isSet()) m_blinking = blinkArg.getValue();
			if (noblinkArg.isSet()) m_blinking = noblinkArg.getValue();
		    ::SendMessage(GET_HWND(IDC_BLINK_CHECK), BM_SETCHECK, m_blinking?BST_CHECKED:BST_UNCHECKED, 0);
			OnBlinkCheck(0,0);
		}
		if (startArg.isSet()) Play();
	}
	catch (TCLAP::ArgException &e) {
		MessageBox(NULL, _T(e.what()), _T("Error"), MB_OK);
	}
}


// BHB: End ProcessCmdLine (TCLAP version)



// Here's a simple command line processor function that doesn't have any 
// outside libraries (only handles -t and -p options so far).
/*

#include <string>
using namespace std;


void CMetronomeDlg_RegPersist::ProcessCmdLine(void) {
	vector<string> args(__argv + 1, __argv + __argc);
	for (vector<string>::iterator i = args.begin(); i != args.end(); i++) {
		// Debug: // MessageBox(NULL, (*i).c_str(), _T("Info"), MB_OK);
		vector<string>::iterator nextArg = i + 1;
		if ((*i == "-t" || *i == "--tempo") && (nextArg != args.end())) {
			int newTempo = atoi((*++i).c_str());
			if (newTempo >= m_MinBPM && newTempo <= m_MaxBPM) {
				m_BPMinute = newTempo;
				// Update GUI with new value
				::SetWindowText(GET_HWND(IDC_BPMINUTE_EDIT), (*i).c_str());
				::SendMessage(GET_HWND(IDC_BPMINUTE_SLIDER), TBM_SETPOS, TRUE, BPMToSlider(m_BPMinute));
			} else {
				MessageBox(NULL, _T("Argument to -t switch is invalid or out of range.  Should be a decimal value representing the tempo in beats per minute."), _T("Error"), MB_OK);
			}
		} else if ((*i == "-p" || *i == "--preset") && (nextArg != args.end())) {
			// Figure out the index if this preset exists
			i++;
            int entry_index = ::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_FINDSTRINGEXACT, 0, (long)(*i).c_str());
			if (entry_index < 0) {
				MessageBox(NULL, _T("Preset name not found"), _T("Error"), MB_OK);
			} else {
				// Load the requested preset
				LoadSettings((*i).c_str());
				// Make the selection box realize that it should have that entry selected.
				::SendMessage(GET_HWND(IDC_PRESET_COMBO), CB_SETCURSEL, entry_index, 0);
			}
		} else {
			MessageBox(NULL, _T("Error processing command line. Valid options are:\n-t n      Set tempo to n beats per minute\n-p x      Set preset to x\n"), _T("Error"), MB_OK);
		}
	}
}
*/