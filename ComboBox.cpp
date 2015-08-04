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
ComboWnd::ComboWnd()
: m_pRoot(new Widget)
{
}

void ComboWnd::OnInPlaceWindowKillFocus()
{

}

HWND ComboWnd::CreateInPlaceWindow()
{
	ComboBox* pComboBox = dynamic_cast<ComboBox*>(m_pOwner);
	WFX_CONDITION(pComboBox != NULL);
	WFX_CONDITION(pComboBox->m_pDispatch != NULL);
	Rect rcWid = pComboBox->GetRect();
	Rect rc = rcWid;
	rc.top = rc.bottom;
	rc.bottom = rc.top + pComboBox->GetSize() * pComboBox->GetItemHeight(0);
	MapWindowRect(pComboBox->m_pDispatch->GetHwnd(), HWND_DESKTOP, &rc);
	Create(pComboBox->m_pDispatch->GetHwnd(), NULL, WS_POPUP | WS_BORDER, WS_EX_TOOLWINDOW, rc);
	HWND hWndParent = m_hWnd;
	while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
	::ShowWindow(m_hWnd, SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	return m_hWnd;
}

LPCTSTR ComboWnd::GetWindowClassName() const
{
	return L"ComboWnd";
}


LRESULT ComboWnd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ComboBox* pComboBox = dynamic_cast<ComboBox*>(m_pOwner);
	WFX_CONDITION(pComboBox != NULL);
	WFX_CONDITION(pComboBox->m_pDispatch != NULL);
	Rect rc;
	m_pRoot->Create(rc, m_pDispatch.get());
	ULONG lItems = pComboBox->GetSize();
	Widget* pChild = NULL;
	m_rgpItems.clear();
	for (ULONG i = 0; i < lItems; i++)
	{
		pChild = pComboBox->GetWid(i);
		if (pChild != NULL)
		{
			pChild->Create(rc, m_pDispatch.get(), m_pRoot.get());
			m_rgpItems.push_back(pChild);
		}
	}
	return 0;
}

LRESULT ComboWnd::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ComboBox* pComboBox = dynamic_cast<ComboBox*>(m_pOwner);
	WFX_CONDITION(pComboBox != NULL);
	WFX_CONDITION(pComboBox->m_pDispatch != NULL);
	Rect rc;
	GetClientRect(rc);
	Rect rcWid = rc;
	WFX_CONDITION(m_pRoot != NULL);
	m_pRoot->SetRect(rcWid);
	rcWid.bottom = rcWid.top + 30;
	ULONG lItems = pComboBox->GetSize();
	for (ULONG i = 0; i < lItems; i++)
	{
		m_rgpItems[i]->SetRect(rcWid);
		rcWid.top += rcWid.bottom - rcWid.top;
	}
	
	return 0;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ULONG ComboBox::GetItemHeight( ULONG nIndex ) const
{
	nIndex;
	return 15;
}

ULONG ComboBox::GetSize() const
{
	return 10;
}

ULONG ComboBox::GetSel() const
{
	return 0;
}

BOOL ComboBox::Initial()
{
	m_pWindow = new ComboWnd;
	m_pWindow->Initial(this);
	return TRUE;
}

Widget* ComboBox::GetWid( ULONG nIndex ) const
{
	if (nIndex == 0)
	{
		return new CheckBoxItem;
	}
	if (nIndex == 1)
	{
		return new RadioButtonItem;
	}
	if (nIndex == 2)
	{
		return new TextBox;
	}
	return new Widget;
}
