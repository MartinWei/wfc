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
		MemDC(HWND hWnd, const RECT& rcPaint);
		~MemDC();
	public:
		operator HDC();
	private:
		HDC m_hdc;
		HDC m_hdcMem;
		RECT m_rcPaint;
		HBITMAP m_hBitmap;
		HGDIOBJ m_hOldBitmap;
		HWND m_hWnd;
	};

	class RenderClip
	{
	public:
		RenderClip(HDC hdc, const RECT& rcItem);
		~RenderClip();
		RECT m_rcItem;
		HDC m_hDC;
		HRGN m_hRgn;
		HRGN m_hOldRgn;
	};
public:

	static void DrawWidget(HDC hdc, const std::wstring& strText, const RECT& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void DrawButtton(HDC hdc, const std::wstring& strText, const RECT& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void DrawTextBox(HDC hdc, const std::wstring& strText, const RECT& rc, WORD wState, WORD wMode, WidDispatch* pDispatch = NULL);
	static void DrawCheckBox(HDC hdc, const RECT& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void DrawRadioBox(HDC hdc, const RECT& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void DrawSolidRect(HDC hdc, const RECT& rcPaint, COLORREF clr, WidDispatch* pDispatch = NULL);
	static void DrawFrame(HDC hdc, const RECT& rcPaint, COLORREF clr, WidDispatch* pDispatch = NULL);
	static void DrawText(HDC hdc, const RECT& rcPaint, const std::wstring& strText, COLORREF clr, DWORD dwFormat, HFONT hFont = NULL, WidDispatch* pDispatch = NULL);
	static void DrawSlider(HDC hdc, const RECT& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void DrawArror(HDC hdc, const RECT& rc, WORD wState, WidDispatch* pDispatch = NULL);
	static void GenerateClip(HDC hdc, const RECT& rcItem, RenderClip& clip);
	static void DrawHeadCell(HDC hdc, const RECT& rcPaint,  DWORD dwState,
		const std::wstring& strText, COLORREF clrText, DWORD dwFormat);
	static void DrawLinkCell(HDC hdc, const RECT& rcPaint, DWORD dwState,
		const std::wstring& strText, COLORREF clrText, DWORD dwFormat);
	static void DrawLayerCell(HDC hdc, const RECT& rcPaint, DWORD dwState,
		const std::wstring& strText, COLORREF clrText, DWORD dwFormat);
	static SIZE EstimateWidgetSize(const RECT& rc, const std::wstring& strText, WORD wState, WidDispatch* pDispatch = NULL);
	static HFONT GetFontObject();
protected:
	static HFONT s_hFont;
};

END_NAMESPACE_WFX
