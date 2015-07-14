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

Cell::Cell()
{

}

Cell::~Cell()
{

}

LRESULT Cell::OnMouseMove( UINT uMsg, 
						  WPARAM wParam, 
						  LPARAM lParam,
						  BOOL& bHandled )
{
	return 0;
}

LRESULT Cell::OnMouseLeave( UINT uMsg, 
						   WPARAM wParam, 
						   LPARAM lParam, 
						   BOOL& bHandled )
{
	return 0;
}

void Cell::Draw( HDC hdc, const RECT& rcPaint,  DWORD dwState,
				const std::wstring& strText, COLORREF clrText, DWORD dwFormat )
{
	WfxRender::DrawHeadCell(hdc, rcPaint, dwState, strText,
		clrText, dwFormat);
}

ULONG HeaderCtrl::GetItemCount() const
{
	return m_rgpHdi.size();
}

BOOL HeaderCtrl::GetItemRect( ULONG nIndex, 
							 RECT& rc )
{
	return TRUE;
}

ULONG HeaderCtrl::InsertItem( ULONG nPos, 
							 const HeaderInfo& hdi )
{
	SharedPtr<HeaderInfo> pHdi(new HeaderInfo(hdi));
	m_rgpHdi.push_back(pHdi);
	return m_rgpHdi.size();
}

BOOL HeaderCtrl::RemoveItem( ULONG nPos )
{
	return FALSE;
}

void HeaderCtrl::OnDraw( HDC hdc, const RECT& rcPaint )
{
	Cell* pCell = NULL;
	DWORD dwState = WCS_NORMAL;
	for (ULONG i = 0; i < m_rgpHdi.size(); i++)
	{
		pCell = GetCell(i);
		if (pCell != NULL)
		{
			if (i == m_nSelected)
			{
				dwState |= WCS_SELECTED;
				m_bAscendSort? dwState |= WCS_ASORT : dwState |= WCS_NASORT;
			}
			else
			{
				dwState = WCS_NORMAL;
			}
			pCell->Draw(hdc, m_rgpHdi[i]->rcPos, dwState, 
				m_rgpHdi[i]->strText, RGB(255, 0, 0), m_rgpHdi[i]->dwFormat);
		}
	}
}

LRESULT HeaderCtrl::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	RECT rc = GetRect();
	RECT rcItem = rc;
	for (ULONG i = 0; i < m_rgpHdi.size(); i++)
	{
		rcItem.right = rcItem.left + m_rgpHdi[i]->cxy;
		
		m_rgpHdi[i]->rcPos = rcItem;
		
		rcItem.left += rcItem.right - rcItem.left;
	}
	return 1;
}

LRESULT HeaderCtrl::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	LONG nSelected = GetSelected(pt);
	m_bAscendSort = !m_bAscendSort;
	m_nSelected = nSelected;
	Widget* pParent = GetParent();
	if (pParent != NULL)
	{
		return pParent->SendWidMessage(WFXM_LC_HITCOL, 
			(WPARAM)m_nSelected, (LPARAM)m_bAscendSort);
	}
	return 1;
}

HeaderCtrl::HeaderCtrl()
: m_nSelected(-1)
, m_bAscendSort(FALSE)
{

}

LONG HeaderCtrl::GetSelected() const
{
	return m_nSelected;
}

void HeaderCtrl::SetSelected( LONG nSelected )
{
	m_nSelected = nSelected;
}

LONG HeaderCtrl::GetSelected( POINT pt )
{
	RECT rcItem = {0};
	for (ULONG i = 0; i < m_rgpHdi.size(); i++)
	{
		rcItem = m_rgpHdi[i]->rcPos;
		if (::PtInRect(&rcItem, pt))
		{
			return i;
		}
	}
	return -1;
}

Cell* HeaderCtrl::GetCell( ULONG nCol )
{
	static Cell cell;
	return &cell;
}

BOOL HeaderCtrl::IsAscendSort() const
{
	return m_bAscendSort;
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

std::wstring ListCtrl::GetItemText( ULONG nItem, 
								   ULONG nSubItem ) const
{
	return L"";
}

BOOL ListCtrl::SetItemText( ULONG nItem, 
						   ULONG nSubItem, 
						   const std::wstring& strText )
{
	return FALSE;
}

LRESULT ListCtrl::OnCreate( UINT uMsg, 
						   WPARAM wParam, 
						   LPARAM lParam, 
						   BOOL& bHandled )
{
	RECT rc;
	m_pHeadCtrl->Create(rc, m_pDispatch, this);
	return 0;
}

LRESULT ListCtrl::OnSize( UINT uMsg, 
						 WPARAM wParam, 
						 LPARAM lParam, 
						 BOOL& bHandled )
{
	RECT rc = GetRect();
	rc.bottom = rc.top + m_nHeadHeight;
	rc.left += 1;
	rc.right -= 1;
	rc.top += 1;
	m_pHeadCtrl->SetWidRect(rc);
	return 0;
}

int ListCtrl::InsertColumn( const std::wstring& strName, 
						   UINT nWidth, 
						   DWORD dwFormat, 
						   ULONG nCellType )
{
	HeaderInfo hi;
	hi.cxy = nWidth;
	hi.strText = strName;
	m_pHeadCtrl->InsertItem(m_pHeadCtrl->GetItemCount() + 1, hi);
	return m_pHeadCtrl->GetItemCount();
}

Cell* ListCtrl::GetCell( ULONG nRow, 
						ULONG nCol )
{
	static Cell cell;
	return &cell;
}

LRESULT ListCtrl::OnColChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	InvalidWid();
	return 1;
}
