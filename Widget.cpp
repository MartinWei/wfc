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
#include "wfxrender.h"

USING_NAMESPACE_WFX;

LRESULT WidgetBase::SendWidMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;
	ProcessMessage(uMsg, wParam, lParam, lResult, 0);
	return lResult;
}

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
, m_clrBkgnd(WID_BKGND_STATIC)
, m_clrFrame(WID_FRAME_STATIC)
, m_clrText(WID_TEXT_STATIC)
, m_nHorzPosOffset(0)
, m_nVertPosOffset(0)
, m_bVirtualSizeValid(FALSE)
, m_nID(0)
{
	m_pFont.reset(new LOGFONTW);
	memset(&m_rcWid, 0, sizeof(RECT));
	memset(&m_rcDraw, 0, sizeof(RECT));
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

RECT Widget::GetRect() const 
{
	return m_rcWid;
}

void Widget::SetRect( const RECT& rc )
{
	m_rcWid = rc;
	m_pDispatch->SetWidRect(this, rc);
}

RECT Widget::GetParentRect() const
{
	RECT rcParent = {0};
	if (m_pParent != NULL)
	{
		rcParent = m_pParent->GetRect();
	}
	else
	{
		ASSERT(m_pDispatch != NULL);
		ASSERT(::IsWindow(m_pDispatch->GetHwnd()));
		::GetClientRect(m_pDispatch->GetHwnd(), &rcParent);
	}
	return rcParent;
}

BOOL Widget::Create( const RECT& rc, WidDispatch* pDispatch, 
					Widget* pParent /*= NULL*/, BOOL bNC /*= FALSE*/ )
{
	ASSERT(pDispatch != NULL);
	m_pDispatch = pDispatch;
	m_pDispatch->Create(this);
	m_bNC = bNC;
	SetParent(pParent);
	SendWidMessage(WM_CREATE, 0, 0);
	SetRect(rc);
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
	m_pDispatch->Invalidate(GetRect());
}

void Widget::OnDraw( HDC hdc, const RECT& rcPaint )
{
	RECT rc = m_rcWid;
	rc.left = m_rcWid.left - m_nHorzPosOffset;
	rc.top = m_rcWid.top - m_nVertPosOffset;
	WfxRender::DrawSolidRect(hdc, m_rcWid, WID_BKGND_STATIC, m_pDispatch);
	WfxRender::DrawText(hdc, rc, m_strText, RGB(255, 0, 0), DT_VCENTER | DT_SINGLELINE | DT_LEFT, NULL, m_pDispatch);
	WfxRender::DrawFrame(hdc, m_rcWid, WBTN_BKGND_MOUSE, m_pDispatch);
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

void Widget::SetText( const String& strText )
{
	m_strText = strText;
}

String Widget::GetText() const
{
	return m_strText;
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

void Widget::ShowWid( WORD wShow )
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->ShowWid(this, wShow);
	if (IsShow())
	{
		InvalidWid();
	}
}

BOOL Widget::PostWidMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	LRESULT lResult = 0;
	return ProcessMessage(uMsg, wParam, lParam, lResult, 0);
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
	switch(wParam)
	{
	case WID_STATE_MOUSE:
		{
		
		}
		break;
	case WID_STATE_STATIC:
		{
			
		}
		break;
	case WID_STATE_PUSH:
		{
			
		}
		break;
	default:
		{
			
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
	return 0;
}

LRESULT Widget::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
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

LRESULT Widget::OnKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	InvalidWid();
	return 0;
}

LRESULT Widget::OnQueryVirtualSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	/*if (m_bVirtualSizeValid)
	{
		return MAKELRESULT(m_szVirtual.cx, m_szVirtual.cy);
	}*/
	m_szVirtual = EstimateVirualSize();
	m_bVirtualSizeValid = TRUE;
	return MAKELRESULT(m_szVirtual.cx, m_szVirtual.cy);
}

LRESULT Widget::OnGetVirtualSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return MAKELRESULT(m_szVirtual.cx, m_szVirtual.cy);
}

void Widget::SetVirtualSizeValid( BOOL bValid /*= TRUE*/ )
{
	m_bVirtualSizeValid = bValid;
}

SIZE Widget::EstimateVirualSize()
{
	return WfxRender::EstimateWidgetSize(m_rcWid, 
		m_strText, m_wState, m_pDispatch);
}

LRESULT Widget::OnQueryVisualSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return MAKELRESULT(m_rcWid.right - m_rcWid.left, m_rcWid.bottom - m_rcWid.top);
}

LRESULT Widget::OnScrollBarOffset( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ScrollBar* pScrollBar = GetScrollBar(wParam);
	if (pScrollBar == NULL)
	{
		return FALSE;
	}

	SharedPtr<SCROLLINFO> psinfo(new SCROLLINFO);
	psinfo->cbSize = sizeof(SCROLLINFO);
	pScrollBar->GetScrollInfo(psinfo.get());

	TRACE(L"nPos = %d", psinfo->nPos);
	float fOffset = 0;
	RECT rcDraw = GetDrawRect();
	if (wParam == SB_HORZ)
	{
		fOffset = (rcDraw.right - rcDraw.left) * (float)psinfo->nPos / (float)(psinfo->nMax - psinfo->nMin);
		fOffset *= (float)m_szVirtual.cx / (float)(m_rcDraw.right - m_rcDraw.left);
		SetHOffset(fOffset);
	}
	else
	{
		fOffset = (rcDraw.bottom - rcDraw.top) * (float)psinfo->nPos / (float)(psinfo->nMax - psinfo->nMin);
		fOffset *= (float)m_szVirtual.cy / (float)(m_rcDraw.bottom - m_rcDraw.top);
		SetVOffset(fOffset);
	}
	return 1;
}

LRESULT Widget::OnHScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	InvalidWid();
	return 1;
}

HFONT Widget::GetFontObject() const
{
	return WfxRender::GetFontObject();
}

LRESULT Widget::SendParentMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	if (m_pParent != NULL)
		return m_pParent->SendWidMessage(uMsg, wParam, lParam);
	return 0;
}

RECT Widget::GetDrawRect() const
{
	return m_rcDraw;
}

void Widget::SetDrawRect( const RECT& rc )
{
	m_rcDraw = rc;
}

LONG Widget::GetVOffset() const
{
	return m_nVertPosOffset;
}

void Widget::SetVOffset( LONG nOffset )
{
	m_nVertPosOffset = nOffset;
}

LONG Widget::GetHOffset() const
{
	return m_nHorzPosOffset;
}

void Widget::SetHOffset( LONG nOffset )
{
	m_nHorzPosOffset = nOffset;
}

SIZE Widget::GetVirtualSize() const
{
	return m_szVirtual;
}

UINT Widget::GetID() const
{
	return m_nID;
}

void Widget::SetID( UINT nID )
{
	m_nID = nID;
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
	m_pScrollInfo->nMax = 10000;
	m_pScrollInfo->nMin = 0;
	m_ptLButtonDown.x = -1;
	m_ptLButtonDown.y = -1;
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

void ScrollBar::SetRange( int nMax, int nMin )
{
	ASSERT(m_pScrollInfo != NULL);
	if (m_pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		m_pScrollInfo->nMax = nMax;
		m_pScrollInfo->nMin = nMin;
	}
}

LRESULT ScrollBar::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ASSERT(m_pDispatch != NULL);
	ASSERT(m_pArrow1 != NULL);
	ASSERT(m_pArrow2 != NULL);
	ASSERT(m_pSlider != NULL);
	RECT rc = {0};
	m_pArrow1->Create(rc, m_pDispatch, this);
	m_pArrow2->Create(rc, m_pDispatch, this);
	m_pSlider->Create(rc, m_pDispatch, this);
	return 1;
}

LRESULT ScrollBar::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	RECT rcWid = {0};
	rcWid = GetRect();
	RECT rc = rcWid;
	rc.left += m_nSAMargin;

	if (SB_VERT == m_nBar)
	{
		rc.right -= m_nSAMargin;
		rc.bottom = rc.top + m_nArrorSize;
	}
	else
	{
		rc.right = rc.left + m_nArrorSize;
		rc.bottom -= m_nSAMargin;
	}

	rc.top += m_nSAMargin;

	RECT rcArrow1 = rc;
	m_pArrow1->SetRect(rcArrow1);

	rc = rcWid;

	if (SB_VERT == m_nBar)
	{
		rc.left += m_nSAMargin;
		rc.right -= m_nSAMargin;
		rc.bottom = rcWid.bottom - m_nSAMargin;
		rc.top = rc.bottom - m_nArrorSize;
	}
	else
	{
		rc.left = rcWid.right - m_nArrorSize - m_nSAMargin;
		rc.right = rc.left + m_nArrorSize;
		rc.bottom -= m_nSAMargin;
		rc.top += m_nSAMargin;
	}

	RECT rcArrow2 = rc;
	m_pArrow2->SetRect(rcArrow2);

	rc = rcWid;

	float fSliderSize = CalcSliderSize();
	
	RECT rcSliserMax = rcWid;
	
	if (SB_HORZ == m_nBar)
	{
		rcSliserMax.left += m_nArrorSize + 2 * m_nSAMargin;
		rcSliserMax.right -= m_nArrorSize + 2 * m_nSAMargin;
		rcSliserMax.top += m_nSAMargin;
		rcSliserMax.bottom -= m_nSAMargin;
	}
	else
	{
		rcSliserMax.top += m_nArrorSize + 2 * m_nSAMargin;
		rcSliserMax.bottom -= m_nArrorSize + 2 * m_nSAMargin;
		rcSliserMax.left += m_nSAMargin;
		rcSliserMax.right -= m_nSAMargin;
	}
	rc = CalcSliderRect(rcSliserMax);

	m_pSlider->SetRect(rc);

	return 1;
}

LRESULT ScrollBar::OnSliderOffset( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int nOffset = (int)wParam;
	RECT rcSlider = m_pSlider->GetRect();
	int nMin = GetSliderMin();
	int nMax = GetSliderMax();
	if (m_nBar == SB_HORZ)
	{
		rcSlider.left += nOffset;
		rcSlider.right += nOffset;
		if (rcSlider.left < nMin)
		{
			rcSlider.left = nMin;
			rcSlider.right = rcSlider.left + m_nSliderSize;
		}
		if (rcSlider.right > nMax)
		{
			rcSlider.right = nMax;
			rcSlider.left = rcSlider.right - m_nSliderSize;
		}
	}
	else
	{
		rcSlider.top += nOffset;
		rcSlider.bottom += nOffset;
		if (rcSlider.top < nMin)
		{
			rcSlider.top = nMin;
			rcSlider.bottom = rcSlider.top + m_nSliderSize;
		}
		if (rcSlider.bottom > nMax)
		{
			rcSlider.bottom = nMax;
			rcSlider.top = rcSlider.bottom - m_nSliderSize;
		}
	}
	m_pSlider->SetRect(rcSlider);
	SetPos(CalcSliderPos(rcSlider));
	return 1;
}

float ScrollBar::CalcSliderSize()
{
	ASSERT(m_pScrollInfo != NULL);
	if (GetParent() == NULL)
	{
		return 0.0;
	}

	RECT rcWid = GetRect(); 
	LRESULT lResult = SendParentMessage(WUM_GET_VIRTUAL_SIZE);
	SIZE szVirtual = {LOWORD(lResult), HIWORD(lResult)};

	lResult = SendParentMessage(WUM_GET_VISUAL_SIZE);
	SIZE szVisual = {LOWORD(lResult), HIWORD(lResult)};

	LONG nOffset = 2 * m_nArrorSize - 2 * m_nSAMargin;
	LONG nVirtualSize = SB_VERT == m_nBar? szVirtual.cy : szVirtual.cx;	
	LONG nVisualSize = SB_VERT == m_nBar? szVisual.cy : szVisual.cx;
	LONG nScrollBarSize = SB_VERT == m_nBar? rcWid.bottom - rcWid.top - nOffset
		: rcWid.right - rcWid.left - nOffset;

	if (nVirtualSize > 0)
		m_nSliderSize = nScrollBarSize * nVisualSize / nVirtualSize;
	else
		m_nSliderSize = 0;

	if (m_nSliderSize < MIN_SCROLLBAR_SLIDER)
		m_nSliderSize = MIN_SCROLLBAR_SLIDER;

	return m_nSliderSize;
}

void ScrollBar::SetPos( int nPos )
{
	ASSERT(m_pScrollInfo != NULL);
	BOOL bFurther = nPos > m_pScrollInfo->nPos;
	if (m_pScrollInfo->nPos != nPos)
	{
		m_pScrollInfo->nPos = nPos;
		if (m_pScrollInfo->nPos < m_pScrollInfo->nMin)
			m_pScrollInfo->nPos = m_pScrollInfo->nMin;
		if (m_pScrollInfo->nPos > m_pScrollInfo->nMax)
			m_pScrollInfo->nPos = m_pScrollInfo->nMax;
		SendParentMessage(WUM_SB_OFFSET, m_nBar);
		if (m_nBar == SB_VERT) 
		{
			SendParentMessage(WM_VSCROLL, (WPARAM)bFurther);
		}
		else
		{
			SendParentMessage(WM_HSCROLL,(WPARAM)bFurther);
		}
	}
}

int ScrollBar::GetPos() const
{
	ASSERT(m_pScrollInfo != NULL);
	return m_pScrollInfo->nPos;
}

int ScrollBar::CalcSliderPos( const RECT& rcSlider )
{
	int nMaxSlider = GetSliderMax() - m_nSliderSize / 2;
	int nMinSlider = GetSliderMin() + m_nSliderSize / 2;;
	int nMidSlider = GetSlierMid();
	if (nMaxSlider > 0)
		return  (m_pScrollInfo->nMax - m_pScrollInfo->nMin) * (nMidSlider - nMinSlider) / (nMaxSlider - nMinSlider);
	return 0;
}

RECT ScrollBar::CalcSliderRect(const RECT& rcMaxSlider)
{
	RECT rcSlider = rcMaxSlider;
	int nMid = 0;
	if (m_nBar == SB_HORZ)
	{
		nMid = rcMaxSlider.right * (float)m_pScrollInfo->nPos / (float)(m_pScrollInfo->nMax - m_pScrollInfo->nMin);
		rcSlider.left = nMid - (float)m_nSliderSize / (float)2;
		rcSlider.right = rcSlider.left + m_nSliderSize;
		if (rcSlider.left < rcMaxSlider.left)
		{
			rcSlider.left = rcMaxSlider.left;
			rcSlider.right = rcSlider.left + m_nSliderSize;
		}
		if (rcSlider.right > rcMaxSlider.right)
		{
			rcSlider.right = rcMaxSlider.right;
			rcSlider.left = rcSlider.right - m_nSliderSize;
		}
	}
	else
	{
		nMid = rcMaxSlider.bottom * (float)m_pScrollInfo->nPos / (float)(m_pScrollInfo->nMax - m_pScrollInfo->nMin);
		rcSlider.top = nMid - (float)m_nSliderSize / (float)2;
		rcSlider.bottom = rcSlider.top + m_nSliderSize;
		if (rcSlider.top < rcMaxSlider.top)
		{
			rcSlider.top = rcMaxSlider.top;
			rcSlider.bottom = rcSlider.top + m_nSliderSize;
		}
		if (rcSlider.bottom > rcMaxSlider.bottom)
		{
			rcSlider.bottom = rcMaxSlider.bottom;
			rcSlider.top = rcSlider.right - m_nSliderSize;
		}
	}
	
	return rcSlider;
}

int ScrollBar::GetSliderMax()
{
	RECT rcWid = GetRect();
	if (m_nBar == SB_HORZ)
		return rcWid.right - m_nArrorSize - m_nSAMargin;
	else
		return rcWid.bottom - m_nArrorSize - m_nSAMargin;
}

int ScrollBar::GetSliderMin()
{
	RECT rcWid = GetRect();
	if (m_nBar == SB_HORZ)
		return rcWid.left + m_nArrorSize + m_nSAMargin;
	else
		return rcWid.top + m_nArrorSize + m_nSAMargin;
}

int ScrollBar::GetSlierMid()
{
	RECT rcSlider = m_pSlider->GetRect();
	if (m_nBar == SB_HORZ)
		return rcSlider.left + m_nSliderSize / 2;
	else
		return rcSlider.top + m_nSliderSize / 2;
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

	if (m_bLButtonDown)
	{
		RECT rc = GetParentRect();
		if (SB_VERT == m_nBar)
		{
			if (pt.y < rc.top || pt.y > rc.bottom)
				return 0;
			nOffset = pt.y - m_ptLButtonDown.y;
		}
		else
		{
			if (pt.x < rc.left || pt.x > rc.right)
				return 0;
			nOffset = pt.x - m_ptLButtonDown.x;
		}
		SendParentMessage(WUM_SB_OFFSET, (WPARAM)nOffset);
		m_ptLButtonDown = pt;
	}
	else
	{
		__super::OnMouseMove(uMsg, wParam, lParam, bHandled);
	}
	return 0;
}

void Slider::OnDraw( HDC hdc, const RECT& rcPaint )
{
	WfxRender::DrawSlider(hdc, GetRect(), GetState(), m_pDispatch);
}

Timer::Timer( WidDispatch* pDispatch )
: m_pDispatch(pDispatch)
, m_nTimerID(0x1000)
{

}


UINT_PTR Timer::SetWidTimer( Widget* pWid,
								   UINT_PTR nIDEvent, UINT uElapse, 
								   TIMERPROC lpTimerFunc )
{
	ASSERT(m_pDispatch != NULL);
	ASSERT(pWid != NULL);
	ASSERT(uElapse > 0);
	PTimerInfo pTI(new TimerInfo);
	pTI->m_pSender.first = pWid->GetHwid();
	pTI->m_pSender.second = pWid;
	pTI->m_hWnd = m_pDispatch->GetHwnd();
	pTI->m_nRealTimer = GenerateTimerID();
	pTI->m_nSrcTimer = nIDEvent;
	m_rgpTimers.push_back(pTI);
	return ::SetTimer(m_pDispatch->GetHwnd(), pTI->m_nRealTimer, uElapse, lpTimerFunc);
}

BOOL Timer::KillWidTimer( Widget* pWid, UINT_PTR uIDEvent )
{
	ASSERT(m_pDispatch != NULL);
	ASSERT(pWid!=NULL);
	PTimerInfo pTimer;
	for( TimerIter it = m_rgpTimers.begin(); it != m_rgpTimers.end(); ++it ) 
	{
		pTimer = *it;
		if( pTimer->m_pSender.first == pWid->GetHwid()
			&& pTimer->m_pSender.second == pWid
			&& pTimer->m_hWnd == m_pDispatch->GetHwnd()
			&& pTimer->m_nSrcTimer == uIDEvent )
		{
			::KillTimer(pTimer->m_hWnd, pTimer->m_nRealTimer);
			m_rgpTimers.erase(it);
			return TRUE;
		}
	}
	return FALSE;
}

UINT_PTR Timer::GenerateTimerID()
{
	m_nTimerID = (++m_nTimerID) % 0xFF;
	return m_nTimerID;
}

Widget* Timer::GetWidgetFromTimer( UINT_PTR uIDEvent )
{
	PTimerInfo pTimer;
	for( TimerIter it = m_rgpTimers.begin(); it != m_rgpTimers.end(); ++it ) 
	{
		pTimer = *it;
		if(pTimer->m_hWnd == m_pDispatch->GetHwnd()
			&& pTimer->m_nRealTimer == uIDEvent )
		{
			return pTimer->m_pSender.second;
		}
	}
	return FALSE;
}


void Timer::Destroy( Widget* pWid )
{

}

Timer::~Timer()
{

}

ImageWid::ImageWid()
{

}

ImageWid::ImageWid(const String& strStatic,
				   const String& strMouse, 
				   const String& strPush,
				   const String& strChecked)
				   : m_pStatic(Gdiplus::Image::FromFile(strStatic.c_str()))
				   , m_pMouse(Gdiplus::Image::FromFile(strMouse.c_str()))
				   , m_pPush(Gdiplus::Image::FromFile(strPush.c_str()))
				   , m_pChecked(Gdiplus::Image::FromFile(strChecked.c_str()))
{
}

ImageWid::~ImageWid()
{

}

void ImageWid::SetImage( WORD wState, const String& strImage )
{
	switch(wState)
	{
	case WID_STATE_STATIC:
		m_pStatic.reset(Gdiplus::Image::FromFile(strImage.c_str()));
		break;
	case WID_STATE_MOUSE:
		m_pMouse.reset(Gdiplus::Image::FromFile(strImage.c_str()));
		break;
	case WID_STATE_PUSH:
		m_pPush.reset(Gdiplus::Image::FromFile(strImage.c_str()));
		break;
	case WID_STATE_CHECKED:
		m_pChecked.reset(Gdiplus::Image::FromFile(strImage.c_str()));
		break;
	default:
		ASSERT(FALSE);
	}
}

void ImageWid::SetImage( const String& strStatic, 
						const String& strMouse, 
						const String& strPush,
						const String& strChecked)
{
	m_pStatic.reset(Gdiplus::Image::FromFile(strStatic.c_str()));
	m_pMouse.reset(Gdiplus::Image::FromFile(strMouse.c_str()));
	m_pPush.reset(Gdiplus::Image::FromFile(strPush.c_str()));
	m_pChecked.reset(Gdiplus::Image::FromFile(strChecked.c_str()));
}

SharedPtr<Gdiplus::Image> ImageWid::GetImageFromState()
{
	SharedPtr<Gdiplus::Image> pImage;
	switch(GetState())
	{
	case WID_STATE_STATIC:
		pImage = m_pStatic;
		break;
	case WID_STATE_MOUSE:
		pImage = m_pMouse;
		break;
	case WID_STATE_PUSH:
		pImage = m_pPush;
		break;
	case WID_STATE_CHECKED:
		pImage = m_pChecked;
		break;
	default:
		ASSERT(FALSE);
		pImage = m_pStatic;
	}
	return pImage;
}

InPlaceWid::InPlaceWid()
: m_pWindow(NULL)
{

}

InPlaceWid::~InPlaceWid()
{

}

LRESULT InPlaceWid::OnSetFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (Initial())
	{
		return 0;
	}
	return 1;
}
