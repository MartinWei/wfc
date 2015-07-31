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