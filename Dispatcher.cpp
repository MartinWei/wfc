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
#include "Widget.h"

USING_NAMESPACE_WFX;

HWID Dispatcher::s_hWidBase = INVALID_HWID;

HINSTANCE Dispatcher::s_hInstance = NULL;

Dispatcher::Dispatcher( HWND hWnd /*= NULL*/ )
: m_hWnd(hWnd)
{
	ClearH2O(m_h2oLastMouseMove);
	ClearH2O(m_h2oCaptured);
	m_pTimer.reset(new Timer(this));
}

Dispatcher::~Dispatcher()
{

}

void Dispatcher::SetHwnd( HWND hWnd )
{
	m_hWnd = hWnd;
}

HWND Dispatcher::GetHwnd() const
{
	return m_hWnd;
}

BOOL Dispatcher::Create( Widget* pThis )
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

BOOL Dispatcher::Destroy( HWID& hWid )
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

HWID Dispatcher::GenerateHwid()
{
	return ++s_hWidBase;
}

void Dispatcher::RecycleHwid( HWID& hWid )
{
	hWid = INVALID_HWID;
}

void Dispatcher::DrawWid( Widget* pWid )
{
	ASSERT(m_hWnd != NULL);
	ASSERT(pWid != NULL);
	if (!pWid->IsShow())
	{
		return;
	}
	HDC hdc = ::GetDC(m_hWnd);
	if (hdc == NULL)
	{
		return;
	}
	Gdiplus::RectF rc;
	pWid->GetWidRect(rc);
	__begin_mem_draw
	MemDC drawdc(hdc, FromRect(rc));
	Gdiplus::Graphics grph(drawdc);
	grph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	DrawBkgnd(pWid, rc, grph);
	DrawGen(pWid, grph);
	__end_mem_draw;

	::ReleaseDC(m_hWnd, hdc);
}

void Dispatcher::DrawGen( Widget* pWid, Gdiplus::Graphics& grph)
{
	ASSERT(pWid != NULL);
	
	pWid->OnDraw(grph);
	std::vector<Widget*> rgpChildren;
	pWid->GetChildren(rgpChildren);
	for (std::vector<Widget*>::iterator it = 
		rgpChildren.begin(); it != rgpChildren.end(); ++it)
	{
		ASSERT((*it) != NULL);
		DrawGen((*it), grph);
	}
}

LRESULT Dispatcher::DispatchMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
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
			pWid->SendWidMessage(uMsg, wParam, lParam);
			return lResult;
		}
	}
	switch(uMsg)
	{
	case WM_PAINT:
		// Handle WM_PAINT message
		OnPaint();
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
				pWid->SendWidMessage(WM_MOUSEMOVE, wParam, lParam);

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
				pWid->SendWidMessage(uMsg, wParam, lParam);
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
				pWid->SendWidMessage(uMsg,  wParam, lParam);
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
				pWid->SendWidMessage(uMsg, wParam, lParam);
			}
		}
		break;
	}
	return lResult;
}

Widget* Dispatcher::GetWidPt( POINT pt )
{
	Widget* pWid = NULL;
	std::vector<Widget*> rgpWidInPt;
	for (std::map<HWID, Widget*>::iterator it = m_Handle2Object.begin();
		it != m_Handle2Object.end(); ++it)
	{
		ASSERT(it->second != NULL);
		Gdiplus::RectF rcWid;
		it->second->GetWidRect(rcWid);
		if (::PtInRect(&FromRect(rcWid), pt))
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

Widget* Dispatcher::GetWidPt(const std::vector<Widget*>& rgpWid)
{
	Widget* pWid = NULL;
	std::vector<Widget*>::const_iterator it = rgpWid.begin();
	if (it != rgpWid.end())
	{
		pWid = *it;
		ASSERT(pWid != NULL);
		Gdiplus::RectF rcWid;
		pWid->GetWidRect(rcWid);
		float fArea = rcWid.Width * rcWid.Height;
		float fMinArea = fArea;
		for (; it != rgpWid.end(); ++it)
		{
			pWid->GetWidRect(rcWid);
			fArea = rcWid.Width * rcWid.Height;
			if (fArea < fMinArea)
			{
				fMinArea = fArea;
			}
			pWid = (*it);
		}
	}
	return pWid;
}

HINSTANCE Dispatcher::GetInstance()
{
	return s_hInstance;
}

void Dispatcher::SetInstance( HINSTANCE hInstance )
{
	s_hInstance = hInstance;
}

BOOL Dispatcher::SetParent( Widget* pThis, Widget* pParent )
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

Widget* Dispatcher::FromHwid( HWID hWid ) const
{
	std::map<HWID, Widget*>::const_iterator it = 
		m_Handle2Object.find(hWid);
	if (it != m_Handle2Object.end())
	{
		return it->second;
	}
	return NULL;
}

RECT Dispatcher::FromRect( const Gdiplus::RectF& rc )
{
	RECT rcc = {0};
	rcc.left = (LONG)rc.X;
	rcc.top = (LONG)rc.Y;
	rcc.right = rcc.left + (LONG)rc.Width + 1;
	rcc.bottom = rcc.top + (LONG)rc.Height + 1;
	return rcc;
}

void Dispatcher::OnPaint()
{
	// Note: Only orphans need to handle WM_PAINT message,
	// parents will handle it for their children.
	for (std::map<HWID, Widget*>::iterator it = m_h2oOrphan.begin();
		it != m_h2oOrphan.end(); ++it)
	{
		ASSERT(it->second != NULL);
		it->second->InvalidWid();
	}
}

void Dispatcher::ShowWid( Widget* pWid, WORD wShow )
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

void Dispatcher::SetCapture( Widget* pWid )
{
	ASSERT(m_hWnd != NULL);
	ASSERT(pWid != NULL);
	ASSERT(*pWid != INVALID_HWID);
	m_h2oCaptured = std::make_pair(pWid->GetHwid(), pWid);
	::SetCapture(m_hWnd);
}

void Dispatcher::ReleaseCapture()
{
	ClearH2O(m_h2oCaptured);
	::ReleaseCapture();
}

void Dispatcher::ClearH2O( std::pair<HWID, Widget*>& h2o )
{
	h2o.first = INVALID_HWID;
	h2o.second = NULL;
}

Widget* Dispatcher::GetObject( const std::pair<HWID, Widget*>& h2o )
{
	if (h2o.first != INVALID_HWID)
	{
		ASSERT(h2o.second != NULL);
		return h2o.second;
	}
	return NULL;
}

void Dispatcher::SetMouseMoveH2O( const std::pair<HWID, Widget*>& h2o )
{
	m_h2oLastMouseMove.first = h2o.first;
	m_h2oLastMouseMove.second = h2o.second;
}

void Dispatcher::SetCapturedH2O( const std::pair<HWID, Widget*>& h2o )
{
	m_h2oCaptured.first = h2o.first;
	m_h2oCaptured.second = h2o.second;
}

void Dispatcher::EnableScrollBar( Widget* pWid, UINT uBarFlag, BOOL bEnable /*= TRUE*/ )
{
	ASSERT(pWid != NULL);
	Gdiplus::RectF rcWid;
	pWid->GetWidRect(rcWid);
	Gdiplus::RectF rcSB;
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
			pWid->GetWidRect(rcWid);
		}
		else
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_VERT);
			TDEL(pBar);
			pBar = pWid->GetScrollBar(SB_HORZ);
			TDEL(pBar);
			pWid->SetSBFlag(WESB_NONE);
			pWid->GetWidRect(rcWid);
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
			pWid->GetWidRect(rcWid);
		}
		else
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_HORZ);
			TDEL(pBar);
			pWid->SetSBFlag(pWid->GetSBFlag() & ~WESB_HORZ);
			pWid->GetWidRect(rcWid);
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
			pWid->GetWidRect(rcWid);
		}
		else
		{
			ScrollBar* pBar = pWid->GetScrollBar(WESB_VERT);
			TDEL(pBar);
			pWid->SetSBFlag(pWid->GetSBFlag() & ~WESB_VERT);
			pWid->GetWidRect(rcWid);
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}

void Dispatcher::SetScrollInfo( Widget* pWid, int nBar, LPCSCROLLINFO lpsi, BOOL redraw )
{

}

void Dispatcher::GetScrollInfo( Widget* pWid, int nBar, LPSCROLLINFO lpsi )
{

}

void Dispatcher::PreProcessMsg( Widget* pWid, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

}

void Dispatcher::SetWidRect( Widget* pWid, const Gdiplus::RectF& rc )
{
	Gdiplus::RectF rcWid;
	pWid->GetRect(rcWid);
	Gdiplus::RectF rcSB = rcWid;
	if (pWid->GetSBFlag() & WESB_VERT)
	{
		ASSERT(pWid->GetScrollBar(SB_VERT) != NULL);
		rcSB.X = rcWid.X + rcWid.Width - SIZE_SCROLLBAR - 1;
		rcSB.Width = SIZE_SCROLLBAR;
		rcSB.Y += 1;
		rcSB.Height -= SIZE_SCROLLBAR + 1;
		pWid->GetScrollBar(SB_VERT)->SetWidRect(rcSB);
	}
	rcSB = rcWid;
	if (pWid->GetSBFlag() & WESB_HORZ)
	{
		ASSERT(pWid->GetScrollBar(SB_HORZ) != NULL);
		rcSB.X += 1;//rcWid.X + rcWid.Width - SIZE_SCROLLBAR - 1;
		rcSB.Width -= SIZE_SCROLLBAR + 1;
		rcSB.Y = rcWid.Y + rcWid.Height - SIZE_SCROLLBAR - 1;
		rcSB.Height = SIZE_SCROLLBAR;
		pWid->GetScrollBar(SB_HORZ)->SetWidRect(rcSB);
	}
	pWid->SendWidMessage(WM_SIZE, 0, 0);
}

UINT_PTR Dispatcher::SetWidTimer( Widget* pWid, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc )
{
	return m_pTimer->SetWidTimer(pWid, nIDEvent, uElapse, lpTimerFunc);
}

BOOL Dispatcher::KillWidTimer( Widget* pWid, UINT_PTR uIDEvent )
{
	return m_pTimer->KillWidTimer(pWid, uIDEvent);
}

Widget* Dispatcher::GetWidgetFromTimer( UINT_PTR uIDEvent )
{
	return m_pTimer->GetWidgetFromTimer(uIDEvent);
}

void Dispatcher::DrawBkgnd( Widget* pWid, const Gdiplus::RectF& rc, Gdiplus::Graphics& grph )
{
	ASSERT(pWid != NULL);
	Widget* pParent = pWid->GetParent();

	Gdiplus::Color clrBkgnd;
	if (pParent != NULL)
	{
		clrBkgnd = pParent->GetBkgnd();
		Gdiplus::SolidBrush bkgnd(clrBkgnd);
		grph.FillRectangle(&bkgnd, rc);
		Gdiplus::Pen pnbkgnd(clrBkgnd);
		grph.DrawRectangle(&pnbkgnd, rc);
	}
	else
	{
		//clrBkgnd.SetFromCOLORREF(::GetBkColor(grph.GetHDC()));
	}
}
