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

HWND TextBoxWnd::CreateInPlaceWindow()
{
	TextBox* pTextBox = dynamic_cast<TextBox*>(m_pOwner);
	ASSERT(pTextBox != NULL);
	ASSERT(pTextBox->m_pDispatch != NULL);
	Gdiplus::RectF rcWid;
	pTextBox->GetRect(rcWid);
	RECT rc = Dispatcher::FromRect(rcWid);
	::InflateRect(&rc, -1, -1);
	Create(m_pOwner->m_pDispatch->GetHwnd(),
		NULL, WS_CHILD, ES_AUTOHSCROLL, rc);
	SetFont(m_pOwner->GetFontObject());
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

void TextBoxWnd::OnInPlaceWindowKillFocus()
{
	if (m_pOwner == NULL)
	{
		return;
	}
	m_pOwner->SetText(GetText());
}

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
