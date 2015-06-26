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
#include "Widget.h"

USING_NAMESPACE_WFX;

Window::Window()
: m_pDispatch(new Dispatcher)
, m_hWnd(NULL)
{

}

Window::~Window()
{

}

HWND Window::GetHwnd() const
{
	return m_hWnd;
}

Window::operator HWND() const
{
	return m_hWnd;
}

BOOL Window::RegistWindowClass()
{
	return FALSE;
}

BOOL Window::RegistSuperClass()
{
	return FALSE;
}
