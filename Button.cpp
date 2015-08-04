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
#include "wfxwid.h"
#include "wfxcmn.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;

Button::Button(BOOL bCheckable /*= FALSE*/)
: m_bLButtonDown(FALSE)
, m_bChecked(FALSE)
, m_bCheckable(bCheckable)
{
	SetText(L"Button");
}

void Button::OnDraw( HDC hdc, const Rect& rcPaint )
{
	PImage pImage = GetImageFromState();
	if (pImage == NULL)
	{
		Rect rc = GetRect();
		WfxRender::DrawButtton(hdc, GetText(), rc, GetState(), m_pDispatch);
	}
}

void Button::Check( BOOL bCheck /*= TRUE*/ )
{
	m_bChecked = bCheck;
}

BOOL Button::IsChecked() const
{
	return m_bChecked;
}

LRESULT Button::OnLButtonClik( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bChecked = !m_bChecked;
	SendParentMessage(WM_COMMAND, MAKELONG(GetID(), BN_CLICKED), (LPARAM)GetHwid());
	InvalidWid();
	return 1;
}

LRESULT Button::OnStateChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_bCheckable && m_bChecked)
	{
		m_clrText = (WID_TEXT_CHECKED);
		m_clrBkgnd = (WID_BKGND_CHECKED);
		m_clrFrame = (WID_FRAME_CHECKED);
		InvalidWid();
		return 0;
	}
	InvalidWid();
	return 1;
}