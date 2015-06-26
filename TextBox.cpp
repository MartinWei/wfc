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

BOOL TextBoxWnd::Initial( InPlaceWid* pOwner )
{

}

LPCTSTR TextBoxWnd::GetWindowClassName() const
{

}

LPCTSTR TextBoxWnd::GetSuperClassName() const
{

}

LRESULT TextBoxWnd::HandleMessage( UINT nMsg, WPARAM wParam, LPARAM lParam )
{

}

void TextBoxWnd::OnFinalMessage( HWND hWnd )
{

}

LRESULT TextBoxWnd::OnKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{

}

LRESULT TextBoxWnd::OnEditChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{

}

BOOL TextBox::Initial()
{
	if (m_bReadOnly)
	{
		return FALSE;
	}
	return FALSE;
}
