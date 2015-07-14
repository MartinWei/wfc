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

WfxRender::MemDC::MemDC( HDC hdc, const RECT& rc )
: m_hdc(hdc)
, m_rc(rc)
{
	m_hdcMem = ::CreateCompatibleDC(m_hdc);
	m_lWidth = m_rc.right - m_rc.left;
	m_lHeight = m_rc.bottom - m_rc.top;
	m_hBitmap = ::CreateCompatibleBitmap(m_hdc,
		m_lWidth, m_lHeight);
	m_hOldBitmap = ::SelectObject(m_hdcMem, m_hBitmap);
	//::SetWindowOrgEx(m_hdcMem, m_rc.left, m_rc.top, &m_ptOrg);
}

WfxRender::MemDC::~MemDC()
{
	BitBlt(m_hdc, m_rc.left, m_rc.top, m_lWidth, m_lHeight,
		m_hdcMem, m_rc.left, m_rc.top, SRCCOPY);
	::SelectObject(m_hdcMem, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hdcMem);
}

WfxRender::MemDC::operator HDC()
{
	return m_hdcMem;
}

GdiPlusHelper::GdiPlusHelper()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_nGdiPlusToken, &gdiplusStartupInput, NULL);
}

GdiPlusHelper::~GdiPlusHelper()
{
	Gdiplus::GdiplusShutdown(m_nGdiPlusToken);
}

BOOL WfxRender::GetRoundRect( const RECT& rc, 
								 Gdiplus::GraphicsPath& path, float r /*= 10.0*/ )
{
	return FALSE;
	/*if (rc.Width <= 0 || rc.Height <= 0)
		return FALSE;

	path.Reset();

	Gdiplus::PointF pt1;
	Gdiplus::PointF pt2;
	RECT rcArc(rc);
	rcArc.Height = r;
	rcArc.Width = r;
	pt1.X = rc.X;
	pt1.Y = rc.Y + rc.Height - r/2 ;
	pt2.X = pt1.X;
	pt2.Y = rc.Y + r;
	path.AddLine(pt1, pt2);
	path.AddArc(rcArc, 180.0, 90.0);

	pt1.X = rc.X + r;
	pt1.Y = rc.Y;
	pt2.X = rc.X + rc.Width - r;
	pt2.Y = pt1.Y;
	path.AddLine(pt1, pt2);
	rcArc.X = pt2.X;
	rcArc.Y = pt2.Y;
	path.AddArc(rcArc, 270.0, 90.0);

	pt1.X = rc.X + rc.Width;
	pt1.Y = rc.Y + r;
	pt2.X = pt1.X;
	pt2.Y = rc.Y + rc.Height - r;
	path.AddLine(pt1, pt2);

	rcArc.X = rc.X + rc.Width - r;
	rcArc.Y = pt2.Y;
	path.AddArc(rcArc, 0.0, 90.0);

	pt1.X = rc.X + rc.Width - r;
	pt1.Y = rc.Y + rc.Height;
	pt2.X = rc.X + r;
	pt2.Y = pt1.Y;
	path.AddLine(pt1, pt2);

	rcArc.X = rc.X;
	rcArc.Y = rc.Y + rc.Height - r;
	path.AddArc(rcArc, 90.0, 90);

	return TRUE;*/
}

void WfxRender::DrawButtton( HDC hdc, const std::wstring& strText, const RECT& rc, WORD wState, WidDispatch* pDispatch )
{
	COLORREF clrBk = WBTN_BKGND_STATIC;
	COLORREF clrFrm = WBTN_FRAME_STATIC;
	switch(wState)
	{
	case WID_STATE_STATIC:
		clrBk = WID_BKGND_STATIC;
		clrFrm = WBTN_FRAME_STATIC;
		break;
	case WID_STATE_MOUSE:
		clrBk = WBTN_BKGND_MOUSE;
		clrFrm = WBTN_FRAME_MOUSE;
		break;
	case WID_STATE_PUSH:
		clrBk = WBTN_BKGND_PUSH;
		clrFrm = WBTN_FRAME_PUSH;
		break;
	case WID_STATE_CHECKED:
		clrBk = WBTN_BKGND_CHECKED;
		clrFrm = WBTN_FRAME_CHECKED;
		break;
	}
	DrawSolidRect(hdc, rc, clrBk, pDispatch);
	DrawFrame(hdc, rc, clrFrm, pDispatch);
	DrawText(hdc, rc, strText, clrFrm, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}


void WfxRender::DrawRadioBox( HDC hdc, const RECT& rc, WORD wState, WidDispatch* pDispatch /*= NULL*/ )
{

}

void WfxRender::DrawWidget( HDC hdc, const std::wstring& strText, const RECT& rc, WORD wState , WidDispatch* pDispatch)
{
	DrawSolidRect(hdc, rc, WID_BKGND_STATIC, pDispatch);
	DrawFrame(hdc, rc, WID_FRAME_STATIC, pDispatch);
}

void WfxRender::DrawSolidRect( HDC hdc, const RECT& rcPaint, COLORREF clr ,WidDispatch* pDispatch)
{
	ASSERT(::GetObjectType(hdc)==OBJ_DC || ::GetObjectType(hdc)==OBJ_MEMDC);
	::SetBkColor(hdc, clr);
	::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcPaint, NULL, 0, NULL);
}

void WfxRender::DrawFrame( HDC hdc, const RECT& rcPaint, COLORREF clr, WidDispatch* pDispatch )
{
	HBRUSH brsh = ::CreateSolidBrush(clr);
	::FrameRect(hdc, &rcPaint, brsh);
	::DeleteObject(brsh);
}

void WfxRender::DrawText( HDC hdc, const RECT& rcPaint, const std::wstring& strText, COLORREF clr, DWORD dwFormat, HFONT hFont /*= NULL*/, WidDispatch* pDispatch /*= NULL*/ )
{
	ASSERT(::GetObjectType(hdc)==OBJ_DC || ::GetObjectType(hdc)==OBJ_MEMDC);
	::SetBkMode(hdc, TRANSPARENT);
	::SetTextColor(hdc, clr);
	if (hFont != NULL)
		::SelectObject(hdc, hFont);
	::DrawTextW(hdc, strText.c_str(), strText.size(), (LPRECT)&rcPaint, dwFormat);
}

void WfxRender::DrawCheckBox( HDC hdc, const RECT& rc, WORD wState, WidDispatch* pDispatch /*= NULL*/ )
{

}

void WfxRender::DrawSlider( HDC hdc, const RECT& rc, WORD wState, WidDispatch* pDispatch /*= NULL*/ )
{

}

void WfxRender::DrawArror( HDC hdc, const RECT& rc, WORD wState, WidDispatch* pDispatch /*= NULL*/ )
{

}

void WfxRender::GenerateClip( HDC hDC, const RECT& rcItem, RenderClip& clip )
{
	RECT rcClip = { 0 };
	::GetClipBox(hDC, &rcClip);
	clip.m_hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	clip.m_hRgn = ::CreateRectRgnIndirect(&rcItem);
	::ExtSelectClipRgn(hDC, clip.m_hRgn, RGN_AND);
	clip.m_hDC = hDC;
	clip.m_rcItem = rcItem;
}

void WfxRender::DrawHeadCell( HDC hdc, const RECT& rcPaint, DWORD dwState, const std::wstring& strText, COLORREF clrText, DWORD dwFormat )
{
	COLORREF clrBk = WCELL_BKGRND;
	if (dwState & WCS_SELECTED)
		clrBk = WCELL_SELECTED;
	std::wstring strDraw = strText;
	if (dwState & WCS_ASORT)
	{
		strDraw += L"^";
		clrText = RGB(0, 255, 0);
	}
	else if (dwState & WCS_NASORT)
	{
		strDraw += L"v";
		clrText = RGB(255, 0, 0);
	}
	WfxRender::DrawSolidRect(hdc, rcPaint, clrBk, NULL);
	WfxRender::DrawFrame(hdc, rcPaint, WCELL_FRAME, NULL);
	WfxRender::DrawText(hdc, rcPaint, strDraw, clrText, dwFormat);
}


BEGIN_NAMESPACE_WFX

GdiPlusHelper gdiplushelper;

END_NAMESPACE_WFX

WfxRender::RenderClip::~RenderClip()
{
	ASSERT(::GetObjectType(m_hDC)==OBJ_DC || ::GetObjectType(m_hDC)==OBJ_MEMDC);
	ASSERT(::GetObjectType(m_hRgn)==OBJ_REGION);
	ASSERT(::GetObjectType(m_hOldRgn)==OBJ_REGION);
	::SelectClipRgn(m_hDC, m_hOldRgn);
	::DeleteObject(m_hOldRgn);
	::DeleteObject(m_hRgn);
}

WfxRender::RenderClip::RenderClip(HDC hdc, const RECT& rcItem)
: m_hDC(hdc)
{
	RECT rcClip = { 0 };
	::GetClipBox(m_hDC, &rcClip);
	m_hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	m_hRgn = ::CreateRectRgnIndirect(&rcItem);
	::ExtSelectClipRgn(m_hDC, m_hRgn, RGN_AND);
	m_rcItem = rcItem;
}
