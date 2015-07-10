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
class WfxDispatch;
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
};

typedef BOOL (*DrawFunction)(Widget* pWid, Gdiplus::Graphics& grph);

//////////////////////////////////////////////////////////////////////////
// Widget: the root class of ui classes
class WFX_API Widget :
	public WidgetBase
{
	friend class WfxDispatch;
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
	WFX_END_MSG_MAP()

public:
	BOOL Create(const Gdiplus::RectF& rc, WfxDispatch* pDispatch,
		Widget* pParent = NULL, BOOL bNC = FALSE);

	// Position
public:
	void GetRect(Gdiplus::RectF& rc) const;
	void GetWidRect(Gdiplus::RectF& rc);
	void SetWidRect(const Gdiplus::RectF& rc);
	void ShowWid(WORD wShow);
	BOOL IsShow() const;
	void SetCapture();
	void ReleaseCapture();
	virtual void EstimateVirtualSize();

	// Size
	Gdiplus::SizeF GetVirtualSize() const;
	void SetVirtualSize(const Gdiplus::SizeF& sz);

protected:
	void MyShowWid(WORD wShow);

private:
	void SetRect(const Gdiplus::RectF& rc);
public:
	// Generation
	Widget* GetParent() const;
	void SetParent(Widget* pParent);

	// For WfxDispatch
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
	virtual void OnDraw(Gdiplus::Graphics& grph);

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
	void SetFormat(const SharedPtr<Gdiplus::StringFormat>& pFormat);
	const SharedPtr<Gdiplus::StringFormat> GetFormat() const;
	void SetFont(const SharedPtr<Gdiplus::Font>& pFont);
	const SharedPtr<Gdiplus::Font> GetFont() const;
	void SetBkgnd(const Gdiplus::Color& clrBkgnd);
	Gdiplus::Color GetBkgnd() const;
	void SetFrameClr(const Gdiplus::Color& clrFrame);
	Gdiplus::Color GetFrameClr() const;
	void SetTextClr(const Gdiplus::Color& clrText);
	Gdiplus::Color GetTextColor() const;
	void SetDrawFunction(DrawFunction pDrawFun);
	DrawFunction GetDrawFunction() const;
	virtual HFONT GetFontObject() const;
public:
	BOOL SendWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL PostWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

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
private:
	WORD m_wState;

private:
	void InitFont();

	// Identifier
public:
	HWID m_hWid;
	WfxDispatch* m_pDispatch;
protected:
	std::wstring m_strText;
	SharedPtr<Gdiplus::StringFormat> m_pFormat;
	SharedPtr<Gdiplus::Font> m_pFont;
	Gdiplus::Color m_clrBkgnd;
	Gdiplus::Color m_clrFrame;
	Gdiplus::Color m_clrText;
	DrawFunction m_pDrawFun;
private:
	// Position
	Gdiplus::RectF m_rc;
	Gdiplus::RectF m_rcWid;
	BOOL m_bNC;
	WORD m_wShow;

	// Generation
	Widget* m_pParent;
	std::vector<Widget*> m_rgpChildren;

	// Scrollbar
	ScrollBar* m_pHScrollbar;
	ScrollBar* m_pVScrollbar;
	UINT m_uBarFlag;
	// Timers
	std::vector<UINT_PTR> m_rgTimer;

	// Size
	Gdiplus::SizeF m_szVirtual;
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
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	int GetBar() const;
	void SetBar(int nBar);
	void GetScrollInfo(SCROLLINFO* pScrollInfo) const;
	void SetScrollInfo(const SCROLLINFO* pScrollInfo);
	void SetPos(int nPos);
	int GetPos() const;
protected:
	float GetSliderSize() const;
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
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
	virtual void OnDraw(Gdiplus::Graphics& grph);
protected:
	SharedPtr<Gdiplus::GraphicsPath> m_pGrphPath;
};

class WFX_API ImageWid : public RoundWid
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

class WFX_API Button : public ImageWid
{
public:
	Button(BOOL bCheckState = FALSE);
	virtual ~Button();
public:
	virtual void OnDraw(Gdiplus::Graphics& grph);

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
	virtual void OnDraw(Gdiplus::Graphics& grph);

protected:
	SharedPtr<Gdiplus::Image> GetImage() const;

public:
	static BOOL DrawCheckBoxItem(Widget* pWid, Gdiplus::Graphics& grph);

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
	virtual void OnDraw(Gdiplus::Graphics& grph);
protected:
	ULONG m_lOffset;
	SharedPtr<CheckBoxItem> m_pItem;
};

class WFX_API RadioButtonItem : public CheckBoxItem
{
public:
	virtual void OnDraw(Gdiplus::Graphics& grph);
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
	SharedPtr<WfxDispatch> m_pDispatch;
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
// Timer: helper class for WfxDispatch
class Timer
{
	friend class WfxDispatch;
public:
	Timer(WfxDispatch* pDispatch);
	~Timer();
public:
	UINT_PTR SetWidTimer(Widget* pWid, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillWidTimer(Widget* pWid, UINT_PTR uIDEvent);
	Widget* GetWidgetFromTimer(UINT_PTR uIDEvent);
	void Destroy(Widget* pWid);
protected:
	WfxDispatch* m_pDispatch;
	std::map<UINT_PTR, 
		std::pair<HWID, Widget*> > m_Timer;
};

//////////////////////////////////////////////////////////////////////////
// WfxDispatch: dispatch messages for widget
class WFX_API WfxDispatch
{
	friend class Widget;
	friend class Timer;
public:
	WfxDispatch(HWND hWnd = NULL);
	~WfxDispatch();
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
	void DrawWid(Widget* pWid);
	void DrawGen(Widget* pWid, Gdiplus::Graphics& grph);
	void DrawBkgnd(Widget* pWid, const Gdiplus::RectF& rc, Gdiplus::Graphics& grph);
	void OnPaint();
protected:
	void SetWidRect(Widget* pWid, const Gdiplus::RectF& rc);
	void ShowWid(Widget* pWid, WORD wShow);
public:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	Widget* GetWidPt(POINT pt);
	Widget* GetWidPt(const std::vector<Widget*>& rgpWid);
	Widget* FromHwid(HWID hWid) const;
	void SetCapture(Widget* pWid);
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
	static RECT FromRect(const Gdiplus::RectF& rc);
protected:
	HWND m_hWnd;
	std::vector<HWID> m_rghWid;
	std::map<HWID, Widget*> m_Handle2Object;
	std::map<HWID, Widget*> m_h2oOrphan;
	std::pair<HWID, Widget*> m_h2oLastMouseMove;
	std::pair<HWID, Widget*> m_h2oCaptured;
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
	virtual SharedPtr<WfxDispatch> Parse(const std::wstring& strXml);
};

END_NAMESPACE_WFX