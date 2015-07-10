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
#include "wfxgdi.h"

USING_NAMESPACE_WFX;

MemDC::MemDC( HDC hdc, const RECT& rc )
: m_hdc(hdc)
, m_rc(rc)
{
	m_hdcMem = ::CreateCompatibleDC(m_hdc);
	m_lWidth = m_rc.right - m_rc.left;
	m_lHeight = m_rc.bottom - m_rc.top;
	m_hBitmap = ::CreateCompatibleBitmap(m_hdc,
		m_lWidth, m_lHeight);
	m_hOldBitmap = ::SelectObject(m_hdcMem, m_hBitmap);
	::SetWindowOrgEx(m_hdcMem, m_rc.left, m_rc.top, &m_ptOrg);
}

MemDC::~MemDC()
{
	BitBlt(m_hdc, m_rc.left, m_rc.top, m_lWidth, m_lHeight,
		m_hdcMem, m_rc.left, m_rc.top, SRCCOPY);
	::SelectObject(m_hdcMem, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hdcMem);
}

MemDC::operator HDC()
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

BOOL CommonDraw::GetRoundRect( const Gdiplus::RectF& rc, 
								 Gdiplus::GraphicsPath& path, float r /*= 10.0*/ )
{
	if (rc.Width <= 0 || rc.Height <= 0)
		return FALSE;

	path.Reset();

	Gdiplus::PointF pt1;
	Gdiplus::PointF pt2;
	Gdiplus::RectF rcArc(rc);
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

	return TRUE;
}

void CommonDraw::DrawButtton( WORD wState, const Gdiplus::RectF& rc, Gdiplus::Graphics& grph, WfxImagePtr pImage /*= ImagePtr()*/ )
{

}

void CommonDraw::DrawCheckBox( BOOL bCheck, const Gdiplus::RectF& rc, Gdiplus::Graphics& grph, WfxImagePtr pImage /*= ImagePtr()*/ )
{

}

void CommonDraw::DrawRadioBox( BOOL bCheck, const Gdiplus::RectF& rc, Gdiplus::Graphics& grph, WfxImagePtr pImage /*= ImagePtr()*/ )
{

}


BEGIN_NAMESPACE_WFX

GdiPlusHelper gdiplushelper;

END_NAMESPACE_WFX
