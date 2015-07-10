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
#include "wfxgdi.h"

USING_NAMESPACE_WFX;

Button::Button(BOOL bCheckState /*= FALSE*/)
: m_bLButtonDown(FALSE)
, m_bChecked(FALSE)
, m_bCheckState(bCheckState)
{
	SetText(L"Button");
}

Button::~Button()
{

}

void Button::OnDraw( Gdiplus::Graphics& grph )
{
	SharedPtr<Gdiplus::Image> pImage = GetImageFromState();
	if (pImage == NULL)
	{
		return RoundWid::OnDraw(grph);
	}

}

LRESULT Button::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bLButtonDown = TRUE;
	return 0;
}

LRESULT Button::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_bLButtonDown)
	{
		m_bChecked = !m_bChecked;
		if (m_bChecked)
		{
			SetState(WID_STATE_CHECKED);
		}
	}
	m_bLButtonDown = FALSE;
	return 0;
}

void Button::Check( BOOL bCheck /*= TRUE*/ )
{
	m_bChecked = bCheck;
}

BOOL Button::IsCheck() const
{
	return m_bChecked;
}

LRESULT Button::OnStateChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_bCheckState && m_bChecked)
	{
		m_clrText.SetFromCOLORREF(WID_TEXT_CHECKED);
		m_clrBkgnd.SetFromCOLORREF(WID_BKGND_CHECKED);
		m_clrFrame.SetFromCOLORREF(WID_FRAME_CHECKED);
		InvalidWid();
		return 0;
	}
	return __super::OnStateChanged(uMsg, wParam, lParam, bHandled);
}