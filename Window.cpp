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
#include "Widget.h"

USING_NAMESPACE_WFX;

Window::Window()
: m_pDispatch(new Dispatcher)
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
	WNDCLASS wc = { 0 };
	wc.style = GetClassStyle();
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = Window::__WndProc;
	wc.hInstance = Dispatcher::GetInstance();
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClass(&wc);
	DWORD dwError = ::GetLastError();
	ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

BOOL Window::RegisterSuperClass()
{
	// Get the class information from an existing
	// window so we can subclass it later on...
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	if( !::GetClassInfoEx(NULL, GetSuperClassName(), &wc) ) {
		if( !::GetClassInfoEx(NULL, GetSuperClassName(), &wc) ) {
			ASSERT(!"Unable to locate window class");
			return NULL;
		}
	}
	m_OldWndProc = wc.lpfnWndProc;
	wc.lpfnWndProc = Window::__ControlProc;
	wc.hInstance = Dispatcher::GetInstance();
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClassEx(&wc);
	DWORD dwError = ::GetLastError();
	ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

HWND Window::Create( HWND hwndParent, LPCTSTR pszName, DWORD dwStyle, DWORD dwExStyle, const RECT& rc, HMENU hMenu /*= NULL*/ )
{
	if( GetSuperClassName() != NULL && !RegisterSuperClass() ) return NULL;
	if( GetSuperClassName() == NULL && !RegisterWindowClass() ) return NULL;
	LPCTSTR pszWindowClassName = GetWindowClassName();
	m_hWnd = ::CreateWindowEx(dwExStyle, pszWindowClassName, pszName, dwStyle, 
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hwndParent, hMenu, Dispatcher::GetInstance(), this);
	ASSERT(m_hWnd!=NULL);
	return m_hWnd;
}

HWND Window::SubClass( HWND hWnd )
{
	ASSERT(::IsWindow(hWnd));
	ASSERT(m_hWnd==NULL);
	m_OldWndProc = SubclassWindow(hWnd, __WndProc);
	if( m_OldWndProc == NULL ) return NULL;
	m_bSubclassed = TRUE;
	m_hWnd = hWnd;
	return m_hWnd;
}

void Window::UnSubClass()
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	if( !m_bSubclassed ) return;
	SubclassWindow(m_hWnd, m_OldWndProc);
	m_OldWndProc = ::DefWindowProc;
	m_bSubclassed = FALSE;
}

void Window::ShoWidWnd( BOOL bShow /*= TRUE*/, BOOL bTakeFocus /*= TRUE*/ )
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

BOOL Window::ShowModal()
{
	return FALSE;
}

void Window::Close()
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	PostMessage(WM_CLOSE);
}

void Window::CenterWindow()
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
	RECT rcDlg = { 0 };
	::GetWindowRect(m_hWnd, &rcDlg);
	RECT rcArea = { 0 };
	RECT rcCenter = { 0 };
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
	ASSERT(hIcon);
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
	hIcon = (HICON)::LoadImage(NULL, MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	ASSERT(hIcon);
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

LRESULT Window::SendMessage( UINT nMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, nMsg, wParam, lParam);
}

LRESULT Window::PostMessage( UINT nMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	ASSERT(::IsWindow(m_hWnd));
	return ::PostMessage(m_hWnd, nMsg, wParam, lParam);
}

void Window::ResizeClient( int cx /*= -1*/, int cy /*= -1*/ )
{
	ASSERT(::IsWindow(m_hWnd));
	RECT rc = { 0 };;
	if( !::GetClientRect(m_hWnd, &rc) ) return;
	if( cx != -1 ) rc.right = cx;
	if( cy != -1 ) rc.bottom = cy;
	if( !::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd)) ) return;
	UINT uFlags = SWP_NOZORDER | SWP_NOMOVE;
	::SetWindowPos(m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, uFlags);
}

LRESULT Window::HandleMessage( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
	return ::CallWindowProc(m_OldWndProc, m_hWnd, nMsg, wParam, lParam);
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
		return pThis->HandleMessage(uMsg, wParam, lParam);
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
		return pThis->HandleMessage(uMsg, wParam, lParam);
	} 
	else {
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
