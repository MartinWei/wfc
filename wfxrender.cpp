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

WfxRender::MemDC::MemDC( HWND hWnd, const RECT& rcPaint )
: m_hWnd(hWnd)
, m_rcPaint(rcPaint)
, m_hdc(NULL)
, m_hdcMem(NULL)
, m_hBitmap(NULL)
, m_hOldBitmap(NULL)
{
	m_hdc = ::GetDC(m_hWnd);
	if (m_hdc != NULL)
	{
		m_hdcMem = ::CreateCompatibleDC(m_hdc);
		RECT rcClient = {0};
		::GetClientRect(m_hWnd, &rcClient);
		m_hBitmap = ::CreateCompatibleBitmap(m_hdc,
			rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
		if (m_hdcMem != NULL && m_hBitmap != NULL)
			m_hOldBitmap = ::SelectObject(m_hdcMem, m_hBitmap);
	}
}

WfxRender::MemDC::~MemDC()
{
	if (m_hdc != NULL && m_hdcMem != NULL
		&& m_hBitmap != NULL)
	{
		::BitBlt(m_hdc, m_rcPaint.left, m_rcPaint.top, 
			m_rcPaint.right - m_rcPaint.left, 
			m_rcPaint.bottom - m_rcPaint.top,
			m_hdcMem, m_rcPaint.left, m_rcPaint.top, SRCCOPY);
		::SelectObject(m_hdcMem, m_hOldBitmap);
		::DeleteObject(m_hBitmap);
		::DeleteDC(m_hdcMem);
		::ReleaseDC(m_hWnd, m_hdc);
	}
	
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
	DrawText(hdc, rc, strText, RGB(255, 0, 0), DT_VCENTER | DT_SINGLELINE | DT_CENTER, NULL, pDispatch);
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
	DrawSolidRect(hdc, rc, RGB(60, 60, 60), pDispatch);
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

SIZE WfxRender::EstimateWidgetSize( const RECT& rc, const std::wstring& strText, WORD wState, WidDispatch* pDispatch /*= NULL*/ )
{
	SIZE sz = {0};
	sz.cx = rc.right - rc.left;
	sz.cy = rc.bottom - rc.top;
	if (pDispatch != NULL)
	{
		HWND hWnd = pDispatch->GetHwnd();
		if (hWnd != NULL && ::IsWindow(hWnd))
		{
			HDC hdc = ::GetDC(hWnd);
			if (hdc != NULL)
			{
				::GetTextExtentPoint32(hdc, strText.c_str(), strText.size(), &sz);
			}
			::ReleaseDC(hWnd, hdc);
		}
	}
	return sz;
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
