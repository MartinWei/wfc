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

class WFX_API WfxRender
{
public:
	class MemDC
	{
	public:
		MemDC(HWND hWnd, const Rect& rcPaint);
		~MemDC();
	public:
		operator HDC();
	private:
		HDC m_hdc;
		HDC m_hdcMem;
		Rect m_rcPaint;
		HBITMAP m_hBitmap;
		HGDIOBJ m_hOldBitmap;
		HWND m_hWnd;
	};

	class RenderClip
	{
	public:
		RenderClip(HDC hdc, const Rect& rcItem);
		~RenderClip();
		Rect m_rcItem;
		HDC m_hDC;
		HRGN m_hRgn;
		HRGN m_hOldRgn;
	};
public:

	static void DrawWidget(HDC hdc, const String& strText, const Rect& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void DrawButtton(HDC hdc, const String& strText, const Rect& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void DrawTextBox(HDC hdc, const String& strText, const Rect& rc, WORD wState, WORD wMode, WidDispatch* pDispatch = NULL);
	static void DrawCheckBoxItem(HDC hdc, const Rect& rc, WORD wState, BOOL bChecked, WidDispatch* pDispatch = NULL);
	static void DrawCheckBox(HDC hdc, const String& strText, const Rect& rc, WORD wState, ULONG nTextOffset, WidDispatch* pDispatch = NULL);
	static void DrawRadioBoxItem(HDC hdc, const Rect& rc, WORD wState, BOOL bChecked, WidDispatch* pDispatch = NULL);
	static void DrawRadioBox(HDC hdc, const Rect& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void DrawProcessBar(HDC hdc, const Rect& rc, WORD wState, ULONG nMax, ULONG nPos, WidDispatch* pDispatch = NULL);
	static void DrawSolidRect(HDC hdc, const Rect& rcPaint, COLORREF clr, WidDispatch* pDispatch = NULL);
	static void DrawFrame(HDC hdc, const Rect& rcPaint, COLORREF clr, WidDispatch* pDispatch = NULL);
	static void DrawText(HDC hdc, const Rect& rcPaint, const String& strText, COLORREF clr, DWORD dwFormat, HFONT hFont = NULL, WidDispatch* pDispatch = NULL);
	static void DrawSlider(HDC hdc, const Rect& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void DrawArror(HDC hdc, const Rect& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void GenerateClip(HDC hdc, const Rect& rcItem, RenderClip& clip);
	static void DrawHeadCell(HDC hdc, const Rect& rcPaint,  DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
	static void DrawLinkCell(HDC hdc, const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
	static void DrawLayerCell(HDC hdc, const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
	static Size EstimateWidgetSize(const Rect& rc, const String& strText, WORD wState, WidDispatch* pDispatch = NULL);
	static HFONT GetFontObject();
protected:
	static HFONT s_hFont;
};

END_NAMESPACE_WFX
