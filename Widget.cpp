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

Widget::Widget(void)
: m_bNC(FALSE)
, m_pParent(NULL)
, m_pDispatch(NULL)
, m_wState(WID_STATE_STATIC)
, m_hWid(INVALID_HWID)
, m_pVScrollbar(NULL)
, m_pHScrollbar(NULL)
, m_strText(WID_DEFAULT_TEXT)
, m_uBarFlag(WESB_NONE)
, m_wShow(SW_SHOW)
{
	InitFont();
}

Widget::~Widget(void)
{
	if (m_pDispatch != NULL)
	{
		m_pDispatch->Destroy(m_hWid);
	}
	TDEL(m_pVScrollbar);
	TDEL(m_pHScrollbar);
}

void Widget::GetRect( Gdiplus::RectF& rc ) const 
{
	rc = m_rc;
}

void Widget::GetWidRect( Gdiplus::RectF& rc )
{
	rc = m_rcWid;
}

void Widget::SetWidRect( const Gdiplus::RectF& rc )
{
	m_rcWid = rc;
	SetRect(rc);
	m_pDispatch->SetWidRect(this, rc);
}

void Widget::SetRect( const Gdiplus::RectF& rc )
{
	m_rc = rc;
}

BOOL Widget::Create( const Gdiplus::RectF& rc, Dispatcher* pDispatch, 
					Widget* pParent /*= NULL*/, BOOL bNC /*= FALSE*/ )
{
	ASSERT(pDispatch != NULL);
	m_pDispatch = pDispatch;
	m_pDispatch->Create(this);
	m_bNC = bNC;
	SetParent(pParent);
	SetWidRect(rc);
	SendWidMessage(WM_CREATE, 0, 0);
	return TRUE;
}

Widget* Widget::GetParent() const
{
	return m_pParent;
}

void Widget::SetParent( Widget* pParent )
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->SetParent(this, pParent);
}

ULONG Widget::GetChildren( std::vector<Widget*>& rgpChildren ) const
{
	rgpChildren = m_rgpChildren;
	return rgpChildren.size();
}

ULONG Widget::GetChildren() const
{
	return m_rgpChildren.size();
}

BOOL Widget::HasChild() const
{
	return m_rgpChildren.size() != 0;
}

void Widget::InvalidWid()
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->DrawWid(this);
}

void Widget::OnDraw( Gdiplus::Graphics& grph )
{
	Gdiplus::RectF rc;
	GetRect(rc);
	Gdiplus::SolidBrush bkgnd(m_clrBkgnd);
	grph.FillRectangle(&bkgnd, rc.X, rc.Y, rc.Width, rc.Height);
	Gdiplus::Pen pn(m_clrFrame);
	grph.DrawRectangle(&pn, rc);
	Gdiplus::SolidBrush text(m_clrText);
	grph.DrawString(m_strText.c_str(), m_strText.size(), m_pFont.get(),
		rc, m_pFormat.get(), &text);
}

void Widget::SetHwid( HWID hWid )
{
	m_hWid = hWid;
}

HWID Widget::GetHwid() const
{
	return m_hWid;
}

ULONG Widget::AddChild( Widget* pWid )
{
	std::vector<Widget*>::iterator it = 
		std::find(m_rgpChildren.begin(), m_rgpChildren.end(), pWid);
	if (it == m_rgpChildren.end())
	{
		m_rgpChildren.push_back(pWid);
	}
	return m_rgpChildren.size();
}

ULONG Widget::RemoveChild( Widget* pWid )
{
	std::vector<Widget*>::iterator it = 
		std::find(m_rgpChildren.begin(), m_rgpChildren.end(), pWid);
	if (it != m_rgpChildren.end())
	{
		m_rgpChildren.erase(it);
	}
	return m_rgpChildren.size();
}

void Widget::SetMyParent( Widget* pParent )
{
	m_pParent = pParent;
}

void Widget::EnableScrollBar( UINT uBarFlag, BOOL bEnable /*= TRUE*/ )
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->EnableScrollBar(this, uBarFlag, bEnable);
}

void Widget::SetScrollInfo( int nBar, LPCSCROLLINFO lpsi, BOOL redraw )
{

}

void Widget::GetScrollInfo( int nBar, LPSCROLLINFO lpsi )
{

}

void Widget::SetScrollBar( int nBar, ScrollBar* pScrollBar )
{
	if (SB_VERT == nBar)
	{
		m_pVScrollbar = pScrollBar;
	}
	else
	{
		m_pHScrollbar = pScrollBar;
	}
}

void Widget::SetText( const std::wstring& strText )
{
	m_strText = strText;
}

std::wstring Widget::GetText() const
{
	return m_strText;
}

void Widget::SetFormat( const SharedPtr<Gdiplus::StringFormat>& pFormat )
{
	m_pFormat = pFormat;
}

const SharedPtr<Gdiplus::StringFormat> Widget::GetFormat() const
{
	return m_pFormat;
}

void Widget::SetFont( const SharedPtr<Gdiplus::Font>& pFont )
{
	m_pFont = pFont;
}

const SharedPtr<Gdiplus::Font> Widget::GetFont() const
{
	return m_pFont;
}

void Widget::SetBkgnd( const Gdiplus::Color& clrBkgnd )
{
	m_clrBkgnd = clrBkgnd;
}

Gdiplus::Color Widget::GetBkgnd() const
{
	return m_clrBkgnd;
}

void Widget::SetFrameClr( const Gdiplus::Color& clrFrame )
{
	m_clrFrame = clrFrame;
}

Gdiplus::Color Widget::GetFrameClr() const
{
	return m_clrFrame;
}

void Widget::SetState( WORD wState )
{
	if (wState == m_wState)
	{
		return;
	}

	m_wState = wState;

	SendWidMessage(WM_UPDATEUISTATE, m_wState);
}

WORD Widget::GetState() const
{
	return m_wState;
}

void Widget::SetTextClr( const Gdiplus::Color& clrText )
{
	m_clrText = clrText;
}

Gdiplus::Color Widget::GetTextColor() const
{
	return m_clrText;
}

void Widget::InitFont()
{
	m_pFormat.reset(new Gdiplus::StringFormat);
	m_pFormat->SetAlignment(Gdiplus::StringAlignmentCenter);
	m_pFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	m_pFont.reset(new Gdiplus::Font(WID_FONT_STATIC, 
		WID_FSIZE_STATIC));
	m_clrBkgnd.SetFromCOLORREF(WID_BKGND_STATIC);
	m_clrFrame.SetFromCOLORREF(WID_FRAME_STATIC);
	m_clrText.SetFromCOLORREF(WID_TEXT_STATIC);
}

void Widget::ShowWid( WORD wShow )
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->ShowWid(this, wShow);
	if (IsShow())
	{
		InvalidWid();
	}
}

BOOL Widget::SendWidMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	LRESULT lResult = 0;
	return ProcessWidMessage(uMsg, wParam, lParam, lResult, 0);
}

BOOL Widget::PostWidMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	LRESULT lResult = 0;
	return ProcessWidMessage(uMsg, wParam, lParam, lResult, 0);
}

UINT_PTR Widget::SetWidTimer( UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc )
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->SetWidTimer(this, nIDEvent, uElapse, lpTimerFunc);
	return 1;
}

BOOL Widget::KillWidTimer( UINT_PTR uIDEvent )
{
	ASSERT(FALSE);
	return m_pDispatch->KillWidTimer(this, uIDEvent);
}

BOOL Widget::IsShow() const
{
	return (m_wShow != SW_HIDE);
}

void Widget::MyShowWid( WORD wShow )
{
	m_wShow = wShow;
	PostWidMessage(WM_SHOWWINDOW, m_wShow);
}

LRESULT Widget::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (wParam == MK_LBUTTON)
		return 0;
	SetState(WID_STATE_MOUSE);
	return 0;
}

LRESULT Widget::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetState(WID_STATE_STATIC);
	return 0;
}

LRESULT Widget::OnStateChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(m_wState)
	{
	case WID_STATE_MOUSE:
		{
			m_clrText.SetFromCOLORREF(WID_TEXT_MOUSE);
			m_clrBkgnd.SetFromCOLORREF(WID_BKGND_MOUSE);
			m_clrFrame.SetFromCOLORREF(WID_FRAME_MOUSE);
		}
		break;
	case WID_STATE_STATIC:
		{
			m_clrText.SetFromCOLORREF(WID_TEXT_STATIC);
			m_clrBkgnd.SetFromCOLORREF(WID_BKGND_STATIC);
			m_clrFrame.SetFromCOLORREF(WID_FRAME_STATIC);
		}
		break;
	case WID_STATE_PUSH:
		{
			m_clrText.SetFromCOLORREF(WID_TEXT_PUSH);
			m_clrBkgnd.SetFromCOLORREF(WID_BKGND_PUSH);
			m_clrFrame.SetFromCOLORREF(WID_FRAME_PUSH);
		}
		break;
	default:
		{
			m_clrText.SetFromCOLORREF(WID_TEXT_STATIC);
			m_clrBkgnd.SetFromCOLORREF(WID_BKGND_STATIC);
			m_clrFrame.SetFromCOLORREF(WID_FRAME_STATIC);
		}
	}
	InvalidWid();
	return 0;
}

Widget::operator HWID() const
{
	return m_hWid;
}

void Widget::SetCapture()
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->SetCapture(this);
}

void Widget::ReleaseCapture()
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->ReleaseCapture();
}

Gdiplus::SizeF Widget::GetVirtualSize() const
{
	return m_szVirtual;
}

void Widget::SetVirtualSize( const Gdiplus::SizeF& sz )
{
	m_szVirtual = sz;
}

ScrollBar* Widget::GetScrollBar( int nBar ) const
{
	if (SB_VERT == nBar)
		return m_pVScrollbar;
	return m_pHScrollbar;
}

LRESULT Widget::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetState(WID_STATE_PUSH);
	return 1;
}

LRESULT Widget::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetState(WID_STATE_MOUSE);
	return 1;
}

UINT Widget::GetSBFlag() const
{
	return m_uBarFlag;
}

void Widget::SetSBFlag( UINT uSBFlag )
{
	m_uBarFlag = uSBFlag;
}

BOOL Widget::ScrollWid( int XAmount, int YAmount )
{
	return TRUE;
}

LRESULT Widget::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

void Widget::EstimateVirtualSize()
{

}

LRESULT Widget::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//SetWidTimer(1111, 50, NULL);
	return 0;
}

LRESULT Widget::OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_wState == WID_STATE_STATIC)
	{
		SetState(WID_STATE_MOUSE);
	}
	if (m_wState == WID_STATE_MOUSE)
	{
		SetState(WID_STATE_PUSH);
	}
	if (m_wState == WID_STATE_PUSH)
	{
		SetState(WID_STATE_STATIC);
	}
	return 0;
}

ScrollBar::ScrollBar( int nBar )
: m_nBar(nBar)
, m_pScrollInfo(new SCROLLINFO)
, m_pArrow1(new Widget)
, m_pArrow2(new Widget)
, m_pSlider(new Slider(nBar))
, m_nSliderSize(20)
, m_nArrorSize(10)
, m_nSAMargin(1)
{
	memset(m_pScrollInfo.get(), 0, sizeof(SCROLLINFO));
	m_pScrollInfo->cbSize = sizeof(SCROLLINFO);
	m_pScrollInfo->nMax = 100;
	m_pScrollInfo->nMin = 0;
	m_ptLButtonDown.x = -1;
	m_ptLButtonDown.y = -1;
	m_pArrow1->SetText(L"A1");
	m_pArrow2->SetText(L"A2");
	m_pSlider->SetText(L"Sl");
}

ScrollBar::~ScrollBar()
{
}

int ScrollBar::GetBar() const
{
	return m_nBar;
}

void ScrollBar::SetBar( int nBar )
{
	m_nBar = nBar;
}

void ScrollBar::GetScrollInfo( SCROLLINFO* pScrollInfo ) const
{
	ASSERT(pScrollInfo != NULL);
	if (pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		memcpy(pScrollInfo, m_pScrollInfo.get(), sizeof(SCROLLINFO));
	}
}

void ScrollBar::SetScrollInfo( const SCROLLINFO* pScrollInfo )
{
	ASSERT(pScrollInfo != NULL);
	if (pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		memcpy(m_pScrollInfo.get(), pScrollInfo, sizeof(SCROLLINFO));
	}
}

LRESULT ScrollBar::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ASSERT(m_pDispatch != NULL);
	ASSERT(m_pArrow1 != NULL);
	ASSERT(m_pArrow2 != NULL);
	ASSERT(m_pSlider != NULL);
	Gdiplus::RectF rc;
	m_pArrow1->Create(rc, m_pDispatch, this);
	m_pArrow2->Create(rc, m_pDispatch, this);
	m_pSlider->Create(rc, m_pDispatch, this);
	return 1;
}

LRESULT ScrollBar::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Gdiplus::RectF rcWid;
	GetRect(rcWid);
	Gdiplus::RectF rc = rcWid;
	rc.X += m_nSAMargin;

	if (SB_VERT == m_nBar)
	{
		rc.Width -= m_nSAMargin;
		rc.Height = m_nArrorSize;
	}
	else
	{
		rc.Width = m_nArrorSize;
		rc.Height -= m_nSAMargin;
	}

	rc.Y += m_nSAMargin;

	Gdiplus::RectF rcArrow1 = rc;
	m_pArrow1->SetWidRect(rcArrow1);

	rc = rcWid;

	if (SB_VERT == m_nBar)
	{
		rc.X += m_nSAMargin;
		rc.Width -= m_nSAMargin;
		rc.Height = m_nArrorSize;
		rc.Y = rcWid.Y + rcWid.Height - m_nArrorSize - m_nSAMargin;
	}
	else
	{
		rc.X = rcWid.X + rcWid.Width - m_nArrorSize - m_nSAMargin;
		rc.Width = m_nArrorSize;
		rc.Height -= m_nSAMargin;
		rc.Y += m_nSAMargin;
	}

	Gdiplus::RectF rcArrow2 = rc;
	m_pArrow2->SetWidRect(rcArrow2);

	rc = rcWid;

	float fSliderSize = GetSliderSize();
	float fPos = 0.0;
	float fTotal = m_nBar == SB_VERT? rcWid.Height : rcWid.Width;//m_pScrollInfo->nMax - m_pScrollInfo->nMin;
	if (fTotal > 0.00)
	{
		fPos = m_pScrollInfo->nPos / fTotal * fTotal;
	}
	else
	{
		fPos = 0.00;
	}

	if (SB_VERT == m_nBar)
	{
		rc.X += m_nSAMargin;
		rc.Width -= m_nSAMargin;
		rc.Y += fPos + m_nArrorSize + m_nSAMargin;
		rc.Height = fSliderSize;
	}
	else
	{
		rc.X += fPos + m_nArrorSize + m_nSAMargin;
		rc.Width = fSliderSize;
		rc.Y += m_nSAMargin;
		rc.Height -= m_nSAMargin;
	}

	if (SB_VERT == m_nBar)
	{
		if (rc.Y < rcArrow1.Y + m_nArrorSize + m_nSAMargin)
		{
			rc.Y = rcArrow1.Y + rcArrow1.Height + m_nSAMargin;
			rc.Height = fSliderSize;
		}
		if (rc.Y + rc.Height > rcArrow2.Y - m_nSAMargin)
		{
			rc.Height = fSliderSize;
			rc.Y = rcArrow2.Y - m_nSAMargin - fSliderSize;
		}
	}
	else
	{
		if (rc.X < rcArrow1.X + m_nArrorSize + m_nSAMargin)
		{
			rc.X = rcArrow1.X + rcArrow1.Width + m_nSAMargin;
			rc.Width = fSliderSize;
		}
		if (rc.X + rc.Width > rcArrow2.X - m_nSAMargin)
		{
			rc.Width = fSliderSize;
			rc.X = rcArrow2.X - m_nSAMargin - fSliderSize;
		}
	}

	m_pSlider->SetWidRect(rc);

	return 1;
}

float ScrollBar::GetSliderSize() const
{
	ASSERT(m_pScrollInfo != NULL);
	if (GetParent() == NULL)
	{
		return 0.0;
	}

	float ftotal = m_pScrollInfo->nMax - m_pScrollInfo->nMin;
	if (ftotal < 0.00)
	{
		return 0.00;
	}

	Gdiplus::RectF rcWid;
	GetRect(rcWid);

	float fsildersize = 0.0;
	float fslidersizeMax = 0.0;

	float farrowsize = 2 * m_nArrorSize;
	Gdiplus::SizeF sz = GetParent()->GetVirtualSize();
	if (SB_VERT == m_nBar)
	{
		fslidersizeMax = rcWid.Height - farrowsize;
		if (sz.Height > 0.00)
			fsildersize = fslidersizeMax - (fslidersizeMax * rcWid.Height / sz.Height);
		else
			fsildersize = 0.0;
	}
	else
	{
		fslidersizeMax = rcWid.Width - farrowsize;
		if (sz.Width > 0.00)
			fsildersize = fslidersizeMax - (fslidersizeMax * rcWid.Width / sz.Width);
		else
			fsildersize = 0.0;
	}
	if (fsildersize < MIN_SCROLLBAR_SLIDER)
		fsildersize = MIN_SCROLLBAR_SLIDER;
	return fsildersize;
}

void ScrollBar::SetPos( int nPos )
{
	ASSERT(m_pScrollInfo != NULL);
	Gdiplus::RectF rcWid;
	GetRect(rcWid);
	if (m_pScrollInfo->nPos != nPos)
	{
		m_pScrollInfo->nPos = nPos;
		if (m_pScrollInfo->nPos < 0)
			m_pScrollInfo->nPos = 0;
		if (SB_VERT == m_nBar)
		{
			if (m_pScrollInfo->nPos > rcWid.Height)
				m_pScrollInfo->nPos = rcWid.Height;
		}
		else
		{
			if (m_pScrollInfo->nPos > rcWid.Width)
				m_pScrollInfo->nPos = rcWid.Width;
		}
		SendWidMessage(WM_SIZE);
		InvalidWid();
	}
}

int ScrollBar::GetPos() const
{
	ASSERT(m_pScrollInfo != NULL);
	return m_pScrollInfo->nPos;
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
	rcc.right = rcc.left + (LONG)rc.Width;
	rcc.bottom = rcc.top + (LONG)rc.Height;
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

SharedPtr<Dispatcher> LayoutBase::Parse( const std::wstring& strXml )
{
	SharedPtr<Dispatcher> p;
	return p;
}

Slider::Slider( int nBar )
: m_nBar(nBar)
, m_bInSlider(FALSE)
, m_bLButtonDown(FALSE)
{
	memset(&m_ptLButtonDown, 0, sizeof(POINT));
}

Slider::~Slider()
{

}

LRESULT Slider::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bLButtonDown = TRUE;
	m_ptLButtonDown.x = GET_X_LPARAM(lParam);
	m_ptLButtonDown.y = GET_Y_LPARAM(lParam);
	return 0;
}

LRESULT Slider::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bLButtonDown = FALSE;
	m_ptLButtonDown.x = -1;
	m_ptLButtonDown.y = -1;
	return 0;
}

LRESULT Slider::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int nOffset = 0.00;
	POINT pt = {0};
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	Gdiplus::RectF rc;
	GetParent()->GetRect(rc);
	if (m_bLButtonDown)
	{
		if (SB_VERT == m_nBar)
		{
			if (pt.y < rc.Y || pt.y > rc.Y + rc.Height)
				return 0;
			nOffset = pt.y - m_ptLButtonDown.y;
		}
		else
		{
			if (pt.x < rc.X || pt.x > rc.X + rc.Width)
				return 0;
			nOffset = pt.x - m_ptLButtonDown.x;
		}
		ScrollBar* pParent = dynamic_cast<ScrollBar*>(GetParent());
		if (pParent != NULL)
		{
			pParent->SetPos(pParent->GetPos() + nOffset);
		}
		m_ptLButtonDown = pt;
	}
	else
	{
		__super::OnMouseMove(uMsg, wParam, lParam, bHandled);
	}
	return 0;
}

TimerManager::TimerManager( Dispatcher* pDispatch )
: m_pDispatch(pDispatch)
{

}


UINT_PTR TimerManager::SetWidTimer( Widget* pWid,
								   UINT_PTR nIDEvent, UINT uElapse, 
								   TIMERPROC lpTimerFunc )
{
	ASSERT(m_pDispatch != NULL);
	ASSERT(pWid != NULL);
	m_Timer.insert(std::make_pair(nIDEvent, std::make_pair(pWid->GetHwid(), pWid)));
	return ::SetTimer(m_pDispatch->GetHwnd(), nIDEvent, uElapse, lpTimerFunc);
}

BOOL TimerManager::KillWidTimer( Widget* pWid, UINT_PTR uIDEvent )
{
	std::map<UINT_PTR, std::pair<HWID, Widget*> >::iterator it =
		m_Timer.find(uIDEvent);
	if (it != m_Timer.end())
	{
		m_Timer.erase(it);
	}
	return ::KillTimer(m_pDispatch->GetHwnd(), uIDEvent);
}

Widget* TimerManager::GetWidgetFromTimer( UINT_PTR uIDEvent )
{
	std::map<UINT_PTR, std::pair<HWID, Widget*> >::iterator it =
		m_Timer.find(uIDEvent);
	if (it != m_Timer.end())
	{
		ASSERT(it->second.second != NULL);
		return it->second.second;
	}
	return NULL;
}

void TimerManager::Destroy( Widget* pWid )
{

}

TimerManager::~TimerManager()
{

}
