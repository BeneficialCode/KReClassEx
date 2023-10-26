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
	CScintillaView(IMainFrame* frame, PCWSTR title);

	BEGIN_MSG_MAP(CScintillaView)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()

	CString GetTitle() const;
	CScintillaCtrl& GetCtrl();

	void SetText(PCWSTR text);
	void SetText(PCSTR text);
	void SetLanguage(LexLanguage language);

	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CString m_Title;
	CScintillaCtrl m_Sci;
	LexLanguage m_Language;
	IMainFrame* m_pFrame;
};