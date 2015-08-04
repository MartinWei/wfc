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

///////////////////////////*** a gorgeous partition line ***/////////////////////////////

// forward declare
class WidDispatch;
class Timer;

class Widget;
	class Slider;
	class ScrollBar;
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
		class Menu;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// MsgMap Interface of widget
class WFX_API MsgMap
{
public:
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID) = 0;
	LRESULT SendWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
};

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef BOOL (*DrawFunction)(Widget* pWid, Gdiplus::Graphics& grph);
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API AttrBase
{
public:
	AttrBase(Widget* pParent = NULL);
public:
	virtual void SetText(const String& strText);
	virtual void SetFormat(DWORD dwFormat);
	virtual void SetTextColor(COLORREF clrText);
	virtual void SetBkgnd(COLORREF clrBkgnd);
	virtual void SetFrame(COLORREF clrFrame);
	virtual void SetImage(const String& strImage);
	virtual void SetState(WORD wState);
	virtual void SetRect(const Rect& rect);
	virtual String GetText() const;
	virtual DWORD GetFormat() const;
	virtual COLORREF GetTextColor() const;
	virtual COLORREF GetBkgnd() const;
	virtual COLORREF GetFrame() const;
	virtual PImage GetImage() const;
	virtual WORD GetState() const;
	virtual Rect GetRect() const;
	virtual Widget* GetParent() const;
	virtual void SetParent(Widget* pParent);
protected:
	String m_strText;
	DWORD m_dwFormat;
	COLORREF m_clrText;
	COLORREF m_clrBkgnd;
	COLORREF m_clrFrame;
	WORD m_wState;
	PImage m_pImg;
	Rect m_rect;
	Widget* m_pParent;
};
typedef SharedPtr<AttrBase> PBasicAttr;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// Widget: the root class of ui classes
class WFX_API Widget :
	public MsgMap, public AttrBase
{
	friend class WidDispatch;
	friend class Timer;
public:
	typedef std::vector<Widget*> WidList;
	typedef std::vector<Widget*>::iterator WidIter;
	typedef std::vector<Widget*>::const_iterator CWidIter;
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
	BOOL Create(const Rect& rc, WidDispatch* pDispatch,
		Widget* pParent = NULL, BOOL bNC = FALSE);

	// Position
public:
	virtual void SetRect(const Rect& rc);
	virtual void SetState(WORD wState);
public:
	Rect GetParentRect() const;
	void ShowWid(WORD wShow);
	BOOL IsShow() const;
	void SetCapture();
	void ReleaseCapture();

protected:
	void MyShowWid(WORD wShow);
	Rect GetDrawRect() const;
	void SetDrawRect(const Rect& rc);
public:
	virtual void SetParent(Widget* pParent);
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
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);

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
public:
	BOOL PostWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT SendParentMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	UINT_PTR SetWidTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillWidTimer(UINT_PTR uIDEvent);
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
	Size GetVirtualSize() const;
	
public:
	UINT GetID() const;
	void SetID(UINT nID);
public:
	void SetVirtualSizeValid(BOOL bValid = TRUE);
protected:
	virtual Size EstimateVirualSize();
	// Identifier
public:
	HWID m_hWid;
	WidDispatch* m_pDispatch;
private:
	// Position
	Rect m_rcDraw;
	BOOL m_bNC;
	WORD m_wShow;
	// Generation
	std::vector<Widget*> m_rgpChildren;
	// Scrollbar
	ScrollBar* m_pHScrollbar;
	ScrollBar* m_pVScrollbar;

	UINT m_uBarFlag;
	LONG m_nHorzPosOffset;
	LONG m_nVertPosOffset;
	// Timers need to be killed.
	std::vector<UINT_PTR> m_rgTimer;
	// Size
	Size m_szVirtual;
	BOOL m_bVirtualSizeValid;
	// Event ID
	UINT m_nID;
};
typedef SharedPtr<Widget> PWidget;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API UnitBase : public MsgMap, public AttrBase
{
public:
	UnitBase(Widget* pWid = NULL);
public:
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
	LRESULT SendParentMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
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
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	BOOL m_bLButtonDown;
	BOOL m_bInSlider;
	Point m_ptLButtonDown;
	int m_nBar;
};
typedef SharedPtr<Slider> PSlider;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef SharedPtr<SCROLLINFO> PSCROLLINFO;
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
	int CalcSliderPos(const Rect& rcSlider);
	Rect CalcSliderRect(const Rect& rcMaxSlider);
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
	Point m_ptLButtonDown;
	LONG m_nSliderSize;
	LONG m_nArrorSize;
	LONG m_nSAMargin;
protected:
	PSCROLLINFO m_pScrollInfo;
	PWidget m_pArrow1;
	PWidget m_pArrow2;
	PWidget m_pSlider;
};
typedef SharedPtr<ScrollBar> PScrollBar;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ImageWid : public Widget
{
public:
	ImageWid();
	ImageWid(const String& strStatic,
		const String& strMouse, 
		const String& strPush,
		const String& strChecked);
public:
	void SetImage(WORD wState, const String& strImage);
	void SetImage(const String& strStatic,
		const String& strMouse, 
		const String& strPush,
		const String& strChecked);
protected:
	PImage GetImageFromState();
protected:
	PImage m_pStatic;
	PImage m_pMouse;
	PImage m_pPush;
	PImage m_pChecked;
};
typedef SharedPtr<ImageWid> PImageWid;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API SplitterBar : public Widget
{

};
typedef SharedPtr<SplitterBar> PSplitterBar;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Button : public ImageWid
{
public:
	Button(BOOL m_bCheckable = FALSE);
public:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);

	WFX_BEGIN_MSG_MAP(Button)
		WFX_MESSAGE_HANDLER(WUM_LBUTTONCLICK, OnLButtonClik)
		WFX_CHAIN_MSG_MAP(ImageWid)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnLButtonClik(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnStateChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	void Check(BOOL bCheck = TRUE);
	BOOL IsChecked() const;
protected:
	BOOL m_bLButtonDown;
	BOOL m_bChecked;
	BOOL m_bCheckable;
};
typedef SharedPtr<Button> PButton;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API CheckBoxItem : public Button
{
public:
	CheckBoxItem();
	CheckBoxItem(const String& strChecked,
		const String& strUnCheck);
public:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);

protected:
	PImage GetImage() const;

protected:
	PImage m_pImageChecked;
	PImage m_pImageUnCheck;
};
typedef SharedPtr<CheckBoxItem> PCheckBoxItem;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ToolTip : public Button
{

};
typedef SharedPtr<ToolTip> PToolTip;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
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
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	ULONG m_lOffset;
	PButton m_pItem;
};
typedef SharedPtr<CheckBox> PCheckBox;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API RadioButtonItem : public CheckBoxItem
{
public:
	virtual void OnDraw(HDC hdc, const Rect& rc);
};
typedef SharedPtr<RadioButtonItem> PRadioButtonItem;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API RadioButton : public CheckBox
{
public:
	RadioButton();
};
typedef SharedPtr<RadioButton> PRadioButton;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Label : public Widget
{

};
typedef SharedPtr<Label> PLabel;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ProcessBar : public Widget
{
public:
	ProcessBar();
public:
	void SetRange(ULONG nMax);
	void SetPos(ULONG nPos, BOOL bDraw = TRUE);
	ULONG GetPos() const;
public:
	virtual void OnDraw(HDC hdc, const Rect& rc);
protected:
	ULONG m_nMax;
	ULONG m_nPos;
};
typedef SharedPtr<ProcessBar> PProcessBar;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API InPlaceWid : public Widget
{
public:
	InPlaceWid();
public:
	WFX_BEGIN_MSG_MAP(InPlaceWid)
		WFX_MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	virtual BOOL Initial() = 0;
protected:
	InPlaceWnd* m_pWindow;
};
typedef SharedPtr<InPlaceWid> PInPlaceWid;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API TextBox : public InPlaceWid
{
public:
	TextBox(WORD wMode = WID_TBM_READWRITE);
public:
	WFX_BEGIN_MSG_MAP(TextBox)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		//WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_CHAIN_MSG_MAP(InPlaceWid)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
public:
	void SetMode(WORD wMode, BOOL bAdd = FALSE);
	WORD GetMode() const;
	BOOL IsReadonly() const;
	BOOL IsPassword() const;
protected:
	virtual BOOL Initial();
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	WORD m_wMode;
	BOOL m_bEditting;
};
typedef SharedPtr<TextBox> PTextBox;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
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
typedef SharedPtr<ComboBox> PComboBox;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef SharedPtr<WidDispatch> PWidDispatch;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// Window: Window for controls
class WFX_API Window : public MsgMap
{
public:
	// !!! m_pDispatch must be the first member
	PWidDispatch m_pDispatch;
public:
	Window();
	virtual ~Window();
public:
	HWND GetHwnd() const;
	operator HWND() const;

	BOOL RegisterWindowClass();
	BOOL RegisterSuperClass();

	HWND Create(HWND hwndParent, LPCTSTR pszName,
		DWORD dwStyle, DWORD dwExStyle, const Rect& rc,
		HMENU hMenu = NULL);

	HWND SubClass(HWND hWnd);
	void UnSubClass();
	void ShoWidWnd(BOOL bShow = TRUE, BOOL bTakeFocus = TRUE);
	BOOL ShowModal();
	void Close();
	void CenterWindow();
	void SetIcon(UINT nRes);
	String GetText() const;
	void SetText(const String& strText);
	void SetFont(HFONT hFont) const;
	void GetClientRect(Rect& rc);
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
typedef SharedPtr<Window> PWindow;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API InPlaceWnd : public Window
{
public:
	InPlaceWnd();
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
typedef SharedPtr<InPlaceWnd> PInPlaceWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API TextBoxWnd : public InPlaceWnd
{
public:
	WFX_BEGIN_MSG_MAP(TextBoxWnd)
		WFX_MESSAGE_HANDLER(OCM_COMMAND, OnEditChanged)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_CHAIN_MSG_MAP(InPlaceWnd)
	WFX_END_MSG_MAP()
public:
	virtual void OnInPlaceWindowKillFocus();
	virtual HWND CreateInPlaceWindow();
	virtual LPCTSTR GetSuperClassName() const;
	virtual LPCTSTR GetWindowClassName() const;
public:
	wfx_msg LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
};
typedef SharedPtr<InPlaceWnd> PInPlaceWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
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
	PWidget m_pRoot;
	std::vector<Widget*> m_rgpItems;
};
typedef SharedPtr<InPlaceWnd> PInPlaceWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ToolTipWnd : public Window
{

};
typedef SharedPtr<ToolTipWnd> PToolTipWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Menu : public InPlaceWnd
{

};
typedef SharedPtr<Menu> PMenu;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// Timer: helper class for WidDispatch
struct TimerInfo
{
	TimerInfo()
		: m_nRealTimer(0)
		, m_hWnd(NULL)
		, m_nSrcTimer(0)
	{
		m_pSender.first = INVALID_HWID;
		m_pSender.second = NULL;
	}
	std::pair<HWID, Widget*> m_pSender;
	UINT_PTR m_nRealTimer;
	HWND m_hWnd;
	UINT_PTR m_nSrcTimer;
};
typedef SharedPtr<TimerInfo> PTimerInfo;
typedef std::vector<PTimerInfo>::iterator TimerIter;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class Timer
{
	friend class WidDispatch;
public:
	Timer(WidDispatch* pDispatch);
public:
	UINT_PTR SetWidTimer(Widget* pWid, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillWidTimer(Widget* pWid, UINT_PTR uIDEvent);
	Widget* GetWidgetFromTimer(UINT_PTR uIDEvent);
	void Destroy(Widget* pWid);
	UINT_PTR GenerateTimerID();
protected:
	WidDispatch* m_pDispatch;
	std::map<UINT_PTR, 
		std::pair<HWID, Widget*> > m_Timer;
	std::vector<PTimerInfo> m_rgpTimers; 
	UINT_PTR m_nTimerID;
};
typedef SharedPtr<Timer> PTimer;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// WidDispatch: dispatch messages for widget
class WFX_API WidDispatch : public MsgMap
{
	friend class Widget;
	friend class Timer;
public:
	WidDispatch(HWND hWnd = NULL);
	virtual ~WidDispatch();
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
	void DrawWid(Widget* pWid, const Rect& rcPaint);
	void DrawGen(Widget* pWid, HDC hdc, const Rect& rcPaint);
	void DrawBkgnd(Widget* pWid, HDC hdc, const Rect& rcPaint);
	void OnPaint(const Rect& rcPaint);
	void Invalidate(const Rect& rc);
protected:
	void SetWidRect(Widget* pWid, const Rect& rc);
	void ShowWid(Widget* pWid, WORD wShow);
public:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
public:
	Widget* GetWidPt(POINT pt);
	Widget* GetWidPt(const std::vector<Widget*>& rgpWid);
	Widget* FromHwid(HWID hWid) const;
	void SetCapture(Widget* pWid);
	void SetFocus(Widget* pWid);
	void SetLButtonDown(Widget* pWid);
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
	static Rect FromRect(const Rect& rc);
protected:
	HWND m_hWnd;
	std::vector<HWID> m_rghWid;
	std::map<HWID, Widget*> m_Handle2Object;
	std::map<HWID, Widget*> m_h2oOrphan;
	std::pair<HWID, Widget*> m_h2oLastMouseMove;
	std::pair<HWID, Widget*> m_h2oCaptured;
	std::pair<HWID, Widget*> m_h2oFocused;
	std::pair<HWID, Widget*> m_h2oLButtonDown;
	PTimer m_pTimer;
public:
	static HWID s_hWidBase;
private:
	static HINSTANCE s_hInstance;
};
typedef SharedPtr<WidDispatch> PWidDispatch;

END_NAMESPACE_WFX
