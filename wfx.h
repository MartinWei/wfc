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
#include <string>
#include <sstream>
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

#define SharedPtr  std::tr1::shared_ptr
#define WeakPtr	   std::tr1::weak_ptr

#ifdef _DEBUG
#ifndef TRACE
#define TRACE __Trace
#endif
#else
#define TRACE
#define TRACEMSG _T("")
#endif

#define lengthof(x) (sizeof(x)/sizeof(*x))

void WFX_API __Trace(const wchar_t* pstrFormat, ...);

#ifndef TDEL
#define TDEL(p) if ((p) != NULL) { delete (p); (p) = NULL; }
#endif

#ifndef TDELGDI
#define TDELGDI(p) if ((p) != NULL) { ::DeleteObject((p)), (p) = NULL; }
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


#define WID_DEFAULT_TEXT	L""
// State
#define WID_STATE_STATIC	0
#define WID_STATE_MOUSE		1
#define WID_STATE_PUSH		2
#define WID_STATE_CHECKED	3

// Widget State Color
#define WID_BKGND_STATIC	RGB(0, 0, 0)
#define WID_BKGND_MOUSE		RGB(60, 60, 60)
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

#define WBTN_BKGND_STATIC	RGB(30, 30, 30)
#define WBTN_BKGND_MOUSE	RGB(40, 40, 40)
#define WBTN_BKGND_PUSH		WBTN_BKGND_MOUSE
#define WBTN_BKGND_CHECKED	RGB(60, 60, 60)

#define WBTN_FRAME_STATIC	RGB(45, 45, 45)
#define WBTN_FRAME_MOUSE	RGB(75, 75, 75)
#define WBTN_FRAME_PUSH		WBTN_FRAME_MOUSE
#define WBTN_FRAME_CHECKED	WBTN_FRAME_STATIC

// check box color
#define WCKB_BKGND_STATIC	RGB(244, 244, 244)
#define WCKB_BKGND_MOUSE	RGB(142, 143, 143)
#define WCKB_BKGND_PUSH		RGB(202, 207, 212)
#define WCKB_BKGND_CHECKED	RGB(49, 52, 124)
// check box size
#define WID_CKB_MARGIN		1
#define WID_CKB_SIZE		13

#define WID_RB_CHECK		RGB(19, 127, 188)

// TextBox
#define WID_TBM_READ		0x00000001
#define WID_TBM_WRITE		0x00000010
#define WID_TBM_PW			0x00000100
#define WID_TBM_READWRITE	(WID_TBM_READ | WID_TBM_WRITE)

// ListCtrl
#define WID_LC_HEAD_HEIGHT	20

#define WCELL_BKGRND		WID_BKGND_STATIC
#define WCELL_FRAME			WID_FRAME_STATIC
#define WCELL_SELECTED		RGB(255, 255, 255)

// Cell State
#define WCS_NORMAL			0x00000000
#define WCS_SELECTED		0x00000001
#define WCS_ASORT			0x00000010
#define WCS_NASORT			0x00000100

enum Wfx_Msg
{
	WUM_BEGIN = WM_USER + 500,
	WUM_SET_HOFFSET,
	WUM_SET_VOFFSET,
	WUM_GET_VIRTUAL_SIZE,
	WUM_GET_VISUAL_SIZE,
	WUM_SB_OFFSET,
	WUM_LC_HITCOL,
	WUM_LC_HEADSIZE,
	WUM_END
};

#define WFX_BEGIN_MSG_MAP(theClass)\
	public:\
	BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID)\
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
	if(theChainClass::ProcessMessage(uMsg, wParam, lParam, lResult, dwMsgMapID)) \
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

#define __begin_mem_draw { 

#define __end_mem_draw }

BEGIN_NAMESPACE_WFX

typedef SharedPtr<Gdiplus::Image>			WfxImagePtr;
typedef SharedPtr<Gdiplus::StringFormat>	WfxFormatPtr;
typedef SharedPtr<Gdiplus::Font>			WfxFontPtr;

WFX_API std::wstring StrFormat(const wchar_t* pstrFormat, ...);

END_NAMESPACE_WFX
