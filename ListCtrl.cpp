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

USING_NAMESPACE_WFX;

ULONG HeaderCtrl::GetItemCount() const
{
	return m_rgpHdi.size();
}

BOOL HeaderCtrl::GetItemRect( ULONG nIndex, Gdiplus::RectF& rc )
{
	return TRUE;
}

ULONG HeaderCtrl::InsertItem( ULONG nPos, const HeaderInfo& hdi )
{
	SharedPtr<HeaderInfo> pHdi(new HeaderInfo(hdi));
	return 1;
}

BOOL HeaderCtrl::RemoveItem( ULONG nPos )
{
	return FALSE;
}

ListCtrl::ListCtrl()
: m_pHeadCtrl(new HeaderCtrl)
, m_nHeadHeight(WID_LC_HEAD_HEIGHT)
{

}

ListCtrl::~ListCtrl()
{

}

ULONG ListCtrl::GetColumnWidth( ULONG nCol ) const
{
	return 1;
}

BOOL ListCtrl::SetColumnWidth( ULONG nCol, ULONG cx )
{
	return TRUE;
}

HeaderCtrl* ListCtrl::GetHeaderCtrl() const
{
	return m_pHeadCtrl.get();
}

void ListCtrl::SetHeaderCtrl( HeaderCtrl* pHeader )
{

}

std::wstring ListCtrl::GetItemText( ULONG nItem, ULONG nSubItem ) const
{
	return L"";
}

BOOL ListCtrl::SetItemText( ULONG nItem, ULONG nSubItem, const std::wstring& strText )
{
	return FALSE;
}

LRESULT ListCtrl::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Gdiplus::RectF rc;
	m_pHeadCtrl->Create(rc, m_pDispatch, this);
	return 0;
}

LRESULT ListCtrl::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Gdiplus::RectF rc;
	GetRect(rc);
	rc.Height = m_nHeadHeight;
	m_pHeadCtrl->SetWidRect(rc);
	return 0;
}