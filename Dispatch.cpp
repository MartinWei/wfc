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
	ASSERT(pThis != NULL);
	ASSERT(pThis->GetHwid() == INVALID_HWID);
	if (pThis->GetHwid() != INVALID_HWID)
	{
		return FALSE;
	}
	HWID hNewWid = GenerateHwid();
	ASSERT(hNewWid != INVALID_HWID);
	pThis->SetHwid(hNewWid);
	ASSERT(m_Handle2Object.find(hNewWid) == m_Handle2Object.end());
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
	ASSERT(pWid != NULL);
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

void WidDispatch::DrawWid( Widget* pWid, const RECT& rcPaint )
{
	ASSERT(m_hWnd != NULL);
	ASSERT(pWid != NULL);
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

void WidDispatch::DrawGen( Widget* pWid, HDC hdc, const RECT& rcPaint)
{
	ASSERT(pWid != NULL);
	RECT rcTemp = {0};
	RECT rcItem = pWid->GetRect();

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
		ASSERT((*it) != NULL);
		if (!::IntersectRect(&rcTemp, &rcItem, &(*it)->GetRect()))
			continue;
		DrawGen((*it), hdc, rcPaint);
	}
}

LRESULT WidDispatch::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 1;
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
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
	switch(uMsg)
	{
	case WM_ERASEBKGND:
		return lResult;
	case WM_PAINT:
		{
		// Handle WM_PAINT message
		RECT rcPaint = { 0 };
		if( !::GetUpdateRect(m_hWnd, &rcPaint, FALSE) ) 
			return 1;
		PAINTSTRUCT ps = { 0 };
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
			ASSERT(m_hWnd != NULL);
			::SetFocus(m_hWnd);
			pWid = GetWidPt(pt);
			if (pWid != NULL)
			{
				SetCapture(pWid);
				SetFocus(pWid);
				lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
			}
		}
		break;
	case  WM_LBUTTONUP:
		{
			pWid = GetObject(m_h2oCaptured);
			if (pWid == NULL)
				pWid = GetWidPt(pt);
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
		{
			pWid = GetObject(m_h2oFocused);
			if (pWid != NULL)
			{
				lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
			}
		}
		break;
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
		ASSERT(it->second != NULL);
		RECT rcWid = it->second->GetRect();
		if (::PtInRect(&rcWid, pt))
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
		ASSERT(pWid != NULL);
		RECT rcWid = pWid->GetRect();
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
	ASSERT(pThis != NULL);
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

RECT WidDispatch::FromRect( const RECT& rc )
{
	RECT rcc = {0};
	return rcc;
}

void WidDispatch::OnPaint(const RECT& rcPaint)
{
	// Note: Only orphans need to handle WM_PAINT message,
	// parents will handle it for their children.
	for (std::map<HWID, Widget*>::iterator it = m_h2oOrphan.begin();
		it != m_h2oOrphan.end(); ++it)
	{
		ASSERT(it->second != NULL);
		DrawWid(it->second, rcPaint);
	}
}

void WidDispatch::ShowWid( Widget* pWid, WORD wShow )
{
	ASSERT(pWid != NULL);
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
	ASSERT(m_hWnd != NULL);
	ASSERT(pWid != NULL);
	ASSERT(*pWid != INVALID_HWID);
	m_h2oCaptured = std::make_pair(pWid->GetHwid(), pWid);
	::SetCapture(m_hWnd);
}

void WidDispatch::SetFocus(Widget* pWid)
{
	ASSERT(m_hWnd != NULL);
	ASSERT(pWid != NULL);
	ASSERT(*pWid != INVALID_HWID);
	m_h2oFocused = std::make_pair(pWid->GetHwid(), pWid);
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
		ASSERT(h2o.second != NULL);
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
	ASSERT(pWid != NULL);
	RECT rcWid = pWid->GetRect();
	RECT rcSB = {0};
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
		ASSERT(FALSE);
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

void WidDispatch::SetWidRect( Widget* pWid, const RECT& rc )
{
	ASSERT(pWid != NULL);

	RECT rcWid;
	rcWid = pWid->GetRect();
	RECT rcSB = rcWid;
	RECT rcDraw = rcWid;
	SIZE sz = {0};
	LRESULT lResult = pWid->SendWidMessage(WUM_GET_VIRTUAL_SIZE);
	sz.cx = LOWORD(lResult);
	sz.cy = HIWORD(lResult);
	BOOL bOutRangeHorz = rcWid.right - rcWid.left < sz.cx;
	BOOL bOutRangeVert = rcWid.bottom - rcWid.top < sz.cy;
	if (pWid->GetSBFlag() & WESB_VERT)
	{
		ASSERT(pWid->GetScrollBar(SB_VERT) != NULL);
		if (!bOutRangeVert)
		{
			memset(&rcSB, 0, sizeof(RECT));
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
		ASSERT(pWid->GetScrollBar(SB_HORZ) != NULL);
		if (!bOutRangeHorz)
		{
			memset(&rcSB, 0, sizeof(RECT));
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

void WidDispatch::Invalidate( const RECT& rc )
{
	::InvalidateRect(m_hWnd, &rc, FALSE);
}

void WidDispatch::DrawBkgnd( Widget* pWid, HDC hdc, const RECT& rc )
{
	//ASSERT(pWid != NULL);
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
