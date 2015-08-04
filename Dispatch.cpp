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
#include "stdafx.h"
#include "wfxwid.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;

HWID WidDispatch::s_hWidBase = INVALID_HWID;

HINSTANCE WidDispatch::s_hInstance = NULL;
WidDispatch::WidDispatch( HWND hWnd /*= NULL*/ )
: m_hWnd(hWnd)
{
	ClearH2O(m_h2oLastMouseMove);
	ClearH2O(m_h2oCaptured);
	ClearH2O(m_h2oFocused);
	ClearH2O(m_h2oLButtonDown);
	m_pTimer.reset(new Timer(this));
}

WidDispatch::~WidDispatch()
{

}

void WidDispatch::SetHwnd( HWND hWnd )
{
	m_hWnd = hWnd;
}

HWND WidDispatch::GetHwnd() const
{
	return m_hWnd;
}

BOOL WidDispatch::Create( Widget* pThis )
{
	WFX_CONDITION(pThis != NULL);
	WFX_CONDITION(pThis->GetHwid() == INVALID_HWID);
	if (pThis->GetHwid() != INVALID_HWID)
	{
		return FALSE;
	}
	HWID hNewWid = GenerateHwid();
	WFX_CONDITION(hNewWid != INVALID_HWID);
	pThis->SetHwid(hNewWid);
	WFX_CONDITION(m_Handle2Object.find(hNewWid) == m_Handle2Object.end());
	m_Handle2Object.insert(std::make_pair(hNewWid, pThis));
	return TRUE;
}

BOOL WidDispatch::Destroy( HWID& hWid )
{
	std::map<HWID, Widget*>::iterator it =
		m_Handle2Object.find(hWid);
	if (it == m_Handle2Object.end())
	{
		return FALSE;
	}
	Widget* pWid = it->second;
	WFX_CONDITION(pWid != NULL);
	Widget* pParent = pWid->GetParent();
	if (pParent != NULL)
	{
		pParent->RemoveChild(pWid);
	}
	m_Handle2Object.erase(it);
	std::map<HWID, Widget*>::iterator itPaint = 
		m_h2oOrphan.find(hWid);
	if (itPaint != m_h2oOrphan.end())
	{
		m_h2oOrphan.erase(itPaint);
	}
	if (m_h2oCaptured.first == hWid)
	{
		ClearH2O(m_h2oCaptured);
	}
	m_pTimer->Destroy(pWid);
	RecycleHwid(hWid);
	return TRUE;
}

HWID WidDispatch::GenerateHwid()
{
	return ++s_hWidBase;
}

void WidDispatch::RecycleHwid( HWID& hWid )
{
	hWid = INVALID_HWID;
}

void WidDispatch::DrawWid( Widget* pWid, const Rect& rcPaint )
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	if (!pWid->IsShow())
	{
		return;
	}

	__begin_mem_draw
	WfxRender::MemDC drawdc(m_hWnd, rcPaint);
	DrawBkgnd(pWid, drawdc, rcPaint);
	DrawGen(pWid, drawdc, rcPaint);
	__end_mem_draw;
}

void WidDispatch::DrawGen( Widget* pWid, HDC hdc, const Rect& rcPaint)
{
	WFX_CONDITION(pWid != NULL);
	Rect rcTemp;
	Rect rcItem = pWid->GetRect();

	if (!::IntersectRect(&rcTemp, &rcPaint, &rcItem))
		return;
	if (!pWid->IsShow())
		return;

	WfxRender::RenderClip clip(hdc, rcItem);
	pWid->OnDraw(hdc, rcPaint);

	std::vector<Widget*> rgpChildren;
	pWid->GetChildren(rgpChildren);
	for (std::vector<Widget*>::iterator it = 
		rgpChildren.begin(); it != rgpChildren.end(); ++it)
	{
		WFX_CONDITION((*it) != NULL);
		if (!::IntersectRect(&rcTemp, &rcItem, &(*it)->GetRect()))
			continue;
		DrawGen((*it), hdc, rcPaint);
	}
}

LRESULT WidDispatch::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 1;
	Point pt(lParam);
	Widget* pWid = NULL;

	if (uMsg != WM_PAINT
		&& uMsg != WM_TIMER
		&& uMsg != WM_LBUTTONUP)

	{
		pWid = GetObject(m_h2oCaptured);
		if (pWid != NULL)
		{
			lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
			return lResult;
		}
	}
	ProcessMessage(uMsg, wParam, lParam, lResult, 0);
	return lResult;
	
}

BOOL WidDispatch::ProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	switch(uMsg)
	{
	case WM_ERASEBKGND:
		return lResult;
	case WM_PAINT:
		{
			// Handle WM_PAINT message
			Rect rcPaint;
			if( !::GetUpdateRect(m_hWnd, &rcPaint, FALSE) ) 
				return 1;
			PAINTSTRUCT ps;
			::BeginPaint(m_hWnd, &ps);
			OnPaint(ps.rcPaint);
			::EndPaint(m_hWnd, &ps);
		}
		break;
	case WM_MOUSEMOVE:
		{
			pWid = GetWidPt(pt);
			if (pWid == NULL)
			{
				pWid = GetObject(m_h2oLastMouseMove);
				if (pWid != NULL)
				{
					pWid->SendWidMessage(WM_MOUSELEAVE);
					ClearH2O(m_h2oLastMouseMove);
					return lResult;
				}
			}
			else
			{
				HWID hWidNowMouse = pWid->GetHwid();
				if (m_h2oLastMouseMove.first != hWidNowMouse)
				{
					Widget* pLastMouse = GetObject(m_h2oLastMouseMove);
					if (pLastMouse != NULL)
					{
						pLastMouse->SendWidMessage(WM_MOUSELEAVE);
					}
					SetMouseMoveH2O(std::make_pair(hWidNowMouse, pWid));
				}
				lResult = pWid->SendWidMessage(WM_MOUSEMOVE, wParam, lParam);

				return lResult;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			WFX_CONDITION(m_hWnd != NULL);
			::SetFocus(m_hWnd);
			pWid = GetWidPt(pt);
			if (pWid != NULL)
			{
				if (pWid->GetHwid() != m_h2oFocused.first
					&& pWid != m_h2oFocused.second)
				{
					if (m_h2oFocused.second != NULL)
					{
						m_h2oFocused.second->SendWidMessage(WM_KILLFOCUS, (WPARAM)pWid->GetHwid());
					}
					pWid->SendWidMessage(WM_SETFOCUS, (WPARAM)m_h2oFocused.first);
				}
				SetCapture(pWid);
				SetFocus(pWid);
				SetLButtonDown(pWid);
				lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
			}
			else
			{
				if (m_h2oFocused.second != NULL)
				{
					m_h2oFocused.second->SendWidMessage(WM_KILLFOCUS, INVALID_HWID);
				}
			}
		}
		break;
	case  WM_LBUTTONUP:
		{
			pWid = GetObject(m_h2oCaptured);
			if (pWid == NULL)
			{
				pWid = GetWidPt(pt);
			}
			if (m_h2oCaptured.first != INVALID_HWID)
			{
				ReleaseCapture();
				ClearH2O(m_h2oCaptured);
			}
			if (pWid != NULL)
			{
				lResult = pWid->SendWidMessage(uMsg,  wParam, lParam);
				Widget* pLastMM = GetObject(m_h2oLastMouseMove);
				if (pLastMM != NULL)
				{
					pLastMM->SendWidMessage(WM_MOUSELEAVE);
				}
				ClearH2O(m_h2oLastMouseMove);
			}
			pWid = GetWidPt(pt);
			if (pWid != NULL
				&& m_h2oLButtonDown.first == pWid->GetHwid()
				&& m_h2oLButtonDown.second == pWid)
			{
				pWid->SendWidMessage(WUM_LBUTTONCLICK, wParam, lParam);
			}
			ClearH2O(m_h2oLButtonDown);
		}
		break;
	case WM_TIMER:
		{
			pWid = GetWidgetFromTimer((UINT_PTR)wParam);
			if (pWid != NULL)
			{
				lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
			}
		}
		break;
	case WM_KEYDOWN:
	case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
		{
			pWid = GetObject(m_h2oFocused);
			if (pWid != NULL)
			{
				lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
			}
		}
		break;
	case WM_COMMAND:
		{
			if (lParam == 0)
			{
				break;
			}
			HWND hWndChild = (HWND)lParam;
			lResult = ::SendMessageW(hWndChild, OCM__BASE + uMsg, wParam, lParam);
			break;
		}
	default:
		lResult = 0;
	}
	return lResult;
}

Widget* WidDispatch::GetWidPt( POINT pt )
{
	Widget* pWid = NULL;
	std::vector<Widget*> rgpWidInPt;
	for (std::map<HWID, Widget*>::iterator it = m_Handle2Object.begin();
		it != m_Handle2Object.end(); ++it)
	{
		WFX_CONDITION(it->second != NULL);
		Rect rcWid = it->second->GetRect();
		if (rcWid.PtInRect(pt))
		{
			rgpWidInPt.push_back(it->second);
		}
	}
	if (rgpWidInPt.size() > 0)
	{
		if (rgpWidInPt.size() != 1)
		{
			pWid = GetWidPt(rgpWidInPt);
		}
		else
		{
			pWid = rgpWidInPt[0];
		}
	}

	return pWid;
}

Widget* WidDispatch::GetWidPt(const std::vector<Widget*>& rgpWid)
{
	Widget* pWid = NULL;
	std::vector<Widget*>::const_iterator it = rgpWid.begin();
	if (it != rgpWid.end())
	{
		pWid = *it;
		WFX_CONDITION(pWid != NULL);
		Rect rcWid = pWid->GetRect();
		float fArea = (rcWid.right - rcWid.left) * (rcWid.bottom - rcWid.top);
		float fMinArea = fArea;
		for (; it != rgpWid.end(); ++it)
		{
			rcWid = pWid->GetRect();
			fArea = (rcWid.right - rcWid.left) * (rcWid.bottom - rcWid.top);
			if (fArea < fMinArea)
			{
				fMinArea = fArea;
			}
			pWid = (*it);
		}
	}
	return pWid;
}

HINSTANCE WidDispatch::GetInstance()
{
	return s_hInstance;
}

void WidDispatch::SetInstance( HINSTANCE hInstance )
{
	s_hInstance = hInstance;
}

BOOL WidDispatch::SetParent( Widget* pThis, Widget* pParent )
{
	WFX_CONDITION(pThis != NULL);
	if (pParent == NULL)
	{
		HWID hWid = pThis->GetHwid();
		std::map<HWID, Widget*>::iterator it = 
			m_h2oOrphan.find(hWid);
		if (it == m_h2oOrphan.end())
		{
			m_h2oOrphan.insert(std::make_pair(hWid, pThis));
		}
		return FALSE;
	}

	ULONG nChilren = pParent->GetChildren();
	if (pParent->AddChild(pThis) != nChilren + 1)
	{
		return FALSE;
	}
	pThis->SetMyParent(pParent);
	return TRUE;
}

Widget* WidDispatch::FromHwid( HWID hWid ) const
{
	std::map<HWID, Widget*>::const_iterator it = 
		m_Handle2Object.find(hWid);
	if (it != m_Handle2Object.end())
	{
		return it->second;
	}
	return NULL;
}

Rect WidDispatch::FromRect( const Rect& rc )
{
	Rect rcc;
	return rcc;
}

void WidDispatch::OnPaint(const Rect& rcPaint)
{
	// Note: Only orphans need to handle WM_PAINT message,
	// parents will handle it for their children.
	for (std::map<HWID, Widget*>::iterator it = m_h2oOrphan.begin();
		it != m_h2oOrphan.end(); ++it)
	{
		WFX_CONDITION(it->second != NULL);
		DrawWid(it->second, rcPaint);
	}
}

void WidDispatch::ShowWid( Widget* pWid, WORD wShow )
{
	WFX_CONDITION(pWid != NULL);
	pWid->MyShowWid(wShow);
	std::vector<Widget*> rgpChilren;
	pWid->GetChildren(rgpChilren);
	for (std::vector<Widget*>::iterator it = 
		rgpChilren.begin(); it != rgpChilren.end(); ++it)
	{
		ShowWid((*it), wShow);
	}
}

void WidDispatch::SetCapture( Widget* pWid )
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(*pWid != INVALID_HWID);
	m_h2oCaptured = std::make_pair(pWid->GetHwid(), pWid);
	::SetCapture(m_hWnd);
}

void WidDispatch::SetFocus(Widget* pWid)
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(*pWid != INVALID_HWID);
	m_h2oFocused = std::make_pair(pWid->GetHwid(), pWid);
}

void WidDispatch::SetLButtonDown(Widget* pWid)
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(*pWid != INVALID_HWID);
	m_h2oLButtonDown = std::make_pair(pWid->GetHwid(), pWid);
}
void WidDispatch::ReleaseCapture()
{
	ClearH2O(m_h2oCaptured);
	::ReleaseCapture();
}

void WidDispatch::ClearH2O( std::pair<HWID, Widget*>& h2o )
{
	h2o.first = INVALID_HWID;
	h2o.second = NULL;
}

Widget* WidDispatch::GetObject( const std::pair<HWID, Widget*>& h2o )
{
	if (h2o.first != INVALID_HWID)
	{
		WFX_CONDITION(h2o.second != NULL);
		return h2o.second;
	}
	return NULL;
}

void WidDispatch::SetMouseMoveH2O( const std::pair<HWID, Widget*>& h2o )
{
	m_h2oLastMouseMove.first = h2o.first;
	m_h2oLastMouseMove.second = h2o.second;
}

void WidDispatch::SetCapturedH2O( const std::pair<HWID, Widget*>& h2o )
{
	m_h2oCaptured.first = h2o.first;
	m_h2oCaptured.second = h2o.second;
}

void WidDispatch::EnableScrollBar( Widget* pWid, UINT uBarFlag, BOOL bEnable /*= TRUE*/ )
{
	WFX_CONDITION(pWid != NULL);
	Rect rcWid = pWid->GetRect();
	Rect rcSB;
	if (uBarFlag == WESB_BOTH)
	{
		if (bEnable)
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_VERT);
			if (pBar == NULL)
			{
				pBar = new ScrollBar(SB_VERT);
				pBar->Create(rcSB, this, pWid);
				pWid->SetScrollBar(SB_VERT, pBar);
			}
			pBar = pWid->GetScrollBar(SB_HORZ);

			if (pBar == NULL)
			{
				pBar = new ScrollBar(SB_HORZ);
				pBar->Create(rcSB, this, pWid);
				pWid->SetScrollBar(SB_HORZ, pBar);
			}
			pWid->SetSBFlag(WESB_BOTH);
			rcWid = pWid->GetRect();
		}
		else
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_VERT);
			TDEL(pBar);
			pBar = pWid->GetScrollBar(SB_HORZ);
			TDEL(pBar);
			pWid->SetSBFlag(WESB_NONE);
			rcWid = pWid->GetRect();
		}
	}
	else if (uBarFlag == WESB_HORZ)
	{
		if (bEnable)
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_HORZ);
			if (pBar == NULL)
			{
				pBar = new ScrollBar(SB_HORZ);
				pBar->Create(rcSB, this, pWid);
				pWid->SetScrollBar(SB_HORZ, pBar);
			}
			pWid->SetSBFlag(pWid->GetSBFlag() | WESB_HORZ);
			rcWid = pWid->GetRect();
		}
		else
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_HORZ);
			TDEL(pBar);
			pWid->SetSBFlag(pWid->GetSBFlag() & ~WESB_HORZ);
			rcWid = pWid->GetRect();
		}
	}
	else if (uBarFlag == WESB_VERT)
	{
		if (bEnable)
		{
			ScrollBar* pBar = pWid->GetScrollBar(WESB_VERT);
			if (pBar == NULL)
			{
				pBar = new ScrollBar(WESB_VERT);
				pBar->Create(rcSB, this, pWid);
				pWid->SetScrollBar(WESB_VERT, pBar);
			}
			pWid->SetSBFlag(pWid->GetSBFlag() | WESB_VERT);
			rcWid = pWid->GetRect();
		}
		else
		{
			ScrollBar* pBar = pWid->GetScrollBar(WESB_VERT);
			TDEL(pBar);
			pWid->SetSBFlag(pWid->GetSBFlag() & ~WESB_VERT);
			rcWid = pWid->GetRect();
		}
	}
	else
	{
		WFX_CONDITION(FALSE);
	}
}

void WidDispatch::SetScrollInfo( Widget* pWid, int nBar, LPCSCROLLINFO lpsi, BOOL redraw )
{

}

void WidDispatch::GetScrollInfo( Widget* pWid, int nBar, LPSCROLLINFO lpsi )
{

}

void WidDispatch::PreProcessMsg( Widget* pWid, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

}

void WidDispatch::SetWidRect( Widget* pWid, const Rect& rc )
{
	WFX_CONDITION(pWid != NULL);

	Rect rcWid;
	rcWid = pWid->GetRect();
	Rect rcSB = rcWid;
	Rect rcDraw = rcWid;
	Size sz(pWid->SendWidMessage(WUM_QUERY_VIRTUAL_SIZE));
	BOOL bOutRangeHorz = rcWid.right - rcWid.left < sz.cx;
	BOOL bOutRangeVert = rcWid.bottom - rcWid.top < sz.cy;
	if (pWid->GetSBFlag() & WESB_VERT)
	{
		WFX_CONDITION(pWid->GetScrollBar(SB_VERT) != NULL);
		if (!bOutRangeVert)
		{
			memset(&rcSB, 0, sizeof(Rect));
		}
		else
		{
			rcSB.left = rcWid.right - SIZE_SCROLLBAR - 1;
			rcSB.right = rcSB.left + SIZE_SCROLLBAR;
			rcSB.top += 1;
			rcSB.bottom -= SIZE_SCROLLBAR + 1;
		}
		pWid->GetScrollBar(SB_VERT)->SetRect(rcSB);
		rcDraw.right -= rcSB.right - rcSB.left;
	}
	rcSB = rcWid;
	if (pWid->GetSBFlag() & WESB_HORZ)
	{
		WFX_CONDITION(pWid->GetScrollBar(SB_HORZ) != NULL);
		if (!bOutRangeHorz)
		{
			memset(&rcSB, 0, sizeof(Rect));
		}
		else
		{
			rcSB.left += 1;
			rcSB.right -= SIZE_SCROLLBAR + 1;
			rcSB.top = rcWid.top + rcWid.bottom - rcWid.top - SIZE_SCROLLBAR - 1;
			rcSB.bottom = rcSB.top + SIZE_SCROLLBAR;
		}
		pWid->GetScrollBar(SB_HORZ)->SetRect(rcSB);
		rcDraw.bottom -= rcSB.bottom - rcSB.top;
	}
	pWid->SetDrawRect(rcDraw);
	pWid->SendWidMessage(WM_SIZE, 0, 0);
}

UINT_PTR WidDispatch::SetWidTimer( Widget* pWid, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc )
{
	return m_pTimer->SetWidTimer(pWid, nIDEvent, uElapse, lpTimerFunc);
}

BOOL WidDispatch::KillWidTimer( Widget* pWid, UINT_PTR uIDEvent )
{
	return m_pTimer->KillWidTimer(pWid, uIDEvent);
}

Widget* WidDispatch::GetWidgetFromTimer( UINT_PTR uIDEvent )
{
	return m_pTimer->GetWidgetFromTimer(uIDEvent);
}

void WidDispatch::Invalidate( const Rect& rc )
{
	::InvalidateRect(m_hWnd, &rc, FALSE);
}

void WidDispatch::DrawBkgnd( Widget* pWid, HDC hdc, const Rect& rc )
{
	//WFX_CONDITION(pWid != NULL);
	//Widget* pParent = pWid->GetParent();

	//Gdiplus::Color clrBkgnd;
	//if (pParent != NULL)
	//{
	//	clrBkgnd = pParent->GetBkgnd();
	//	Gdiplus::SolidBrush bkgnd(clrBkgnd);
	//	grph.FillRectangle(&bkgnd, rc);
	//	Gdiplus::Pen pnbkgnd(clrBkgnd);
	//	grph.DrawRectangle(&pnbkgnd, rc);
	//}
	//else
	//{
	//	//clrBkgnd.SetFromCOLORREF(::GetBkColor(grph.GetHDC()));
	//}
}
