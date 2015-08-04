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
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
HWND TextBoxWnd::CreateInPlaceWindow()
{
	TextBox* pTextBox = dynamic_cast<TextBox*>(m_pOwner);
	WFX_CONDITION(pTextBox != NULL);
	WFX_CONDITION(pTextBox->m_pDispatch != NULL);
	Rect rcWid = pTextBox->GetRect();
	Rect rc = rcWid;
	::InflateRect(&rc, -1, -3);
	Create(m_pOwner->m_pDispatch->GetHwnd(),
		NULL, WS_CHILD, ES_AUTOHSCROLL, rc);
	SetFont(WfxRender::GetFontObject());
	SetText(m_pOwner->GetText());
	Edit_SetModify(m_hWnd, FALSE);
	SendMessageW(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(2, 2));
	Edit_SetSel(m_hWnd, 0, -1);
	Edit_Enable(m_hWnd, TRUE);
	Edit_SetReadOnly(m_hWnd, FALSE);
	if (pTextBox->GetMode() & WID_TBM_PW)
	{

	}
	//Edit_SetPasswordChar(m_hWnd,7);
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	::SetFocus(m_hWnd);
	return m_hWnd;
}

LPCTSTR TextBoxWnd::GetWindowClassName() const
{
	return L"TextBoxWnd";
}

LPCTSTR TextBoxWnd::GetSuperClassName() const
{
	return WC_EDITW;
}

LRESULT TextBoxWnd::OnEditChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE && m_pOwner != NULL)
	{
		m_pOwner->SetText(GetText());
	}
	else
	{
		bHandled = FALSE;
	}
	return 0;
}

LRESULT TextBoxWnd::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	if (m_pOwner != NULL)
	{
		m_pOwner->SendWidMessage(uMsg, wParam, lParam);
	}
	return 1;
}


void TextBoxWnd::OnInPlaceWindowKillFocus()
{
	if (m_pOwner == NULL)
	{
		return;
	}
	m_pOwner->SetText(GetText());
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
TextBox::TextBox( WORD wMode /*= WID_TBM_READWRITE*/ )
: m_wMode(wMode)
, m_bEditting(FALSE)
{
	SetText(L"TextBox");
}

BOOL TextBox::Initial()
{
	if (IsReadonly())
	{
		return FALSE;
	}
	m_bEditting = TRUE;
	m_pWindow = new TextBoxWnd;
	m_pWindow->Initial(this);
	return FALSE;
}

void TextBox::OnDraw(HDC hdc, const Rect& rcPaint)
{
	WfxRender::DrawTextBox(hdc, GetText(), GetRect(), GetState(), GetMode(), m_pDispatch);
}

LRESULT TextBox::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
	return __super::OnMouseMove(uMsg, wParam, lParam, bHandled);
}

LRESULT TextBox::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

void TextBox::SetMode( WORD wMode, BOOL bAdd /*= FALSE*/ )
{
	if (bAdd)
	{
		m_wMode |= wMode;
	}
	else
	{
		m_wMode = wMode;
	}
}

WORD TextBox::GetMode() const
{
	return m_wMode;
}

BOOL TextBox::IsReadonly() const
{
	return !(WID_TBM_WRITE & m_wMode);
}

BOOL TextBox::IsPassword() const
{
	return WID_TBM_PW & m_wMode;
}
