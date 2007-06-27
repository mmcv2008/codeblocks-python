#ifndef SHELLCTRL_H
#define SHELLCTRL_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/process.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

//// in the header of the source file BEGIN_DECLARE_EVENT_TYPES()
//DECLARE_EVENT_TYPE(EVTSHELLTERM, value)
//END_DECLARE_EVENT_TYPES()
//// in the implementation
//DEFINE_EVENT_TYPE(EVTSHELLTERM)
//
//class ilShellTermEvent: public wxEvent
//{
//public:
//    wxString &newdata;
//    wxString &newerrdata;
//}


class ShellManager;

// This is a termnial control
// The control derives from a basic Text Control
// and owns a child process
// terminal controls sit inside a tabbed panel
class ShellTermCtrl : public wxTextCtrl
{
    public:
        ShellTermCtrl() {}
        ShellTermCtrl(wxWindow* parent,
                    wxWindowID id, ShellManager *shellmgr=NULL,
                    const wxString& name = wxEmptyString,
                    const wxString& value = wxEmptyString,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxTE_RICH|wxTE_MULTILINE|wxTE_READONLY|wxTE_PROCESS_ENTER|wxEXPAND);
        virtual ~ShellTermCtrl() {if (m_proc) {if (!m_dead) {m_proc->Detach();} } }
        long LaunchProcess(wxString processcmd, int stderrmode);
        void KillProcess();
        void KillWindow();
        long GetPid() {if(m_proc) return m_procid; else return -1;}
        void ReadStream(int maxchars=1000);
        void OnUserInput(wxKeyEvent& ke);
        wxString GetName() {return m_name;}
        void SetName(const wxString &name) {m_name=name;}
        bool IsDead() {return m_dead;}
// Overrideables
//        virtual void PostReadStream(const wxString &latestdata) {}
//        virtual void PostEndProcess() {}
    private:
        wxProcess *m_proc;
        long m_procid;
        wxOutputStream *m_ostream;
        wxInputStream *m_istream;
        wxInputStream *m_estream;
        void OnEndProcess(wxProcessEvent &event);
        bool m_dead;
        wxString m_lateststreamdata;
        wxString m_latesterrstreamdata;
        int m_killlevel;
        int m_exitcode;
        wxString m_name;
        ShellManager *m_shellmgr;
    DECLARE_DYNAMIC_CLASS(wxTextCtrl)
    DECLARE_EVENT_TABLE()
};

// abstract base class to manage collections of shell terms
//class ShellManager
//{
//    friend class ShellTermCtrl;
//    public:
//        virtual ~ShellManager();
//    private:
//        virtual void OnShellTerminate(ShellTermCtrl *term); //notifies manager of shell termination
//};


class ShellManager : public wxPanel
{
    friend class ShellTermCtrl;
    public:
        ShellManager(wxWindow* parent);
        ~ShellManager(); //virtual??
        long LaunchProcess(wxString processcmd, wxString name, int stderrmode);
        void KillProcess(int id);
        void KillWindow(int id);
        ShellTermCtrl *GetPage(size_t i);
        ShellTermCtrl *GetPage(const wxString &name);
        int NumAlive();
    private:
        //Responders to standard wxWidgets Messages
        void OnUserInput(wxKeyEvent& ke);
        void OnPollandSyncOutput(wxTimerEvent& te);
        void OnPageClosing(wxFlatNotebookEvent& event);
        bool QueryClose(ShellTermCtrl* sh);
        //Responders to friend class ShellTermCtrl
        void OnShellTerminate(ShellTermCtrl *term);
        size_t GetTermNum(ShellTermCtrl *term);
    protected:
        wxTimer m_synctimer;
        wxFlatNotebook *m_nb;
    DECLARE_EVENT_TABLE()
};



#endif // SHELLCTRL_H
