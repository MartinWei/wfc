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

LRESULT Cell::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

void Cell::Draw( HDC hdc, const RECT& rcPaint,  DWORD dwState,
				const std::wstring& strText, COLORREF clrText, DWORD dwFormat )
{
	WfxRender::DrawHeadCell(hdc, rcPaint, dwState, strText,
		clrText, dwFormat);
}

void Cell::SetParent( Widget* pParent )
{
	m_pParent = pParent;
}

void Cell::SetRect( const RECT& rc )
{
	m_rc = rc;
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
		rcItem.right = rcItem.left + m_rgpHdi[i]->cx;
		
		m_rgpHdi[i]->rcPos = rcItem;
		
		rcItem.left += rcItem.right - rcItem.left;
	}
	SendParentMessage(WUM_LC_HEADSIZE);
	return 1;
}

LRESULT HeaderCtrl::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	LONG nSelected = GetSelected(pt);
	m_bAscendSort = !m_bAscendSort;
	m_nSelected = nSelected;
	SendParentMessage(WUM_LC_HITCOL, 
		(WPARAM)m_nSelected, (LPARAM)m_bAscendSort);
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

ULONG HeaderCtrl::GetTotalColumns() const
{
	return m_rgpHdi.size();
}

ULONG HeaderCtrl::GetColumnWidth( ULONG nCol ) const
{
	if (nCol >= m_rgpHdi.size())
		return 0;
	return m_rgpHdi[nCol]->cx;
}


CellID::CellID(LONG nRow /*= -1*/, LONG nCol /*= -1*/)
: m_nRow(nRow)
, m_nCol(nCol)
{
}

BOOL CellID::IsValidID() const
{
	return (m_nRow >= 0 && m_nCol >= 0);
}

bool CellID::operator()(const CellID& id1, const CellID& id2) const
{
	if (id1.m_nRow < id2.m_nRow)
		return true;
	else if (id1.m_nRow > id2.m_nRow)
		return false;
	else if (id1.m_nCol < id2.m_nCol )
		return true;
	else
		return false;
}

bool CellID::operator<(const CellID& other) const
{
	return operator()(*this, other);
}

bool CellID::operator==( const CellID& other ) const
{
	return (other.m_nRow == m_nRow && other.m_nCol == m_nCol);
}

ListCtrl::ListCtrl()
: m_pHeadCtrl(new HeaderCtrl)
, m_nHeadHeight(WID_LC_HEAD_HEIGHT)
, m_nTotalRow(0)
, m_nStartRow(0)
, m_nEndRow(0)
, m_nStartCol(0)
, m_nEndCol(0)
, m_nFixedRow(-1)
, m_nFixedCol(-1)
{

}

ListCtrl::~ListCtrl()
{

}

ULONG ListCtrl::GetColumnWidth( ULONG nCol ) const
{
	return m_pHeadCtrl->GetColumnWidth(nCol);
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
	m_pHeadCtrl->SetRect(rc);
	CalcCol();
	CalcRow();
	return 0;
}

int ListCtrl::InsertColumn( const std::wstring& strName, 
						   UINT nWidth, 
						   DWORD dwFormat, 
						   ULONG nCellType )
{
	HeaderInfo hi;
	hi.cx = nWidth;
	hi.strText = strName;
	m_pHeadCtrl->InsertItem(m_pHeadCtrl->GetItemCount() + 1, hi);
	return m_pHeadCtrl->GetItemCount();
}

Cell* ListCtrl::GetCell( ULONG nRow, 
						ULONG nCol )
{
	static Cell cell;
	cell.SetParent(this);
	return &cell;
}

BOOL ListCtrl::GetCellID( POINT pt, std::pair<CellID, RECT>& cellID)
{
	for(std::map<CellID, RECT>::iterator it = m_rgRect.begin();
		it != m_rgRect.end(); ++it)
	{
		if (::PtInRect(&(it->second), pt))
		{
			cellID = (*it);
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT ListCtrl::OnColChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	InvalidWid();
	return 1;
}

int ListCtrl::InsertRow( int nHeight /*= -1*/ )
{
	m_nTotalRow++;
	m_nEndRow++;
	return m_nTotalRow;
}

void ListCtrl::OnDraw( HDC hdc, const RECT& rcPaint )
{
	Widget::OnDraw(hdc, rcPaint);

	Cell* pCell = NULL;
	RECT rcCell = {0};
	DWORD dwState = WCS_NORMAL;
	std::wstring strCellText;
	for (LONG nRow = m_nStartRow; nRow < m_nEndRow; nRow++)
	{
		for (LONG nCol = m_nStartCol; nCol < m_nEndCol; nCol++)
		{
			pCell = GetCell(nRow, nCol);
			if (pCell == NULL)
			{
				continue;
			}
			CellID cellID(nRow, nCol);
			if (m_cellSelected == cellID)
			{
				dwState |= WCS_SELECTED;
			}
			else
			{
				dwState = WCS_NORMAL;
			}
			rcCell = GetCellRect(nRow, nCol);
			pCell->Draw(hdc, rcCell, dwState, StrFormat(L"ROW:%d COL:%d", nRow, nCol), RGB(255, 0, 0),
				DT_VCENTER | DT_SINGLELINE | DT_CENTER);
		}
	}
}

ULONG ListCtrl::GetTotalRows() const
{
	return m_nTotalRow;
}

ULONG ListCtrl::GetTotalColumns() const
{
	return m_pHeadCtrl->GetTotalColumns();
}

LRESULT ListCtrl::OnHeadSizeChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_rgRect.clear();
	ULONG nTotalRow = GetTotalRows();
	ULONG nTotalCol = GetTotalColumns();
	ULONG nSeqBarWidth = GetSeqBarWidth();
	ULONG nHeadHeight = GetHeadHeight();
	RECT rc = GetDrawRect();
	RECT rcCell = rc;
	ULONG nWidth = 0;
	ULONG nHeight = 0;

	rcCell.left += nSeqBarWidth + 1;
	rcCell.top += nHeadHeight + 1;
	for (ULONG nRow = 0; nRow < nTotalRow; nRow++)
	{
		nHeight = GetRowHeight(nRow);
		rcCell.bottom = rcCell.top + nHeight;
		if (rcCell.bottom > rc.bottom)
			rcCell.bottom = rc.bottom;
		for (ULONG nCol = 0; nCol < nTotalCol; nCol++)
		{
			nWidth = GetColumnWidth(nCol);
			rcCell.right = rcCell.left + nWidth;
			if (rcCell.right > rc.right)
				rcCell.right = rc.right;
			m_rgRect.insert(std::make_pair(CellID(nRow, nCol), rcCell));
			rcCell.left += rcCell.right - rcCell.left;
		}
		rcCell.left = rc.left + nSeqBarWidth + 1;
		rcCell.top += rcCell.bottom - rcCell.top;
	}
	return 1;
}

LRESULT ListCtrl::OnHScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CalcPos(SB_HORZ, (BOOL)wParam);
	InvalidWid();
	return 1;
}

LRESULT ListCtrl::OnVScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CalcPos(SB_VERT, (BOOL)wParam);
	InvalidWid();
	return 1;
}

ULONG ListCtrl::GetRowHeight( ULONG nRow ) const
{
	return 20;
}

BOOL ListCtrl::SetRowHeight( ULONG nRow, ULONG cy )
{
	return TRUE;
}

ULONG ListCtrl::GetSeqBarWidth() const
{
	return 0;
}

ULONG ListCtrl::GetHeadHeight() const
{
	return m_nHeadHeight;
}

LONG ListCtrl::GetFixedRow() const
{
	return m_nFixedRow;
}

void ListCtrl::SetFixedRow( LONG nRow )
{
	m_nFixedRow = nRow;
}

LONG ListCtrl::GetFixedCol() const
{
	return m_nFixedCol;
}

void ListCtrl::SetFixedCol( LONG nCol )
{
	m_nFixedCol = nCol;
}

RECT ListCtrl::GetCellRect( ULONG nRow, ULONG nCol )
{
	RECT rcCell = {0};
	CellID cellID(nRow, nCol);
	if (cellID.IsValidID())
	{
		return m_rgRect[cellID];
	}
	return rcCell; 
}

LRESULT ListCtrl::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	std::pair<CellID, RECT> cellID;
	if (!GetCellID(pt, cellID))
	{
		return 0;
	}

	m_cellSelected = cellID.first;

	Cell* pCell = GetCell(cellID.first.m_nRow, cellID.first.m_nCol);
	if (pCell != NULL)
	{
		pCell->SetRect(cellID.second);
		return pCell->SendWidMessage(WM_LBUTTONUP, wParam, lParam);
	}

	return 0;
}

LRESULT ListCtrl::OnQueryVirtualSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SIZE sz = {0};
	int nTotalRow = GetTotalRows();
	for (int i = 0; i < nTotalRow; i++)
	{
		sz.cy += GetRowHeight(i);
	}
	int nTotalCol = GetTotalColumns();
	for (int i = 0; i < nTotalCol; i++)
	{
		sz.cx += GetColumnWidth(i);
	}
	return MAKELRESULT(sz.cx, sz.cy);
}

void ListCtrl::CalcCol()
{
	m_nStartCol = m_nEndCol = 0;

	RECT rcDraw = GetDrawRect();

	LONG nOffset = GetHOffset();

	BOOL bAdd = TRUE;

	if (nOffset > 0)
	{
		bAdd = FALSE;
	}

	nOffset = labs(nOffset);

	ULONG nTotalCol = GetTotalColumns();

	LONG nSumWidth = 0;
	for (ULONG nCol = 0; nCol < nTotalCol; nCol++)
	{
		if (nSumWidth >= nOffset)
		{
			m_nStartCol = nCol;
			break;
		}
		nSumWidth += GetColumnWidth(nCol);
	}

	nSumWidth = 0;

	LONG nViewWidth = rcDraw.right - rcDraw.left;
	for (ULONG nCol = m_nStartCol; nCol < nTotalCol; nCol++)
	{
		if (nSumWidth > nViewWidth)
		{
			m_nEndCol = nCol;
			break;
		}
		nSumWidth += GetColumnWidth(nCol);

	}

	/*if (bAdd)
	{
		GetColumnWidth()
	}
	else
	{

	}*/
}

void ListCtrl::CalcRow()
{

}


void ListCtrl::CalcPos(int nBar, BOOL bFurther)
{
	ScrollBar* pScrollBar = GetScrollBar(nBar);

	if (pScrollBar == NULL)
	{
		return;
	}

	if (nBar == SB_HORZ)
	{
		if (bFurther)
			SetHOffset(GetHOffset() - pScrollBar->GetPos());
		else
			SetHOffset(GetHOffset() + pScrollBar->GetPos());
		CalcCol();
	}
	else
	{
		if (bFurther)
			SetVOffset(GetVOffset() - pScrollBar->GetPos());
		else
			SetVOffset(GetVOffset() + pScrollBar->GetPos());
		CalcRow();
	}
}