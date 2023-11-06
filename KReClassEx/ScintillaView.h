#pragma once

#include "ScintillaCtrl.h"

enum class LexLanguage {
	Xml,
	Asm,
	CPP,
};

class CScintillaView :
	public CWindowImpl<CScintillaView> {
public:
	DECLARE_WND_CLASS(NULL);

	CScintillaView(IMainFrame* frame, PCWSTR title);

	BEGIN_MSG_MAP(CScintillaView)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE,OnSize)
	END_MSG_MAP()

	CString GetTitle() const;
	CScintillaCtrl& GetCtrl();

	void SetText(PCWSTR text);
	void SetText(PCSTR text);
	void SetLanguage(LexLanguage language);

	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void UpdateColors();
private:
	CString m_Title;
	CScintillaCtrl m_Sci;
	LexLanguage m_Language;
	IMainFrame* m_pFrame;
};