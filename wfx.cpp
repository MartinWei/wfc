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
	WCHAR szBuffer[300] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	::wvnsprintf(szBuffer, lengthof(szBuffer) - 2, pstrFormat, args);
	wcscat_s(szBuffer, 300, L"\n");
	va_end(args);
	::OutputDebugStringW(szBuffer);
#endif
}

std::wstring StrFormat( const wchar_t* pstrFormat, ... )
{
	WCHAR szBuffer[300] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	::wvnsprintf(szBuffer, lengthof(szBuffer) - 2, pstrFormat, args);
	wcscat_s(szBuffer, 300, L"\n");
	va_end(args);
	return szBuffer;
}
