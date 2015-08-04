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
#include "stdafx.h"
#include "wfx.h"

#ifdef _DEBUG
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

USING_NAMESPACE_WFX;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
void __Trace( const wchar_t* pstrFormat, ... )
{
#ifdef _DEBUG
	va_list arglist;
	va_start(arglist, pstrFormat);
	if (pstrFormat == NULL)
	{
		return;
	}
	int nLength = String::GetFormattedLength(pstrFormat, arglist);
	wchar_t* pszBuffer = new wchar_t[nLength + 1];
	::ZeroMemory(pszBuffer, nLength + 1);
	String::Format(pszBuffer, nLength + 1, pstrFormat, arglist);
	va_end(arglist);
	String str = pszBuffer;
	delete []pszBuffer;
	str += L"\n";
	::OutputDebugStringW(str.c_str());
#endif
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
void String::Format( const wchar_t* pszFormat, ... )
{
	va_list arglist;
	va_start(arglist, pszFormat);
	FormatV(pszFormat, arglist);
	va_end(arglist);
}

int String::Format( wchar_t* pszBuffer, ULONG nLength, const wchar_t* pszFormat, va_list args )
{
	return vswprintf_s(pszBuffer, nLength, pszFormat, args);
}

void String::FormatV( const wchar_t* pszFormat, va_list args )
{
	if (pszFormat == NULL)
	{
		return;
	}
	int nLength = GetFormattedLength(pszFormat, args);
	wchar_t* pszBuffer = new wchar_t[nLength + 1];
	::ZeroMemory(pszBuffer, nLength + 1);
	Format(pszBuffer, nLength + 1, pszFormat, args);
	std::wstring::operator=(String(pszBuffer));
	delete []pszBuffer;
}

int String::GetFormattedLength( const wchar_t* pszFormat, va_list args )
{
	return _vscwprintf(pszFormat, args);
}

String::String()
: std::wstring()
{

}

String::String(const wchar_t* psz)
: std::wstring(psz)
{

}

String::String(const String& rh)
: std::wstring(rh)
{

}

String& String::operator=(const String& rh)
{
	std::wstring::operator=(rh);
	return *this;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Rect::Rect()
{
	Empty();
}

Rect::Rect( const RECT& src )
{
	left = src.left;
	top = src.top;
	right = src.right;
	bottom = src.bottom;
}

Rect::Rect( LONG iLeft, LONG iTop, LONG iRight, LONG iBottom )
{
	left = iLeft;
	top = iTop;
	right = iRight;
	bottom = iBottom;
}

LONG Rect::GetWidth() const
{
	return right - left;
}

LONG Rect::GetHeight() const
{
	return bottom - top;
}

void Rect::Empty()
{
	left = top = right = bottom = 0;
}

void Rect::Join( const RECT& rc )
{
	if( rc.left < left ) left = rc.left;
	if( rc.top < top ) top = rc.top;
	if( rc.right > right ) right = rc.right;
	if( rc.bottom > bottom ) bottom = rc.bottom;
}

void Rect::ResetOffset()
{
	::OffsetRect(this, -left, -top);
}

void Rect::Normalize()
{
	if( left > right ) { int iTemp = left; left = right; right = iTemp; }
	if( top > bottom ) { int iTemp = top; top = bottom; bottom = iTemp; }
}

void Rect::Offset( LONG cx, LONG cy )
{
	::OffsetRect(this, cx, cy);
}

void Rect::Inflate( LONG cx, LONG cy )
{
	::InflateRect(this, cx, cy);
}

void Rect::Deflate( LONG cx, LONG cy )
{
	::InflateRect(this, -cx, -cy);
}

void Rect::Union( Rect& rc )
{
	::UnionRect(this, this, &rc);
}

Rect::operator LPRECT()
{
	return (tagRECT*)this;
}

Rect::operator LPCRECT()
{
	return (tagRECT*)this;
}

BOOL Rect::PtInRect( POINT pt ) const
{
	return ::PtInRect((tagRECT*)this, pt);
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Size::Size()
{
	Empty();
}

Size::Size( const SIZE& src )
{
	cx = src.cx;
	cy = src.cy;
}

Size::Size( const RECT& rc )
{
	cx = rc.right - rc.left;
	cy = rc.bottom - rc.top;
}

Size::Size( LONG cx, LONG cy )
{
	this->cx = cx;
	this->cy = cy;
}

Size::Size( LPARAM lParam )
{
	cx = LOWORD(lParam);
	cy = HIWORD(lParam);
}


void Size::Empty()
{
	cx = cy = 0;
}

Size::operator LPSIZE()
{
	return (tagSIZE*)this;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Point::Point( )
{
	Empty();
}

Point::Point( const POINT& src )
{
	x = src.x;
	y = src.y;
}

Point::Point( LONG x, LONG y )
{
	this->x = x;
	this->y = y;
}

Point::Point( LPARAM lParam )
{
	x = GET_X_LPARAM(lParam);
	y = GET_Y_LPARAM(lParam);
}

void Point::Empty()
{
	x = y = 0;
}

Point::operator LPPOINT()
{
	return (tagPOINT*)this;
}
