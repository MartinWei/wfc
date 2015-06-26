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
	m_pTimer.reset(new TimerManager(this));
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
	int nMode = ::GetBkMode(hdc);
	int nModeMem = ::GetBkMode(drawdc);
	//::SetMapMode(drawdc)
	Gdiplus::Graphics grph(drawdc);
	grph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
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
			return 1;
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
