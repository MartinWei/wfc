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
#include "StdAfx.h"
#include "wfxwid.h"

USING_NAMESPACE_WFX;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Window::Window()
: m_pDispatch(new WidDispatch)
, m_hWnd(NULL)
, m_OldWndProc(::DefWindowProc)
, m_bSubclassed(FALSE)
{

}

Window::~Window()
{

}

HWND Window::GetHwnd() const
{
	return m_hWnd;
}

Window::operator HWND() const
{
	return m_hWnd;
}

BOOL Window::RegisterWindowClass()
{
	WNDCLASSW wc;
	wc.style = GetClassStyle();
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = Window::__WndProc;
	wc.hInstance = WidDispatch::GetInstance();
	wc.hCursor = ::LoadCursorW(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClassW(&wc);
	DWORD dwError = ::GetLastError();
	WFX_CONDITION(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

BOOL Window::RegisterSuperClass()
{
	// Get the class information from an existing
	// window so we can subclass it later on...
	WNDCLASSEXW wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	if( !::GetClassInfoEx(NULL, GetSuperClassName(), &wc) ) {
		if( !::GetClassInfoEx(NULL, GetSuperClassName(), &wc) ) {
			WFX_CONDITION(!L"Unable to locate window class");
			return NULL;
		}
	}
	m_OldWndProc = wc.lpfnWndProc;
	wc.lpfnWndProc = Window::__ControlProc;
	wc.hInstance = WidDispatch::GetInstance();
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClassExW(&wc);
	DWORD dwError = ::GetLastError();
	WFX_CONDITION(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

HWND Window::Create( HWND hwndParent, LPCTSTR pszName, DWORD dwStyle, DWORD dwExStyle, const Rect& rc, HMENU hMenu /*= NULL*/ )
{
	if( GetSuperClassName() != NULL && !RegisterSuperClass() ) return NULL;
	if( GetSuperClassName() == NULL && !RegisterWindowClass() ) return NULL;
	LPCTSTR pszWindowClassName = GetWindowClassName();
	m_hWnd = ::CreateWindowEx(dwExStyle, pszWindowClassName, pszName, dwStyle, 
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hwndParent, hMenu, WidDispatch::GetInstance(), this);
	WFX_CONDITION(m_hWnd!=NULL);
	return m_hWnd;
}

HWND Window::SubClass( HWND hWnd )
{
	WFX_CONDITION(::IsWindow(hWnd));
	WFX_CONDITION(m_hWnd==NULL);
	m_OldWndProc = SubclassWindow(hWnd, __WndProc);
	if( m_OldWndProc == NULL ) return NULL;
	m_bSubclassed = TRUE;
	m_hWnd = hWnd;
	return m_hWnd;
}

void Window::UnSubClass()
{
	WFX_CONDITION(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	if( !m_bSubclassed ) return;
	SubclassWindow(m_hWnd, m_OldWndProc);
	m_OldWndProc = ::DefWindowProc;
	m_bSubclassed = FALSE;
}

void Window::ShoWidWnd( BOOL bShow /*= TRUE*/, BOOL bTakeFocus /*= TRUE*/ )
{
	WFX_CONDITION(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

BOOL Window::ShowModal()
{
	return FALSE;
}

void Window::Close()
{
	WFX_CONDITION(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	PostMessage(WM_CLOSE);
}

void Window::CenterWindow()
{
	WFX_CONDITION(::IsWindow(m_hWnd));
	WFX_CONDITION((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
	Rect rcDlg;
	::GetWindowRect(m_hWnd, &rcDlg);
	Rect rcArea;
	Rect rcCenter;
	HWND hWndParent = ::GetParent(m_hWnd);
	HWND hWndCenter = ::GetWindowOwner(m_hWnd);
	::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
	if( hWndCenter == NULL ) rcCenter = rcArea; else ::GetWindowRect(hWndCenter, &rcCenter);

	int DlgWidth = rcDlg.right - rcDlg.left;
	int DlgHeight = rcDlg.bottom - rcDlg.top;

	// Find dialog's upper left based on rcCenter
	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	// The dialog is outside the screen, move it inside
	if( xLeft < rcArea.left ) xLeft = rcArea.left;
	else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
	if( yTop < rcArea.top ) yTop = rcArea.top;
	else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
	::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Window::SetIcon( UINT nRes )
{
	HICON hIcon = (HICON)::LoadImage(NULL, MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	WFX_CONDITION(hIcon);
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
	hIcon = (HICON)::LoadImage(NULL, MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	WFX_CONDITION(hIcon);
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);
}

LPCTSTR Window::GetSuperClassName() const
{
	return NULL;
}

UINT Window::GetClassStyle() const
{
	return 0;
}

LRESULT Window::SendMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	WFX_CONDITION(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
}

LRESULT Window::PostMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	WFX_CONDITION(::IsWindow(m_hWnd));
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

void Window::ResizeClient( int cx /*= -1*/, int cy /*= -1*/ )
{
	WFX_CONDITION(::IsWindow(m_hWnd));
	Rect rc;;
	if( !::GetClientRect(m_hWnd, &rc) ) return;
	if( cx != -1 ) rc.right = cx;
	if( cy != -1 ) rc.bottom = cy;
	if( !::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd)) ) return;
	UINT uFlags = SWP_NOZORDER | SWP_NOMOVE;
	::SetWindowPos(m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, uFlags);
}


void Window::OnFinalMessage( HWND hWnd )
{
	
}

LRESULT CALLBACK Window::__WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	Window* pThis = NULL;
	if( uMsg == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<Window*>(lpcs->lpCreateParams);
		pThis->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
	} 
	else {
		pThis = reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if( uMsg == WM_NCDESTROY && pThis != NULL ) {
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
			::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
			if( pThis->m_bSubclassed ) pThis->UnSubClass();
			pThis->m_hWnd = NULL;
			pThis->OnFinalMessage(hWnd);
			return lRes;
		}
	}
	if( pThis != NULL ) {
		LRESULT lResult = 0;
		pThis->ProcessMessage(uMsg, wParam, lParam, lResult, 0);
		return lResult;
	} 
	else {
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK Window::__ControlProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	Window* pThis = NULL;
	if( uMsg == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<Window*>(lpcs->lpCreateParams);
		::SetPropW(hWnd, L"WndX", (HANDLE) pThis);
		pThis->m_hWnd = hWnd;
	} 
	else {
		pThis = reinterpret_cast<Window*>(::GetPropW(hWnd, L"WndX"));
		if( uMsg == WM_NCDESTROY && pThis != NULL ) {
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
			if( pThis->m_bSubclassed ) pThis->UnSubClass();
			::SetPropW(hWnd, L"WndX", NULL);
			pThis->m_hWnd = NULL;
			pThis->OnFinalMessage(hWnd);
			return lRes;
		}
	}
	if( pThis != NULL ) {
		LRESULT lResult = 0;
		pThis->ProcessMessage(uMsg, wParam, lParam, lResult, 0);
		return lResult;
	} 
	else {
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

String Window::GetText() const
{
	WFX_CONDITION(m_hWnd != NULL);
	int nLen = GetWindowTextLengthW(m_hWnd) + 1;
	String strText;
	WCHAR* pszText = new WCHAR[nLen];
	::GetWindowTextW(m_hWnd, pszText, nLen);
	strText = pszText;
	TDEL(pszText);
	return strText;
}

void Window::SetFont( HFONT hFont ) const
{
	SetWindowFont(m_hWnd, hFont, TRUE);
}


void Window::SetText( const String& strText )
{
	WFX_CONDITION(m_hWnd != NULL);
	::SetWindowTextW(m_hWnd, strText.c_str());
}

BOOL Window::ProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	if (m_pDispatch != NULL)
	{
		m_pDispatch->HandleMessage(uMsg, wParam, lParam);
	}
	lResult = ::CallWindowProc(m_OldWndProc, m_hWnd, uMsg, wParam, lParam);
	return TRUE;
}

void Window::GetClientRect( Rect& rc )
{
	WFX_CONDITION(m_hWnd != NULL);
	::GetClientRect(m_hWnd, &rc);
}

UINT_PTR Window::SetTimer( UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc ) const
{
	return ::SetTimer(m_hWnd, nIDEvent, uElapse, lpTimerFunc);
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
InPlaceWnd::InPlaceWnd()
{

}

BOOL InPlaceWnd::Initial( InPlaceWid* pOwner )
{
	WFX_CONDITION(pOwner != NULL);
	m_pOwner = pOwner;
	CreateInPlaceWindow();
	if (m_hWnd != NULL)
	{
		::SetFocus(m_hWnd);
		m_pDispatch->SetHwnd(m_hWnd);
	}
	return FALSE;
}

void InPlaceWnd::OnFinalMessage( HWND hWnd )
{
	delete this;
}

void InPlaceWnd::OnInPlaceWindowKillFocus()
{

}

LRESULT InPlaceWnd::OnKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	OnInPlaceWindowKillFocus();
	PostMessage(WM_CLOSE);
	return 0;
}
