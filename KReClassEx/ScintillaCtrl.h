#pragma once

namespace Scintilla {
	// 指示符
	enum class IndicatorStyle {
		Plain = 0,
		Squiggle = 1,
		TT = 2,
		Diagonal = 3,
		Strike = 4,
		Hidden = 5,
		Box = 6,
		RoundBox = 7,
		StraightBox = 8,
		Dash = 9,
		Dots = 10,
		SquiggleLow = 11,
		DotBox = 12,
		SquigglePixmap = 13,
		CompositionThick = 14,
		CompositionThin = 15,
		FullBox = 16,
		TextFore = 17,
		Point = 18,
		PointCharacter = 19,
		Gradient = 20,
		GradientCentre = 21,
		ExplorerLink = 22,
	};

	enum class MarkerSymbol {
		Circle = 0,
		RoundRect = 1,
		Arrow = 2,
		SmallRect = 3,
		ShortArrow = 4,
		Empty = 5,
		ArrowDown = 6,
		Minus = 7,
		Plus = 8,
		VLine = 9,
		LCorner = 10,
		TCorner = 11,
		BoxPlus = 12,
		BoxPlusConnected = 13,
		BoxMinus = 14,
		BoxMinusConnected = 15,
		LCornerCurve = 16,
		TCornerCurve = 17,
		CirclePlus = 18,
		CirclePlusConnected = 19,
		CircleMinus = 20,
		CircleMinusConnected = 21,
		Background = 22,
		DotDotDot = 23,
		Arrows = 24,
		Pixmap = 25,
		FullRect = 26,
		LeftRect = 27,
		Available = 28,
		Underline = 29,
		RgbaImage = 30,
		Bookmark = 31,
		VerticalBookmark = 32,
		Character = 10000,
	};

	enum class MarkerOutline {
		FolderEnd = 25,
		FolderOpenMid = 26,
		FolderMidTail = 27,
		FolderTail = 28,
		FolderSub = 29,
		Folder = 30,
		FolderOpen = 31,
	};

	enum class MarginType {
		Symbol = 0,
		Number = 1,
		Back = 2,
		Fore = 3,
		Text = 4,
		RText = 5,
		Colour = 6,
	};

	enum class StylesCommon {
		Default = 32,
		LineNumber = 33,
		BraceLight = 34,
		BraceBad = 35,
		ControlChar = 36,
		IndentGuide = 37,
		CallTip = 38,
		FoldDisplayText = 39,
		LastPredefined = 39,
		Max = 255,
	};

	enum class Element {
		List = 0,
		ListBack = 1,
		ListSelected = 2,
		ListSelectedBack = 3,
		SelectionText = 10,
		SelectionBack = 11,
		SelectionAdditionalText = 12,
		SelectionAdditionalBack = 13,
		SelectionSecondaryText = 14,
		SelectionSecondaryBack = 15,
		SelectionInactiveText = 16,
		SelectionInactiveBack = 17,
		Caret = 40,
		CaretAdditional = 41,
		CaretLineBack = 50,
		WhiteSpace = 60,
		WhiteSpaceBack = 61,
		HotSpotActive = 70,
		HotSpotActiveBack = 71,
		FoldLine = 80,
		HiddenLine = 81,
	};

	enum class IndicValue {
		Bit = 0x1000000,
		Mask = 0xFFFFFF,
	};

	enum class IndicFlag {
		None = 0,
		ValueFore = 1,
	};

	enum class AutoCompleteOption {
		Normal = 0,
		FixedSize = 1,
	};

	enum class IndentView {
		None = 0,
		Real = 1,
		LookForward = 2,
		LookBoth = 3,
	};

	enum class PrintOption {
		Normal = 0,
		InvertLight = 1,
		BlackOnWhite = 2,
		ColourOnWhite = 3,
		ColourOnWhiteDefaultBG = 4,
		ScreenColours = 5,
	};

	enum class Alpha {
		Transparent = 0,
		Opaque = 255,
		NoAlpha = 256,
	};

	enum class WhiteSpace {
		Invisible = 0,
		VisibleAlways = 1,
		VisibleAfterIndent = 2,
		VisibleOnlyInIndent = 3,
	};

	enum class TabDrawMode {
		LongArrow = 0,
		StrikeOut = 1,
	};

	enum class EndOfLine {
		CrLf = 0,
		Cr = 1,
		Lf = 2,
	};

	enum class IMEInteraction {
		Windowed = 0,
		Inline = 1,
	};

	enum class Layer {
		Base = 0,
		UnderText = 1,
		OverText = 2,
	};

	enum class IdleStyling {
		None = 0,
		ToVisible = 1,
		AfterVisible = 2,
		All = 3,
	};

	enum class Wrap {
		None = 0,
		Word = 1,
		Char = 2,
		WhiteSpace = 3,
	};

	enum class WrapVisualFlag {
		None = 0x0000,
		End = 0x0001,
		Start = 0x0002,
		Margin = 0x0004,
	};

	enum class WrapVisualLocation {
		Default = 0x0000,
		EndByText = 0x0001,
		StartByText = 0x0002,
	};

	enum class WrapIndentMode {
		Fixed = 0,
		Same = 1,
		Indent = 2,
		DeepIndent = 3,
	};

	enum class LineCache {
		None = 0,
		Caret = 1,
		Page = 2,
		Document = 3,
	};

	enum class PhasesDraw {
		One = 0,
		Two = 1,
		Multiple = 2,
	};

	enum class FontQuality {
		Mask = 0xF,
		Default = 0,
		NonAntialiased = 1,
		Antialiased = 2,
		LcdOptimized = 3,
	};

	enum class MultiPaste {
		Once = 0,
		Each = 1,
	};

	enum class Accessibility {
		Disabled = 0,
		Enabled = 1,
	};

	enum class FindOption {
		None = 0x0,
		WholeWord = 0x2,
		MatchCase = 0x4,
		WordStart = 0x00100000,
		RegExp = 0x00200000,
		Posix = 0x00400000,
		Cxx11RegEx = 0x00800000,
	};
	DEFINE_ENUM_FLAG_OPERATORS(FindOption);

	enum class FoldLevel {
		None = 0x0,
		Base = 0x400,
		WhiteFlag = 0x1000,
		HeaderFlag = 0x2000,
		NumberMask = 0x0FFF,
	};

	enum class FoldDisplayTextStyle {
		Hidden = 0,
		Standard = 1,
		Boxed = 2,
	};

	enum class FoldAction {
		Contract = 0,
		Expand = 1,
		Toggle = 2,
	};

	enum class AutomaticFold {
		None = 0x0000,
		Show = 0x0001,
		Click = 0x0002,
		Change = 0x0004,
	};

	enum class FoldFlag {
		None = 0x0000,
		LineBeforeExpanded = 0x0002,
		LineBeforeContracted = 0x0004,
		LineAfterExpanded = 0x0008,
		LineAfterContracted = 0x0010,
		LevelNumbers = 0x0040,
		LineState = 0x0080,
	};

	enum class CharacterSet {
		Ansi = 0,
		Default = 1,
		Baltic = 186,
		ChineseBig5 = 136,
		EastEurope = 238,
		GB2312 = 134,
		Greek = 161,
		Hangul = 129,
		Mac = 77,
		Oem = 255,
		Russian = 204,
		Oem866 = 866,
		Cyrillic = 1251,
		ShiftJis = 128,
		Symbol = 2,
		Turkish = 162,
		Johab = 130,
		Hebrew = 177,
		Arabic = 178,
		Vietnamese = 163,
		Thai = 222,
		Iso8859_15 = 1000,
	};

	enum class CaseVisible {
		Mixed = 0,
		Upper = 1,
		Lower = 2,
		Camel = 3,
	};

	enum class FontWeight {
		Normal = 400,
		SemiBold = 600,
		Bold = 700,
	};
}

class CScintillaCtrlT :public CWindow {
	using Position = Sci_Position;
	using Line = intptr_t;
	using Colour = int;
	using ColourAlpha = int;

public:
	HWND Create(HWND hWndParent, _U_RECT rect = nullptr, LPCTSTR szWindowName = nullptr,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, DWORD dwExStyle = 0,
		_U_MENUorID MenuOrID = 0U, PVOID lpCreateParam = nullptr) {
		auto hWnd = CWindow::Create(GetWndClassName(), hWndParent, rect.m_lpRect,
			szWindowName, dwStyle, dwExStyle, MenuOrID.m_hMenu, lpCreateParam);
		if (!hWnd)
			return nullptr;

		// 取得直接控制函数
		m_Sci = (decltype(m_Sci))::SendMessage(hWnd, SCI_GETDIRECTFUNCTION, 0, 0);
		ATLASSERT(m_Sci);
		// 取得直接控制指针
		m_SciWndData = (sptr_t)::SendMessage(hWnd, SCI_GETDIRECTPOINTER, 0, 0);
		ATLASSERT(m_SciWndData);
		return hWnd;
	}

	static LPCTSTR GetWndClassName() {
		return L"Scintilla";
	}

	LRESULT Execute(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0) const {
		return m_Sci(m_SciWndData, Msg, wParam, lParam);
	}

	LRESULT ExecuteMsg(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0) const {
		ATLASSERT(::IsWindow(this->m_hWnd));
		return ::SendMessage(this->m_hWnd, Msg, wParam, lParam);
	}

	bool IsSelectionEmpty() const {
		// 如果每个选定范围都为空，则返回1，否则返回0。
		return (bool)Execute(SCI_GETSELECTIONEMPTY);
	}

	int SelectionCount() const {
		// 返回当前活动的选择数
		return (int)Execute(SCI_GETSELECTIONS);
	}

	void AddText(Position len, const char* text) {
		// 在光标所在位置插入一定长度的字符串，插入长度为len
		Execute(SCI_ADDTEXT, len, reinterpret_cast<LPARAM>(text));
	}

	void AppendText(Position len, const char* text) {
		// 在文档结尾位置插入一定长度的字符串
		Execute(SCI_APPENDTEXT, len, reinterpret_cast<LPARAM>(text));
	}

	void MarkerDefine(int index, Scintilla::MarkerSymbol symbol) {
		// 改变标记的样式
		Execute(SCI_MARKERDEFINE, index, static_cast<LPARAM>(symbol));
	}

	void ClearAll() {
		// 清空全部文本内容
		Execute(SCI_CLEARALL);
	}

	Position Length() const {
		// 获取文档字节总长
		return (Position)Execute(SCI_GETLENGTH);
	}

	int CharAt(Position pos) const {
		// 获取所选位置的字节
		return (int)Execute(SCI_GETCHARAT, pos);
	}

	Position CurrentPos() const {
		// 返回当前位置
		return (Position)Execute(SCI_GETCURRENTPOS);
	}

	Position Anchor() const {
		// 返回当前锚点位置
		return (Position)Execute(SCI_GETANCHOR);
	}

	int StyleAt(Position pos) const {
		// 得到所在位置的风格
		return (int)Execute(SCI_GETSTYLEAT, pos);
	}

	void Redo() {
		// 重做
		Execute(SCI_REDO);
	}
	void SetUndoCollection(bool collectUndo) {
		// 用于控制是否收集撤销消息
		Execute(SCI_SETUNDOCOLLECTION, collectUndo);
	}
	void SelectAll() {
		// 选择文档中的所有文本
		Execute(SCI_SELECTALL);
	}
	void SetSavePoint() {
		// 设置保存点
		Execute(SCI_SETSAVEPOINT);
	}

	void GotoLine(Line line) {
		// 转到所在行
		Execute(SCI_GOTOLINE, line);
	}

	void GotoPos(Position caret) {
		// 转到所在位置
		Execute(SCI_GOTOPOS, caret);
	}

	void UsePopup(int mode) {
		// 设置默认编辑菜单的模式
		Execute(SCI_USEPOPUP, mode);
	}

	Position GetLineLength(Line line) const {
		// 返回行的长度，包含任何行尾字符
		return Execute(SCI_LINELENGTH, line);
	}

	Line LineFromPosition(Position pos) const {
		// 通过文档所在位置获得所在行
		return Execute(SCI_LINEFROMPOSITION, pos);
	}

	Position GetCurrentPos() const {
		// 返回当前位置
		return Execute(SCI_GETCURRENTPOS);
	}

	std::string GetCurLine() const {
		std::string text;
		auto len = GetLineLength(LineFromPosition(GetCurrentPos()));
		if (len == 0)
			return "";
		text.resize(len);
		// 获得当前行的文本
		Execute(SCI_GETCURLINE, len, reinterpret_cast<LPARAM>(text.data()));
		return text;
	}

	Position GetEndStyled() const {
		// 获取最后样式化字符的位置
		return Execute(SCI_GETENDSTYLED);
	}

	void StartStyling(Position start) {
		// 设置样式的开始位置，为样式化做准备
		Execute(SCI_STARTSTYLING, start);
	}

	void SetTabWidth(int tabWidth) {
		// 设置TAB的宽度，默认是8
		Execute(SCI_SETTABWIDTH, tabWidth);
	}

	void SetCodePage(int codePage) {
		// 设置代码页，默认为UTF-8
		Execute(SCI_SETCODEPAGE, codePage);
	}

	void Focus(bool focus = true) {
		// 设置是否获取焦点
		Execute(SCI_SETFOCUS, focus);
	}

	void MarkerSetFore(int markerNumber, Colour fore) {
		// 指定标记前景色
		Execute(SCI_MARKERSETFORE, markerNumber, fore);
	}

	void MarkerSetBack(int markerNumber, Colour back) {
		// 指定标记背景色
		Execute(SCI_MARKERSETBACK, markerNumber, back);
	}

	int MarkerAdd(Line line, int markerNumber) {
		// 添加标记
		return Execute(SCI_MARKERADD, line, markerNumber);
	}

	void MarkerDelete(Line line, int markerNumber) {
		// 删除标记
		Execute(SCI_MARKERDELETE, line, markerNumber);
	}

	void MarkerDeleteAll() {
		// 删除所有标记
		Execute(SCI_MARKERDELETEALL);
	}

	int MarkerGet(Line line) const {
		// 返回该行上的标记
		return Execute(SCI_MARKERGET, line);
	}

	void SetMarginWidth(int margin, int pixelWidth) {
		// 设置页边距
		Execute(SCI_SETMARGINWIDTHN, margin, pixelWidth);
	}

	int GetMarginWidthN(int margin) const {
		// 获取页边距
		return Execute(SCI_GETMARGINWIDTHN, margin);
	}

	void SetMarginSensitive(int margin, bool sensitive) {
		// 对鼠标点击是否敏感，默认情况下，所有边距都是不敏感的。
		Execute(SCI_SETMARGINSENSITIVEN, margin, sensitive);
	}

	void SetMarginBack(int margin, Colour back) {
		// 设置页边的背景色
		Execute(SCI_SETMARGINBACKN, margin, back);
	}

	void StyleClearAll() {
		// 将所有所有元素重置成与STYLE_DEFAULT一致
		Execute(SCI_STYLECLEARALL);
	}

	void SetMarginType(int index, Scintilla::MarginType type) {
		// 设置页边类型
		Execute(SCI_SETMARGINTYPEN, index, (LPARAM)type);
	}

	void StyleSetFore(int style, Colour fore) {
		// 设置各种语法元素前景色
		Execute(SCI_STYLESETFORE, style, fore);
	}

	void StyleSetBack(int style, Colour back) {
		// 设置各种语法元素背景色
		Execute(SCI_STYLESETBACK, style, back);
	}

	void StyleSetBold(int style, bool bold) {
		// 设置粗体
		Execute(SCI_STYLESETBOLD, style, bold);
	}

	void StyleSetItalic(int style, bool italic) {
		// 设置斜体
		Execute(SCI_STYLESETITALIC, style, italic);
	}

	void StyleSetSize(int style, int sizePoints) {
		// 设置字号
		Execute(SCI_STYLESETSIZE, style, sizePoints);
	}

	void StyleSetFont(int style, const char* fontName) {
		// 设置字体
		Execute(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(fontName));
	}

	SciFnDirect m_Sci;
	sptr_t m_SciWndData;
};