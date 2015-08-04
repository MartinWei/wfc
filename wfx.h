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
#define END_NAMESPACE_WFX	//	}
#define USING_NAMESPACE_WFX	//using namespace wfx;

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

#ifndef WFX_ASSERT
#define WFX_ASSERT(expr) _ASSERT(expr)
#endif

#ifndef WFX_CONDITION
#define WFX_CONDITION WFX_ASSERT
#endif

#ifndef WFX_CONDITION_VALID
#define WFX_CONDITION_VALID(expr) WFX_CONDITION(expr != NULL)
#endif

#define SharedPtr  std::tr1::shared_ptr
#define WeakPtr	   std::tr1::weak_ptr

BEGIN_NAMESPACE_WFX

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API String : public std::wstring
{
public:
	String();
	String(const wchar_t* psz);
	String(const String& rh);
	String& operator=(const String& rh);
public:
	void Format(const wchar_t* pszFormat, ...);
protected:
	void FormatV(const wchar_t* pszFormat, va_list args);
public:
	static int GetFormattedLength(const wchar_t* pszFormat, va_list args);
	static int Format(wchar_t* pszBuffer, ULONG nLength, const wchar_t* pszFormat, va_list args);
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
template<class T>
class Factory
{
public:
	SharedPtr<T> CreateObject()
	{
		return SharedPtr<T>(new T());
	}
};

#define WFX_DECLARE_FACTORY(classname) \
	friend class Factory<classname>;

#define WFX_CREATE_FACTORY(classname) \
	SharedPtr<Factory<classname> >(new Factory<classname>())
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Rect : public tagRECT
{
public:
	Rect();
	Rect(const RECT& src);
	Rect(LONG iLeft, LONG iTop, LONG iRight, LONG iBottom);

	LONG GetWidth() const;
	LONG GetHeight() const;
	void Empty();
	void Join(const RECT& rc);
	void ResetOffset();
	void Normalize();
	void Offset(LONG cx, LONG cy);
	void Inflate(LONG cx, LONG cy);
	void Deflate(LONG cx, LONG cy);
	void Union(Rect& rc);
	operator LPRECT();
	operator LPCRECT();
	BOOL PtInRect(POINT pt) const;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Size : public tagSIZE
{
public:
	Size();
	Size(const SIZE& src);
	Size(const RECT& rc);
	Size(LONG cx, LONG cy);
	Size(LPARAM lParam);
	void Empty();
	operator LPSIZE();

};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Point : public tagPOINT
{
public:
	Point();
	Point(const POINT& src);
	Point(LONG x, LONG y);
	Point(LPARAM lParam);
	void Empty();
	operator LPPOINT();
};

END_NAMESPACE_WFX

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
#define WID_BKGND_STATIC	RGB(255, 255, 255)
#define WID_BKGND_MOUSE		RGB(60, 60, 60)
#define WID_BKGND_PUSH		RGB(179, 224, 249)
#define WID_BKGND_CHECKED	RGB(201, 234, 252)

#define WID_FRAME_STATIC	RGB(100, 100, 100)
#define WID_FRAME_MOUSE		RGB(77, 137, 193)
#define WID_FRAME_PUSH		RGB(77, 137, 193)
#define WID_FRAME_CHECKED	RGB(77, 137, 193)

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

#define WBTN_BKGND_STATIC	RGB(255, 255, 255)
#define WBTN_BKGND_MOUSE	RGB(77, 137, 193)
#define WBTN_BKGND_PUSH		RGB(77, 137, 193)
#define WBTN_BKGND_CHECKED	RGB(77, 137, 193)

#define WBTN_FRAME_STATIC	RGB(77, 137, 193)
#define WBTN_FRAME_MOUSE	RGB(77, 137, 193)
#define WBTN_FRAME_PUSH		RGB(77, 137, 193)
#define WBTN_FRAME_CHECKED	RGB(77, 137, 193)

#define WTXB_BKGND_STATIC	RGB(255, 255, 255)
#define WTXB_BKGND_MOUSE	WTXB_BKGND_STATIC
#define WTXB_BKGND_PUSH		WTXB_BKGND_STATIC
#define WTXB_BKGND_CHECKED	WTXB_BKGND_STATIC

// check box color
#define WCKB_BKGND_STATIC	RGB(244, 244, 244)
#define WCKB_BKGND_MOUSE	RGB(142, 143, 143)
#define WCKB_BKGND_PUSH		RGB(202, 207, 212)
#define WCKB_BKGND_CHECKED	RGB(49, 52, 124)
#define WCKB_ITEM_BKGND		RGB()
#define WCKB_ITEM_MARK		RGB()
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
#define WCELL_SELECTED		WBTN_BKGND_MOUSE

// Cell State
#define WCS_NORMAL			0x00000000
#define WCS_SELECTED		0x00000001
#define WCS_ASORT			0x00000010
#define WCS_NASORT			0x00000100
#define WCS_MOUSEMOVE		0x00001000
#define WCS_EXPAND			0x00010000
#define WCS_CLOSE			0x00100000

// Cell Type
#define WCT_TEXT			0
#define WCT_Layer			1

enum Wfx_Msg
{
	WUM_BEGIN = WM_USER + 500,
	WUM_LBUTTONCLICK,
	WUM_SET_HOFFSET,
	WUM_SET_VOFFSET,
	WUM_QUERY_VIRTUAL_SIZE,
	WUM_GET_VIRTUAL_SIZE,
	WUM_GET_VISUAL_SIZE,
	WUM_SB_OFFSET,
	WUM_LC_HITCOL,
	WUM_LC_HEADSIZE,
	WUM_LC_GET_PROPERTIES,
	WUM_LC_CELL_DRAW,
	WUM_LC_CELL_EXPAND,
	WUM_END
};

enum Wfx_LC_Param
{
	LCP_START_ROW,
	LCP_END_ROW,
	LCP_START_COL,
	LCP_END_COL,
	LCP_ROWNUM_WIDTH,
	LCP_ROW_HEIGHT
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

#define WFX_COMMAND_HANDLER(id, code, func) \
	if (uMsg == WM_COMMAND && id == LOWORD(wParam) && code == HIWORD(wParam)) \
	{\
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), lParam, bHandled); \
		if (bHandled)\
		{\
			return TRUE;\
		}\
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
		WFX_CONDITION(FALSE); \
		break; \
} \
	return FALSE; \
}

#define wfx_msg

#define __begin_mem_draw { 

#define __end_mem_draw }

BEGIN_NAMESPACE_WFX

typedef SharedPtr<Gdiplus::Image>				PImage;
typedef SharedPtr<LOGFONTW>						PFont;

enum Wid_Type
{
	WT_WIDGET,
	WT_BUTTON,
	WT_TEXTBOX,
};

END_NAMESPACE_WFX
