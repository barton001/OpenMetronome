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
#if !defined(AFX_CBscBblDlg_H__E852D391_8F43_4DDB_89A3_65FCC24D01B0__INCLUDED_)
#define AFX_CBscBblDlg_H__E852D391_8F43_4DDB_89A3_65FCC24D01B0__INCLUDED_
// CBscBblDlg.h: interface for the CBscBblDlg class.
//
// Author: M.T.Billington (24-Jun-2004)
////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <map>

//I used to use dynamic cast here, but it seemed to cause problems in PPC2002 and earlier...
#define DECLARE_MSG_HANDLER(MyHandlerName, MyHandlerClass) virtual long MyHandlerName(unsigned long const wParam, long const lParam); static long stub##MyHandlerName (unsigned long const wParam, long const lParam, CBscBblDlg * pThis){return ((MyHandlerClass*)(pThis))->MyHandlerName(wParam,lParam);};
#define ADD_MSG_HANDLER(Msg, MyHandlerName) AddMsgHandler(Msg, stub##MyHandlerName);

//As "ADD_MSG_HANDLER", but for, e.g., button presses, so put the ID of the control you want to 
//handle as the Msg param
//!!!ADD_CMD_HANDLER is a bit inelegant for handling EN_ messages...
#define ADD_CMD_HANDLER(Msg, MyHandlerName) AddCmdHandler(Msg, stub##MyHandlerName);


#define GET_HWND(x) GetDlgItem(*this, x) //A bit nasty to use a macro:)


//My equivalent of MFC's CDialog.  This exists primarily so that I can divorce my code from the MFC,
//hopefully making it more cross-platformy, and allowing me at the very least to use MS's Visual C++
//2005 Express, the free (but non-mfc) version of their C++ compiler.
//
//Example of usage (you don't have to derived your own class, can use as-is if that works for you):
//1) Derive a class from this one, e.g., CBscBblDlg_Custom
//2) Implement your derived constructor like this:
//CBscBblDlg_Custom::CBscBblDlg_Custom(WORD wTemplate, HINSTANCE hInstance, HWND hParent, 
//                    bool const bCreateModelessOnConstruction, bool const bDeferModelessCreation) :
//    CBscBblDlg(wTemplate, hInstance, hParent, bCreateModelessOnConstruction, true /*MUST*==true*/)
//{
//    //Your custom-construction code (if any, e.g., ADD_MSG_HANDLER calls) goes here
//
//    if ((bCreateModelessOnConstruction) && (!bDeferModelessCreation))
//        CreateIt();
//}
//3) (Optional) add any messge handlers for messages that you want to handle
//e.g.,
//To handle the WM_DESTROY message in a dialog class of your own:
//Add this to your derived class .h, in the "Protected" section:
//DECLARE_MSG_HANDLER(OnDestroy, CBscBblDlg_Custom);
//Add this to your derived class .cpp, in the constructor, BEFORE "CreateIt" is called (so for
//modal dialogs, this is automatic, for modeless ones, it needs to happen before you call CreateIt):
//ADD_MSG_HANDLER(WM_DESTROY, OnDestroy);
//Add a function to your CPP that will be called when WM_DESTROY is received:
//long CFSLDlg::OnDestroy(unsigned long const nNULL, long const lNULL)
//{
//    Your code goes here
//    return 1;
//}
//4) (Optional) add any WM_COMMAND messages that you'd like to handle (you could do this as above, 
//and implement a big switch statement to handle the WM_COMMANDs from different dialog controls,
//but doing it as described below will make life easier!
//e.g., to handle a button press from a button with ID IDC_BUTTON_CONNECT:
//Do as per 3), above, but instead of using ADD_MSG_HANDLER, use ADD_CMD_HANDLER:
//ADD_CMD_HANDLER(IDC_BUTTON_CONNECT, OnButtonConnect);
//Your handler only gets the high half of the wParam, as the low half is the IDC_ id (which you can
//infer, as your handler function only gets called for the IDC_ you specify).
//There isn't currently any scope for defining a single handler to cope with messages from mulitple
//controls (well you can do it but you can't know which control was activated).
//5)Your class is now ready to use:
//a) Modal usage (similar to MFC CDialog::DoModal):
//Create an instance of your dialog, making sure to set bCreateModelessOnConstruction false, then
//just do myIntance.DoModal().
//b) Modeless usage:
//Create an instance of your dialog, setting bCreateModelessOnConstruction true.
//
// NOTE: USERS OF THIS CLASS ABSOLUTELY MUST NOT USE SetWindowLong() ON THE CLASS's HWND!
// I use the WindowLong internally to store pThis.  There may be a better way to do it, I'm not sure.
class CBscBblDlg
{
public:
    //See main class comment
	CBscBblDlg(WORD wTemplate = 0,
               HINSTANCE hInstance = 0, //E.g., (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE)
               HWND hParent = 0, //E.g., GetDesktopWindow()
               bool const bCreateModelessOnConstruction = false, 
               bool const bDeferModelessCreation = false);


    //See main class comment
	virtual ~CBscBblDlg();


    //See main class comment;
    //Analogous to MFC's CDialog::DoModal().
    //The returns IDOK IDCANCEL etc, depending on how the dialog was terminated.
    virtual long DoModal();

    
    //Allows you to use your class instance as an HWND, e.g., 
    //ShowWindow(myInstance, true);
    virtual operator HWND() const {return m_hWnd;};


    //Returns the window text of the supplied control ID
    //This is just because it is slightly more convenient than allocating buffers and having the
    //windows API populate them for you.
    virtual std::basic_string<TCHAR> GetWindowText(unsigned long ctrlID);


    //SetWindowText: Just do ::SetWindowText(GET_HWND(IDD_MY_CONTROL_ID), _T("Whatever"));


protected:
    //This is the function specified to Win32's "DialogBoxParam" function: it handles the messages
    //that we're interested in, and lets Windows handle the rest.
    //Returns either the value returned by the message handler that it ends up calling, or retruns
    //0 (indicating to Windows that it should handle the message).
    virtual LRESULT MyMsgMap(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    virtual long OnInitDialog(); //Analogouse to CDialog::OnInitDialog: Override to do your own init
    virtual long OnOK();         //Analogouse to CDialog::OnOK: Override to do your own OnOK handler
    virtual long OnCancel();     //Analogouse to CDialog::OnCancel: Override as above...

    //This is used specifically to handle WM_COMMAND messages: It calls the function you defined with
    //ADD_CMD_HANDLER
    virtual long OnCommand(unsigned short const CtrlID, 
                           unsigned short const HiWordWparam, LPARAM const lParam);


    //See main class comment: these functions are used by the macro at the top of the file to define
    //and implement the message map.
    typedef long (*MsgHandler)(unsigned long const wParam, long const lParam, CBscBblDlg * pvThis);
    virtual void AddMsgHandler(unsigned long  const Msg   , MsgHandler pfnHandleMsg);
    virtual void AddCmdHandler(unsigned short const CtrlID, MsgHandler pfnHandleMsg);

    virtual void CreateIt(); //Associated MyMsgMap() with m_hWnd so that it is called to handle msgs


    HWND m_hWnd;                 //Copy of constructor-supplied param
    HWND const m_hParent;        //Copy of constructor-supplied param
    HINSTANCE const m_hInstance; //Copy of constructor-supplied param


private: //See MyMsgMap, this is where these are mainly used...
    std::map<unsigned long, MsgHandler> m_mapMsgHandlers;
    std::map<unsigned long, MsgHandler> m_mapCmdHandlers;
    static BOOL CALLBACK MyMsgMap_stub(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


private:
    WORD const m_wTemplate; //Copy of constructor-supplied param
};

#endif // !defined(AFX_CBscBblDlg_H__E852D391_8F43_4DDB_89A3_65FCC24D01B0__INCLUDED_)
