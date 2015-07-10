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
#pragma once

#include "wfx.h"

BEGIN_NAMESPACE_WFX

class WFX_API MemDC
{
public:
	MemDC(HDC hdc, const RECT& rc);
	~MemDC();
public:
	operator HDC();
private:
	HDC m_hdc;
	HDC m_hdcMem;
	RECT m_rc;
	HBITMAP m_hBitmap;
	HGDIOBJ m_hOldBitmap;
	LONG m_lWidth;
	LONG m_lHeight;
	POINT m_ptOrg;
};

class WFX_API GdiPlusHelper
{
public:
	GdiPlusHelper();
	~GdiPlusHelper();
	ULONG_PTR m_nGdiPlusToken;
};

extern WFX_API GdiPlusHelper gdiplushelper;

class WFX_API CommonDraw
{
public:
	static BOOL GetRoundRect(const Gdiplus::RectF& rc, 
		Gdiplus::GraphicsPath& path, float r = 10.0);
	static void DrawButtton(WORD wState, const Gdiplus::RectF& rc,
		Gdiplus::Graphics& grph, WfxImagePtr pImage = WfxImagePtr());
	static void DrawCheckBox(BOOL bCheck, const Gdiplus::RectF& rc,
		Gdiplus::Graphics& grph, WfxImagePtr pImage = WfxImagePtr());
	static void DrawRadioBox(BOOL bCheck, const Gdiplus::RectF& rc,
		Gdiplus::Graphics& grph, WfxImagePtr pImage = WfxImagePtr());
};

END_NAMESPACE_WFX
