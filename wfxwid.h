// This is a part of the Widget Foundation Classes.
// Copyright (C) Grant Ward (grant.ward@gmail.com)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Widget Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Widget Foundation Classes product.
//
#pragma once
#include "wfx.h"

BEGIN_NAMESPACE_WFX

//////////////////////////////////////////////////////////////////////////
// forward declare
class WidDispatch;
class Timer;

class Widget;
	class Slider;
	class ScrollBar;
	class RoundWid;
		class ImageWid;
			class Button;
				class RadioButtonItem;
				class CheckBoxItem;
	class CheckBox;
	class RadioButton;
	class Label;
	class ProcessBar;

	class ToolTip;
	class InPlaceWid;

class Window;
	class InPlaceWnd;
		class EditWnd;
		class ComboWnd;


//////////////////////////////////////////////////////////////////////////
// WidgetBase Interface of widget
class WFX_API WidgetBase
{
public:
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID) = 0;
	LRESULT SendWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
};

typedef BOOL (*DrawFunction)(Widget* pWid, Gdiplus::Graphics& grph);

//////////////////////////////////////////////////////////////////////////
// Widget: the root class of ui classes
class WFX_API Widget :
	public WidgetBase
{
	friend class WidDispatch;
	friend class Timer;
public:
	Widget(void);
	virtual ~Widget(void);

	operator HWID() const;

public:
	WFX_BEGIN_MSG_MAP(Widget)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WM_UPDATEUISTATE, OnStateChanged)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_TIMER, OnTimer)
		WFX_MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		WFX_MESSAGE_HANDLER(WUM_QUERY_VIRTUAL_SIZE, OnQueryVirtualSize)
		WFX_MESSAGE_HANDLER(WUM_GET_VISUAL_SIZE, OnQueryVisualSize)
		WFX_MESSAGE_HANDLER(WUM_GET_VIRTUAL_SIZE, OnGetVirtualSize)
		WFX_MESSAGE_HANDLER(WUM_SB_OFFSET, OnScrollBarOffset)
		WFX_MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	WFX_END_MSG_MAP()

public:
	BOOL Create(const RECT& rc, WidDispatch* pDispatch,
		Widget* pParent = NULL, BOOL bNC = FALSE);

	// Position
public:
	RECT GetRect() const;
	void SetRect(const RECT& rc);
	RECT GetParentRect() const;
	void ShowWid(WORD wShow);
	BOOL IsShow() const;
	void SetCapture();
	void ReleaseCapture();

protected:
	void MyShowWid(WORD wShow);
	RECT GetDrawRect() const;
	void SetDrawRect(const RECT& rc);
public:
	// Generation
	Widget* GetParent() const;
	void SetParent(Widget* pParent);

	// For WidDispatch
protected:
	void SetMyParent(Widget* pParent);
	ULONG GetChildren(std::vector<Widget*>& rgpChildren) const;
	ULONG GetChildren() const;
	BOOL HasChild() const;

protected:
	ULONG AddChild(Widget* pWid);
	ULONG RemoveChild(Widget* pWid);
public:
	// Graphics
	void InvalidWid();
protected:
	virtual void OnDraw(HDC hdc, const RECT& rcPaint);

protected:
	void SetHwid(HWID hWid);
	HWID GetHwid() const;

	// Scrollbar
public:
	void EnableScrollBar(UINT nBar, BOOL bEnable = TRUE);
	UINT GetSBFlag() const;
	void SetSBFlag(UINT uSBFlag);
	void SetScrollInfo(int nBar, LPCSCROLLINFO lpsi, BOOL redraw);
	void GetScrollInfo(int nBar, LPSCROLLINFO lpsi);
	BOOL ScrollWid(int XAmount, int YAmount);
protected:
	void SetScrollBar(int nBar, ScrollBar* pScrollBar);
	ScrollBar* GetScrollBar(int nBar) const;

	// Text
public:
	void SetText(const std::wstring& strText);
	std::wstring GetText() const;
	virtual HFONT GetFontObject() const;
public:
	BOOL PostWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT SendParentMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	UINT_PTR SetWidTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillWidTimer(UINT_PTR uIDEvent);

public:
	void SetState(WORD wState);
	WORD GetState() const;

public:
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnStateChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnQueryVirtualSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnGetVirtualSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnQueryVisualSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnScrollBarOffset(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	LONG GetVOffset() const;
	void SetVOffset(LONG nOffset);
	LONG GetHOffset() const;
	void SetHOffset(LONG nOffset);
	SIZE GetVirtualSize() const;

public:
	void SetVirtualSizeValid(BOOL bValid = TRUE);
protected:
	virtual SIZE EstimateVirualSize();

	// Identifier
public:
	HWID m_hWid;
	WidDispatch* m_pDispatch;

protected:
	std::wstring m_strText;
	DWORD m_dwFormat;
	SharedPtr<LOGFONTW> m_pFont;
	COLORREF m_clrBkgnd;
	COLORREF m_clrFrame;
	COLORREF m_clrText;

private:
	// Position
	RECT m_rcDraw;
	RECT m_rcWid;
	BOOL m_bNC;
	WORD m_wShow;

	// Generation
	Widget* m_pParent;
	std::vector<Widget*> m_rgpChildren;

	// Scrollbar
	ScrollBar* m_pHScrollbar;
	ScrollBar* m_pVScrollbar;
	UINT m_uBarFlag;
	LONG m_nHorzPosOffset;
	LONG m_nVertPosOffset;
	// Timers
	std::vector<UINT_PTR> m_rgTimer;

	// Size
	SIZE m_szVirtual;
	BOOL m_bVirtualSizeValid;

	// State
	WORD m_wState;
};

//////////////////////////////////////////////////////////////////////////
// Slider: for ScrollBar
class WFX_API Slider : public Widget
{
public:
	Slider(int nBar);
	virtual ~Slider();
public:
	WFX_BEGIN_MSG_MAP(Slider)
		WFX_MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	virtual void OnDraw(HDC hdc, const RECT& rcPaint);
protected:
	BOOL m_bLButtonDown;
	BOOL m_bInSlider;
	POINT m_ptLButtonDown;
	int m_nBar;
};

//////////////////////////////////////////////////////////////////////////
// ScrollBar: Common ScrollBar
class WFX_API ScrollBar : public Widget
{
public:
	ScrollBar(int nBar);
	virtual ~ScrollBar();
public:
	WFX_BEGIN_MSG_MAP(ScrollBar)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WUM_SB_OFFSET, OnSliderOffset)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	int GetBar() const;
	void SetBar(int nBar);
	void GetScrollInfo(SCROLLINFO* pScrollInfo) const;
	void SetScrollInfo(const SCROLLINFO* pScrollInfo);
	void SetRange(int nMax, int nMin);
	void SetPos(int nPos);
	int GetPos() const;
	//int GetMax() const;
	//int GetMin() const;

protected:
	int CalcSliderPos(const RECT& rcSlider);
	RECT CalcSliderRect(const RECT& rcMaxSlider);
	int GetSliderMax();
	int GetSliderMin();
	int GetSlierMid();
	float CalcSliderSize();

public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSliderOffset(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);

protected:
	int m_nBar;
	BOOL m_bLButtonDown;
	BOOL m_bInSlider;
	POINT m_ptLButtonDown;
	LONG m_nSliderSize;
	LONG m_nArrorSize;
	LONG m_nSAMargin;
protected:
	SharedPtr<SCROLLINFO> m_pScrollInfo;
	SharedPtr<Widget> m_pArrow1;
	SharedPtr<Widget> m_pArrow2;
	SharedPtr<Widget> m_pSlider;
};

class WFX_API RoundWid : public Widget
{
public:
	RoundWid();
	virtual ~RoundWid();

public:
	WFX_BEGIN_MSG_MAP(RoundWid)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	virtual void OnDraw(HDC hdc, const RECT& rcPaint);
protected:
	SharedPtr<Gdiplus::GraphicsPath> m_pGrphPath;
};

class WFX_API ImageWid : public Widget
{
public:
	ImageWid();
	ImageWid(const std::wstring& strStatic,
		const std::wstring& strMouse, 
		const std::wstring& strPush,
		const std::wstring& strChecked);
	virtual ~ImageWid();
public:
	void SetImage(WORD wState, const std::wstring& strImage);
	void SetImage(const std::wstring& strStatic,
		const std::wstring& strMouse, 
		const std::wstring& strPush,
		const std::wstring& strChecked);
protected:
	SharedPtr<Gdiplus::Image> GetImageFromState();
protected:
	SharedPtr<Gdiplus::Image> m_pStatic;
	SharedPtr<Gdiplus::Image> m_pMouse;
	SharedPtr<Gdiplus::Image> m_pPush;
	SharedPtr<Gdiplus::Image> m_pChecked;
};

class WFX_API SplitterBar : public Widget
{

};

class WFX_API Button : public ImageWid
{
public:
	Button(BOOL bCheckState = FALSE);
	virtual ~Button();
public:
	virtual void OnDraw(HDC hdc, const RECT& rcPaint);

	WFX_BEGIN_MSG_MAP(Button)
		WFX_MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WM_UPDATEUISTATE, OnStateChanged)
		WFX_CHAIN_MSG_MAP(ImageWid)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnStateChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	void Check(BOOL bCheck = TRUE);
	BOOL IsCheck() const;
protected:
	BOOL m_bLButtonDown;
	BOOL m_bChecked;
	BOOL m_bCheckState;
};

class WFX_API CheckBoxItem : public Button
{
public:
	CheckBoxItem();
	CheckBoxItem(const std::wstring& strChecked,
		const std::wstring& strUnCheck);
	virtual ~CheckBoxItem();
public:
	virtual void OnDraw(HDC hdc, const RECT& rcPaint);

protected:
	SharedPtr<Gdiplus::Image> GetImage() const;

protected:
	SharedPtr<Gdiplus::Image> m_pImageChecked;
	SharedPtr<Gdiplus::Image> m_pImageUnCheck;
};

class WFX_API ToolTip : public Button
{

};

class WFX_API CheckBox : public Widget
{
public:
	CheckBox();
public:
	WFX_BEGIN_MSG_MAP(CheckBox)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual void OnDraw(HDC hdc, const RECT& rcPaint);
protected:
	ULONG m_lOffset;
	SharedPtr<CheckBoxItem> m_pItem;
};

class WFX_API RadioButtonItem : public CheckBoxItem
{
public:
	virtual void OnDraw(HDC hdc, const RECT& rc);
public:
	static BOOL DrawRadioButtonItem(Widget* pWid, Gdiplus::Graphics& grph);
};

class WFX_API RadioButton : public CheckBox
{
public:
	RadioButton();
};

class WFX_API Label : public Widget
{

};

class WFX_API ProcessBar : public Widget
{

};

class WFX_API InPlaceWid : public Widget
{
public:
	InPlaceWid();
	virtual ~InPlaceWid();
public:
	WFX_BEGIN_MSG_MAP(InPlaceWid)
		WFX_MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	virtual BOOL Initial() = 0;
protected:
	InPlaceWnd* m_pWindow;
};

class WFX_API TextBox : public InPlaceWid
{
public:
	TextBox(WORD wMode = WID_TBM_READWRITE);
public:
	void SetMode(WORD wMode, BOOL bAdd = FALSE);
	WORD GetMode() const;
	BOOL IsReadonly() const;
	BOOL IsPassword() const;
protected:
	virtual BOOL Initial();
protected:
	WORD m_wMode;
	BOOL m_bEditting;
};

class WFX_API ComboBox : public InPlaceWid
{
public:
	ULONG GetSize() const;
	ULONG GetItemHeight(ULONG nIndex) const;
	ULONG GetSel() const;
	Widget* GetWid(ULONG nIndex) const;
protected:
	virtual BOOL Initial();
};

////////////////////////////////////////////////////////////////////////
// Window: Window for controls
class WFX_API Window : public WidgetBase
{
public:
	// !!! m_pDispatch must be the first member
	SharedPtr<WidDispatch> m_pDispatch;
public:
	Window();
	virtual ~Window();
public:
	HWND GetHwnd() const;
	operator HWND() const;

	BOOL RegisterWindowClass();
	BOOL RegisterSuperClass();

	HWND Create(HWND hwndParent, LPCTSTR pszName,
		DWORD dwStyle, DWORD dwExStyle, const RECT& rc,
		HMENU hMenu = NULL);

	HWND SubClass(HWND hWnd);
	void UnSubClass();
	void ShoWidWnd(BOOL bShow = TRUE, BOOL bTakeFocus = TRUE);
	BOOL ShowModal();
	void Close();
	void CenterWindow();
	void SetIcon(UINT nRes);
	std::wstring GetText() const;
	void SetText(const std::wstring& strText);
	void SetFont(HFONT hFont) const;
	void GetClientRect(RECT& rc);
	UINT_PTR SetTimer(UINT_PTR nIDEvent,
		UINT uElapse,
		TIMERPROC lpTimerFunc) const;
protected:
	virtual LPCTSTR GetWindowClassName() const = 0;
	virtual LPCTSTR GetSuperClassName() const;
	virtual UINT GetClassStyle() const;

	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
	void ResizeClient(int cx = -1, int cy = -1);
	virtual void OnFinalMessage(HWND hWnd);

	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd;
	WNDPROC m_OldWndProc;
	BOOL m_bSubclassed;
};

class WFX_API InPlaceWnd : public Window
{
public:
	InPlaceWnd();
	virtual ~InPlaceWnd();
	virtual void OnFinalMessage(HWND hWnd);
public:
	WFX_BEGIN_MSG_MAP(InPlaceWnd)
		WFX_MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		WFX_CHAIN_MSG_MAP(Window)
	WFX_END_MSG_MAP()
wfx_msg LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
	BOOL& bHandled);
public:
	BOOL Initial(InPlaceWid* pOwner);
	virtual HWND CreateInPlaceWindow() = 0;
	virtual void OnInPlaceWindowKillFocus();
protected:
	InPlaceWid* m_pOwner;
};

class WFX_API TextBoxWnd : public InPlaceWnd
{
public:
	WFX_BEGIN_MSG_MAP(TextBoxWnd)
		WFX_MESSAGE_HANDLER(OCM_COMMAND, OnEditChanged)
		WFX_CHAIN_MSG_MAP(InPlaceWnd)
	WFX_END_MSG_MAP()
public:
	virtual void OnInPlaceWindowKillFocus();
	virtual HWND CreateInPlaceWindow();
	virtual LPCTSTR GetSuperClassName() const;
	virtual LPCTSTR GetWindowClassName() const;
public:
	wfx_msg LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

class WFX_API ComboWnd : public InPlaceWnd
{
public:
	ComboWnd();
public:
	WFX_BEGIN_MSG_MAP(ComboWnd)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(InPlaceWnd)
	WFX_END_MSG_MAP()
public:
	virtual void OnInPlaceWindowKillFocus();
	virtual HWND CreateInPlaceWindow();
	virtual LPCTSTR GetWindowClassName() const;
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	SharedPtr<Widget> m_pRoot;
	std::vector<Widget*> m_rgpItems;
};

class WFX_API ToolTipWnd : public Window
{

};

//////////////////////////////////////////////////////////////////////////
// Timer: helper class for WidDispatch
class Timer
{
	friend class WidDispatch;
public:
	Timer(WidDispatch* pDispatch);
	~Timer();
public:
	UINT_PTR SetWidTimer(Widget* pWid, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillWidTimer(Widget* pWid, UINT_PTR uIDEvent);
	Widget* GetWidgetFromTimer(UINT_PTR uIDEvent);
	void Destroy(Widget* pWid);
protected:
	WidDispatch* m_pDispatch;
	std::map<UINT_PTR, 
		std::pair<HWID, Widget*> > m_Timer;
};

//////////////////////////////////////////////////////////////////////////
// WidDispatch: dispatch messages for widget
class WFX_API WidDispatch
{
	friend class Widget;
	friend class Timer;
public:
	WidDispatch(HWND hWnd = NULL);
	~WidDispatch();
public:
	void SetHwnd(HWND hWnd);
	HWND GetHwnd() const;
protected:
	BOOL Create(Widget* pThis);
	BOOL SetParent(Widget* pThis, Widget* pParent);
	BOOL Destroy(HWID& hWid);
	HWID GenerateHwid();
	void RecycleHwid(HWID& hWid); 
	void ClearH2O(std::pair<HWID, Widget*>& h2o);
	void SetMouseMoveH2O(const std::pair<HWID, Widget*>& h2o);
	void SetCapturedH2O(const std::pair<HWID, Widget*>& h2o);
	Widget* GetObject(const std::pair<HWID, Widget*>& h2o);
protected:
	void DrawWid(Widget* pWid, const RECT& rcPaint);
	void DrawGen(Widget* pWid, HDC hdc, const RECT& rcPaint);
	void DrawBkgnd(Widget* pWid, HDC hdc, const RECT& rcPaint);
	void OnPaint(const RECT& rcPaint);
	void Invalidate(const RECT& rc);
protected:
	void SetWidRect(Widget* pWid, const RECT& rc);
	void ShowWid(Widget* pWid, WORD wShow);
public:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	Widget* GetWidPt(POINT pt);
	Widget* GetWidPt(const std::vector<Widget*>& rgpWid);
	Widget* FromHwid(HWID hWid) const;
	void SetCapture(Widget* pWid);
	void SetFocus(Widget* pWid);
	void ReleaseCapture();
public:
	void EnableScrollBar(Widget* pWid, UINT uBarFlag, BOOL bEnable = TRUE);
	void SetScrollInfo(Widget* pWid, int nBar, LPCSCROLLINFO lpsi, BOOL redraw);
	void GetScrollInfo(Widget* pWid, int nBar, LPSCROLLINFO lpsi);
	void PreProcessMsg(Widget* pWid, UINT uMsg, WPARAM wParam, LPARAM lParam);
	UINT_PTR SetWidTimer(Widget* pWid, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillWidTimer(Widget* pWid, UINT_PTR uIDEvent);
	Widget* GetWidgetFromTimer(UINT_PTR uIDEvent);
public:
	static HINSTANCE GetInstance();
	static void SetInstance(HINSTANCE hInstance);
	static RECT FromRect(const RECT& rc);
protected:
	HWND m_hWnd;
	std::vector<HWID> m_rghWid;
	std::map<HWID, Widget*> m_Handle2Object;
	std::map<HWID, Widget*> m_h2oOrphan;
	std::pair<HWID, Widget*> m_h2oLastMouseMove;
	std::pair<HWID, Widget*> m_h2oCaptured;
	std::pair<HWID, Widget*> m_h2oFocused;
	SharedPtr<Timer> m_pTimer;
public:
	static HWID s_hWidBase;
private:
	static HINSTANCE s_hInstance;
};

class GdiObject
{
public:
	static GdiObject& Instance();
	~GdiObject();
public:
	HFONT GetEditFont();
private:
	HFONT m_hEditFont;
private:
	GdiObject();
};
class WFX_API LayoutBase
{
public:
	virtual SharedPtr<WidDispatch> Parse(const std::wstring& strXml);
};

END_NAMESPACE_WFX
