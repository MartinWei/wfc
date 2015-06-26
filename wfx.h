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

#ifdef WFX_EXPORTS
#define WFX_API __declspec(dllexport)
#else
#define WFX_API __declspec(dllimport)
#pragma comment(lib, "wfc.lib")
#endif

#define BEGIN_NAMESPACE_WFX	//namespace wfx {
#define END_NAMESPACE_WFX		//}
#define USING_NAMESPACE_WFX	//using namespace wfx

#include <xstring>
#include <vector>
#include <map>
#include <algorithm>
#include <windowsx.h>
#include <CommCtrl.h>
#include <OleCtl.h>
#include <memory>
#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

#pragma warning(disable:4251)
#pragma warning(disable:4244)

DECLARE_HANDLE	(HWID);

#ifndef NULL
#define NULL 0
#endif

#ifndef ASSERT
#define ASSERT(expr) _ASSERT(expr)
#endif

#define SharedPtr std::tr1::shared_ptr

#ifdef _DEBUG
#ifndef TRACE
#define TRACE __Trace
#endif
#else
#define TRACE
#define TRACEMSG _T("")
#endif

#define lengthof(x) (sizeof(x)/sizeof(*x))

void WFX_API __Trace(LPCTSTR pstrFormat, ...);

#ifndef TDEL
#define TDEL(p) if ((p) != NULL) { delete (p); (p) = NULL; }
#endif

#ifndef TDELA
#define TDELA(p) if ((p) != NULL) { delete[] (p); (p) = NULL; }
#endif

#define INVALID_HWID NULL

#define SIZE_SCROLLBAR 11

#define MIN_SCROLLBAR_SLIDER 15

#define WESB_NONE	0x0000
#define WESB_VERT   0x0001
#define WESB_HORZ   0x0010
#define WESB_BOTH   (WESB_VERT | WESB_HORZ)


#define WID_DEFAULT_TEXT	L"WFX Widget"
// State
#define WID_STATE_STATIC	0
#define WID_STATE_MOUSE		1
#define WID_STATE_PUSH		2
#define WID_STATE_CHECKED	3

// State Color
#define WID_BKGND_STATIC	RGB(238, 238, 238)
#define WID_BKGND_MOUSE		RGB(228, 240, 246)
#define WID_BKGND_PUSH		RGB(179, 224, 249)
#define WID_BKGND_CHECKED	RGB(201, 234, 252)

#define WID_FRAME_STATIC	RGB(100, 100, 100)
#define WID_FRAME_MOUSE		RGB(90, 90, 90)
#define WID_FRAME_PUSH		RGB(190, 190, 190)
#define WID_FRAME_CHECKED	RGB(45, 212, 255)

#define WID_TEXT_STATIC		RGB(180, 180, 180)
#define WID_TEXT_MOUSE		RGB(180, 180, 180)
#define WID_TEXT_PUSH		RGB(255, 255, 128)
#define WID_TEXT_CHECKED	RGB(0, 0, 0)

#define WID_FONT_STATIC		L"System"
#define WID_FONT_MOUSE		L"System"
#define WID_FONT_PUSH		L"System"
#define WID_FONT_CHECKED	L"System"

#define WID_FSIZE_STATIC	10
#define WID_FSIZE_MOUSE		10
#define WID_FSIZE_PUSH		10
#define WID_FSIZE_CHECKED	10

// check box color
#define WID_CKB_BKGND		RGB(244, 244, 244)
#define WID_CKB_FRAMEO		RGB(142, 143, 143)
#define WID_CKB_FRAMEI		RGB(202, 207, 212)
#define WID_CKB_CHECK		RGB(49, 52, 124)
// check box size
#define WID_CKB_MARGIN		1
#define WID_CKB_SIZE		13

#define WID_RB_CHECK		RGB(19, 127, 188)

#define WFX_BEGIN_MSG_MAP(theClass)\
	public:\
	BOOL ProcessWidMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID)\
{\
	BOOL bHandled = TRUE;\
	(uMsg); \
	(wParam); \
	(lParam); \
	(lResult); \
	(bHandled); \
	switch(dwMsgMapID) \
{ \
	case 0:

#define WFX_MESSAGE_HANDLER(msg, func) \
	if (uMsg == msg) \
{ \
	bHandled = TRUE; \
	lResult = func(uMsg, wParam, lParam, bHandled); \
	if (bHandled = TRUE) \
	return TRUE; \
}

#define WFX_CHAIN_MSG_MAP(theChainClass) \
{ \
	if(theChainClass::ProcessWidMessage(uMsg, wParam, lParam, lResult, dwMsgMapID)) \
	return TRUE; \
}

#define WFX_END_MSG_MAP()\
	break; \
		default: \
		TRACE(L"Invalid message map ID (%i)\n", dwMsgMapID); \
		ASSERT(FALSE); \
		break; \
} \
	return FALSE; \
}

#define wfx_msg

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
public:
	static BOOL GetRoundRect(const Gdiplus::RectF& rc, 
		Gdiplus::GraphicsPath& path, float r = 10.0);  

};

extern WFX_API GdiPlusHelper gdiplushelper;

END_NAMESPACE_WFX

#define __begin_mem_draw { 

#define __end_mem_draw }
