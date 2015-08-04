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
TNode::TNode()
: m_pParent(NULL)
, m_bExpanded(TRUE)
{

}

ULONG TNode::AddChild(const PTNode& pNode)
{
	WFX_CONDITION(pNode != NULL);
	pNode->SetParent(this);
	m_rgpChildren.push_back(pNode);
	return m_rgpChildren.size();
}

ULONG TNode::InsertChild(ULONG nPos, const PTNode& pNode)
{
	BOOL bCondition = nPos <= m_rgpChildren.size();
	WFX_CONDITION(bCondition);
	if (bCondition)
	{
		m_rgpChildren.insert(m_rgpChildren.begin() + nPos, pNode);
		return m_rgpChildren.size();
	}
	return 0;
}

ULONG TNode::GetChildren(std::vector<PTNode>& rgpChilren) const
{
	rgpChilren = m_rgpChildren;
	return rgpChilren.size();
}

ULONG TNode::GetChildren() const
{
	return m_rgpChildren.size();
}

BOOL TNode::HasChild() const
{
	return m_rgpChildren.size() > 0;
}

ULONG TNode::GetMyLayer() const
{
	ULONG nLayer = 0;
	if (m_pParent == NULL)
	{
		return nLayer;
	}
	nLayer++;
	return nLayer + m_pParent->GetMyLayer();
}

BOOL TNode::HasLayers() const
{
	for (std::vector<PTNode>::const_iterator it =
		m_rgpChildren.begin(); it != m_rgpChildren.end(); ++it)
	{
		if ((*it)->HasChild())
		{
			return TRUE;
		}
	}
	return FALSE;
}

TNode* TNode::GetAt(ULONG nItem, BOOL bItemExpanded)
{
	return bItemExpanded?
		GetAt_Expanded(nItem) : GetAt_All(nItem);
}

TNode* TNode::operator[](ULONG nItem)
{
	return GetAt(nItem, TRUE);
}

TNode* TNode::GetParent() const
{
	return m_pParent;
}

void TNode::SetParent(TNode* pParent)
{
	m_pParent = pParent;
}

BOOL TNode::IsExpanded() const
{
	return m_bExpanded;
}

void TNode::Expand(BOOL bExpand /*= TRUE*/)
{
	m_bExpanded = bExpand;
}

BOOL TNode::IsExpanded(ULONG nItem, BOOL bItemExpanded /*= TRUE*/)
{
	TNode* pNode = GetAt(nItem, bItemExpanded);
	if (pNode != NULL)
	{
		return pNode->IsExpanded();
	}
	return FALSE;
}

ULONG TNode::Expand(ULONG nItem, BOOL bItemExpanded /*= TRUE*/, BOOL bExpand /*= TRUE*/)
{
	TNode* pNode = GetAt(nItem, bItemExpanded);
	ULONG nNodeItems = 0;
	if (pNode != NULL)
	{
		if (!bExpand)
		{
			nNodeItems = pNode->GetItems(bItemExpanded);
		}
		pNode->Expand(bExpand);
		if (bExpand)
		{
			nNodeItems = pNode->GetItems(bItemExpanded);
		}
		return nNodeItems;
	}
	return 0;
}

ULONG TNode::GetItems(BOOL bItemExpanded /*= TRUE*/) const
{
	return bItemExpanded? 
		GetItems_Expanded(this) : GetItems_All(this);
}

ULONG TNode::GetItems_All(const TNode* pNode) const
{
	ULONG nItems = 1;
	for (std::vector<PTNode>::const_iterator it = 
		m_rgpChildren.begin();
		it != m_rgpChildren.end(); ++it)
	{
		nItems += GetItems_All((*it).get());
	}
	return nItems;
}

ULONG TNode::GetItems_Expanded(const TNode* pNode) const
{
	ULONG nItems = 0;
	if (pNode->IsExpanded())
	{
		std::vector<PTNode> rgpChilren;
		pNode->GetChildren(rgpChilren);
		for (std::vector<PTNode>::const_iterator it = 
			rgpChilren.begin(); it != rgpChilren.end(); ++it)
		{
			nItems++;
			nItems += GetItems_Expanded((*it).get());
		}
	}
	return nItems;
}

TNode* TNode::GetAt_All(ULONG nItem) const
{
	ULONG nItems = 1;
	for (std::vector<PTNode>::const_iterator it = 
		m_rgpChildren.begin();
		it != m_rgpChildren.end(); ++it)
	{
		if (nItems - 1 == nItem)
		{
			return (*it).get();
		}
		nItems += GetItems_All((*it).get());
	}
	return NULL;
}

TNode* TNode::GetAt_Expanded(ULONG nItem)
{
	ULONG nItems = 0;
	return GetAt_Expanded(nItem, nItems, this);
}

TNode* TNode::GetAt_Expanded(ULONG nItem, ULONG& nItems, TNode* pNode)
{
	TNode* pResultNode = NULL;
	if (pNode->IsExpanded())
	{
		std::vector<PTNode> rgpChilren;
		pNode->GetChildren(rgpChilren);
		for (std::vector<PTNode>::const_iterator it = 
			rgpChilren.begin(); it != rgpChilren.end(); ++it)
		{
			nItems++;
			if (nItems - 1 == nItem)
			{
				pResultNode = (*it).get();
				break;
			}
			pResultNode = GetAt_Expanded(nItem, nItems, (*it).get());
			if (pResultNode != NULL)
			{
				break;
			}
		}
	}
	else
	{
		if (nItems - 1 == nItem)
		{
			return pNode;
		}
	}
	return pResultNode;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Cell::Cell()
{

}

LRESULT Cell::OnMouseMove( UINT uMsg, 
						  WPARAM wParam, 
						  LPARAM lParam,
						  BOOL& bHandled )
{
	return SendParentMessage(WUM_LC_CELL_DRAW, wParam);
}

LRESULT Cell::OnMouseLeave( UINT uMsg, 
						   WPARAM wParam, 
						   LPARAM lParam, 
						   BOOL& bHandled )
{
	return SendParentMessage(WUM_LC_CELL_DRAW, wParam);
}

LRESULT Cell::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return SendParentMessage(WUM_LC_CELL_DRAW, wParam);
}

void Cell::Draw( HDC hdc, const Rect& rcPaint,  DWORD dwState,
				const String& strText, COLORREF clrText, DWORD dwFormat )
{
	WfxRender::DrawHeadCell(hdc, rcPaint, dwState, strText,
		clrText, dwFormat);
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
void LayerCell::Draw(HDC hdc,const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat)
{
	WfxRender::DrawLayerCell(hdc, rcPaint, dwState, strText, clrText, dwFormat);
}

LRESULT LayerCell::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled)
{
	return SendParentMessage(WUM_LC_CELL_EXPAND, wParam);;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ULONG HeaderCtrl::GetItemCount() const
{
	return m_rgpHdi.size();
}

BOOL HeaderCtrl::GetItemRect( ULONG nIndex, 
							 Rect& rc )
{
	return TRUE;
}

ULONG HeaderCtrl::AddItem(const HeaderInfo& hdi)
{
	PHeaderInfo pHdi(new HeaderInfo(hdi));
	m_rgpHdi.push_back(pHdi);
	return m_rgpHdi.size();
}

ULONG HeaderCtrl::InsertItem( ULONG nPos, 
							 const HeaderInfo& hdi )
{
	WFX_CONDITION(nPos < m_rgpHdi.size());
	if (nPos < m_rgpHdi.size())
	{
		PHeaderInfo pHdi(new HeaderInfo(hdi));
		m_rgpHdi.insert(m_rgpHdi.begin() + nPos, pHdi);
		return m_rgpHdi.size();
	}
	return 0;
}

BOOL HeaderCtrl::RemoveItem( ULONG nPos )
{
	return FALSE;
}

void HeaderCtrl::OnDraw( HDC hdc, const Rect& rcPaint )
{
	LONG nStartCol = SendParentMessage(WUM_LC_GET_PROPERTIES, LCP_START_COL);
	LONG nEndCol = SendParentMessage(WUM_LC_GET_PROPERTIES, LCP_END_COL);
	Cell* pCell = NULL;
	DWORD dwState = WCS_NORMAL;
	pCell = GetCell(0);
	if (pCell != NULL)
	{
		pCell->Draw(hdc, m_rgRowNumRect, dwState, 
			L"SEQ", WBTN_BKGND_MOUSE, 0);
	}

	LONG nTotalCal = GetTotalColumns();
	if (nTotalCal <= 0)
	{
		return;
	}

	for (LONG nCol = nStartCol; nCol <= nEndCol; nCol++)
	{
		pCell = GetCell(nCol);
		if (pCell != NULL)
		{
			if (nCol == m_nSelected)
			{
				dwState |= WCS_SELECTED;
				m_bAscendSort? dwState |= WCS_ASORT : dwState |= WCS_NASORT;
			}
			else
			{
				dwState = WCS_NORMAL;
			}
			pCell->Draw(hdc, m_rgpHdi[nCol]->rcPos, dwState, 
				m_rgpHdi[nCol]->strText, WBTN_BKGND_MOUSE, m_rgpHdi[nCol]->dwFormat);
		}
	}
}

LRESULT HeaderCtrl::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc = GetRect();
	Rect rcItem = rc;
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
	if (!Verify())
	{
		return 1;
	}
	Point pt(lParam);
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
	LONG nStartCol = SendParentMessage(WUM_LC_GET_PROPERTIES, LCP_START_COL);
	LONG nEndCol = SendParentMessage(WUM_LC_GET_PROPERTIES, LCP_END_COL);
	Rect rcItem;
	for (LONG nCol = nStartCol; nCol <= nEndCol; nCol++)
	{
		rcItem = m_rgpHdi[nCol]->rcPos;
		if (rcItem.PtInRect(pt))
		{
			return nCol;
		}
	}
	return -1;
}

void HeaderCtrl::CalcCellRect()
{
	LONG nStartCol = SendParentMessage(WUM_LC_GET_PROPERTIES, LCP_START_COL);
	LONG nEndCol = SendParentMessage(WUM_LC_GET_PROPERTIES, LCP_END_COL);
	if (nStartCol < 0 || nEndCol < 0)
	{
		return;
	}
	ULONG nRowNumBarWidth = SendParentMessage(WUM_LC_GET_PROPERTIES, LCP_ROWNUM_WIDTH);
	Rect rcWid = GetRect();
	m_rgRowNumRect = rcWid;
	m_rgRowNumRect.right = m_rgRowNumRect.left + nRowNumBarWidth;
	Rect rcItem = rcWid;
	rcItem.left += nRowNumBarWidth;
	for (LONG nCol = nStartCol; nCol <= nEndCol; nCol++)
	{
		rcItem.right = rcItem.left + m_rgpHdi[nCol]->cx;
		m_rgpHdi[nCol]->rcPos = rcItem;
		rcItem.left += rcItem.right - rcItem.left;
	}
}

Cell* HeaderCtrl::GetCell( ULONG nCol )
{
	static Cell cell;
	return &cell;
}

ULONG HeaderCtrl::GetCellType(ULONG nCol) const
{
	WFX_CONDITION(nCol < m_rgpHdi.size());
	if (nCol < m_rgpHdi.size())
	{
		return m_rgpHdi[nCol]->m_nType;
	}
	return 0;
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

BOOL HeaderCtrl::Verify() const
{
	ULONG nTotalCol = GetTotalColumns();
	return nTotalCol > 0;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
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

bool CellID::operator!=(const CellID& other) const
{
	return !operator==(other);
}

void CellID::Clear()
{
	m_nRow = -1;
	m_nCol = -1;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
VorticalLayerCtrl::VorticalLayerCtrl()
: m_pRoot(new TNode)
, m_bCached(TRUE)
{

}

ULONG VorticalLayerCtrl::AddItem()
{
	m_pRoot->AddChild(PTNode(new TNode));
	CacheInfo chinfo(1, TRUE);
	m_rgCacheInfo.push_back(chinfo);
	return m_rgCacheInfo.size();
}

ULONG VorticalLayerCtrl::InsertItem(ULONG nItem)
{
	if (m_pRoot->InsertChild(nItem, PTNode(new TNode)) != 0)
	{
		CacheInfo chinfo(1, TRUE);
		WFX_CONDITION(nItem + 1 <= m_rgCacheInfo.size());
		m_rgCacheInfo.insert(m_rgCacheInfo.begin() + nItem + 1, chinfo);
	}
	return m_rgCacheInfo.size();
}

ULONG VorticalLayerCtrl::InsertSubItem(ULONG nItem)
{
	TNode* pNode = m_pRoot->GetAt(nItem);
	if (pNode != NULL)
	{
		pNode->AddChild(PTNode(new TNode));
		if (pNode->IsExpanded())
		{
			CacheInfo chinfo(pNode->GetMyLayer() + 1, TRUE);
			WFX_CONDITION(nItem + 1 <= m_rgCacheInfo.size());
			m_rgCacheInfo.insert(m_rgCacheInfo.begin() + nItem + 1, chinfo);
		}
	}
	return m_rgCacheInfo.size();
}
	
ULONG VorticalLayerCtrl::Expand(ULONG nItem, BOOL bExpand /*= TRUE*/)
{
	m_bCached = FALSE;
	ULONG nLine = m_pRoot->Expand(nItem, TRUE, bExpand);
	if (bExpand)
	{
		WFX_CONDITION(nItem < m_rgCacheInfo.size());
		m_rgCacheInfo[nItem].m_bExpanded = TRUE;
		for (int i = 0; i < nLine; i++)
		{
			WFX_CONDITION(nItem + i + 1 <= m_rgCacheInfo.size());
			CacheInfo chinfo(GetLayer(nItem + i + 1), TRUE);
			m_rgCacheInfo.insert(m_rgCacheInfo.begin() + nItem + 1, chinfo);
		}
	}
	else
	{
		m_rgCacheInfo[nItem].m_bExpanded = FALSE;
		for (int i = 0; i < nLine; i++)
		{
			WFX_CONDITION(nItem + i + 1 < m_rgCacheInfo.size());
			m_rgCacheInfo.erase(m_rgCacheInfo.begin() + nItem + 1);
		}
	}
	RefreshCache();
	return m_rgCacheInfo.size();
}

BOOL VorticalLayerCtrl::IsExpanded(ULONG nItem) const
{
	if (m_bCached)
	{
		WFX_CONDITION(nItem < m_rgCacheInfo.size());
		return m_rgCacheInfo[nItem].m_bExpanded;
	}
	return m_pRoot->IsExpanded(nItem);
}

ULONG VorticalLayerCtrl::GetLayer(ULONG nItem) const
{
	if (m_bCached)
	{
		WFX_CONDITION(nItem < m_rgCacheInfo.size());
		return m_rgCacheInfo[nItem].m_nLayer;
	}
	TNode* pNode = m_pRoot->GetAt(nItem);
	if (pNode != NULL)
	{
		return pNode->GetMyLayer();
	}
	return 0;
}

ULONG VorticalLayerCtrl::GetTotalItems() const
{
	return m_rgCacheInfo.size();
}

void VorticalLayerCtrl::RefreshCache()
{
	m_bCached = TRUE;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ListCtrl::ListCtrl()
: m_pHeadCtrl(new HeaderCtrl)
, m_nHeadHeight(WID_LC_HEAD_HEIGHT)
, m_nStartRow(0)
, m_nEndRow(0)
, m_nStartCol(0)
, m_nEndCol(0)
, m_nFixedRow(-1)
, m_nFixedCol(-1)
, m_nRowNumBarWidth(20)
, m_bHasSubItem(FALSE)
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

String ListCtrl::GetItemText( ULONG nItem, 
								   ULONG nSubItem ) const
{
	return L"";
}

BOOL ListCtrl::SetItemText( ULONG nItem, 
						   ULONG nSubItem, 
						   const String& strText )
{
	return FALSE;
}

LRESULT ListCtrl::OnCreate( UINT uMsg, 
						   WPARAM wParam, 
						   LPARAM lParam, 
						   BOOL& bHandled )
{
	Rect rc;
	m_pHeadCtrl->Create(rc, m_pDispatch, this);
	return 0;
}

LRESULT ListCtrl::OnSize( UINT uMsg, 
						 WPARAM wParam, 
						 LPARAM lParam, 
						 BOOL& bHandled )
{
	Rect rc = GetRect();
	rc.bottom = rc.top + m_nHeadHeight;
	rc.left += 1;
	rc.right -= 1;
	rc.top += 1;
	m_pHeadCtrl->SetRect(rc);
	BOOL bCalcRow = CalcRow();
	BOOL bCalcCol = CalcCol();
	if (bCalcRow || bCalcCol)
	{
		CalcCellRect();
		if (bCalcCol)
		{
			m_pHeadCtrl->CalcCellRect();
		}
	}
	return 0;
}

ULONG ListCtrl::AddColumn(const String& strName, 
				UINT nWidth, DWORD dwFormat, ULONG nCellType, BOOL bAdjust /*= FALSE*/)
{
	HeaderInfo hi;
	hi.cx = nWidth;
	hi.strText = strName;
	hi.m_nType = nCellType;

	ULONG nTotalCols = m_pHeadCtrl->AddItem(hi);
	if (bAdjust)
	{

	}
	return nTotalCols;
}

ULONG ListCtrl::InsertColumn(ULONG nCol, const String& strName, 
						   UINT nWidth, 
						   DWORD dwFormat, 
						   ULONG nCellType,
						   BOOL bAdjust /*= FALSE*/)
{
	HeaderInfo hi;
	hi.cx = nWidth;
	hi.strText = strName;
	hi.m_nType = nCellType;

	ULONG nTotalCols = m_pHeadCtrl->InsertItem(nCol, hi);
	if (bAdjust)
	{

	}
	return nTotalCols;
}

Cell* ListCtrl::GetCell(ULONG nCellType)
{
	switch(nCellType)
	{
	case WCT_TEXT:
		{
			static Cell cell;
			cell.SetParent(this);
			return &cell;
		}
	case WCT_Layer:
		{
			static LayerCell cell;
			cell.SetParent(this);
			return &cell;
		}
	default:
		{
			static Cell cell;
			cell.SetParent(this);
			return &cell;
		}
	}
}

Cell* ListCtrl::GetCell( ULONG nRow, ULONG nCol )
{
	return GetCell(m_pHeadCtrl->GetCellType(nCol));
}

Cell* ListCtrl::GetCell(const CellID& cellID)
{
	return GetCell(cellID.m_nRow, cellID.m_nCol);
}

BOOL ListCtrl::GetCellIndirect(const CellID& cellID, 
					 std::pair<Cell*, Rect>& cell)
{
	LONG nOffsetRow = cellID.m_nRow - m_nStartRow;
	LONG nOffsetCol = cellID.m_nCol - m_nStartCol;
	if (nOffsetRow >= m_rgRectFast.size())
	{
		return FALSE;
	}
	if (nOffsetCol >= m_rgRectFast[nOffsetRow].size())
	{
		return FALSE;
	}
	cell.first = GetCell(cellID);
	cell.second = m_rgRectFast[nOffsetRow][nOffsetCol];
	return TRUE;
}

BOOL ListCtrl::GetCellID( POINT pt, std::pair<CellID, Rect>& cellID)
{
	for(std::map<CellID, Rect>::iterator it = m_rgRect.begin();
		it != m_rgRect.end(); ++it)
	{
		if (it->second.PtInRect(pt))
		{
			cellID = (*it);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL ListCtrl::IsValidCellID( const CellID& cellID ) const
{
	return m_rgRect.find(cellID) != m_rgRect.end();
}

LRESULT ListCtrl::OnColChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	InvalidWid();
	return 1;
}

ULONG ListCtrl::AddRow(LONG nHeight /*= -1*/, BOOL bAdjust /*= FALSE*/)
{
	ULONG nTotalItems = AddItem();
	if (bAdjust)
	{

	}
	return nTotalItems;
}

ULONG ListCtrl::InsertRow( ULONG nRow, LONG nHeight /*= -1*/, BOOL bAdjust /*= FALSE*/ )
{
	ULONG nTotalItems = InsertItem(nRow);
	if (bAdjust)
	{

	}
	return nTotalItems;
}

ULONG ListCtrl::InsertSubRow(ULONG nRow, LONG nHeight /*= -1*/, BOOL bAdjust /*= FALSE*/)
{
	if (!m_bHasSubItem)
	{
		InsertColumn(0, L"Layer", 400, DT_VCENTER | DT_SINGLELINE | DT_CENTER, WCT_Layer);
		m_bHasSubItem = TRUE;
	}
	ULONG nTotalItems = InsertSubItem(nRow);
	if (bAdjust)
	{

	}
	return nTotalItems;
}

ULONG ListCtrl::Expand(ULONG nRow, BOOL bExpand /*= TRUE*/, BOOL bAdjust /*= FALSE*/)
{
	ULONG nTotalItems = __super::Expand(nRow, bExpand);
	if (1/*bAdjust*/)
	{
		CalcRow();
		CalcCellRect();
	}
	return nTotalItems;
}

BOOL ListCtrl::IsExpanded(ULONG nRow) const
{
	return __super::IsExpanded(nRow);
}

COLORREF GetColor(ULONG nLayer)
{
	ULONG nRed = 255;
	ULONG nGreen = 0;
	nRed -= (nLayer - 1) * 100;
	nGreen += (nLayer - 1) * 100;
	return RGB(nRed, nGreen, 0);
}
void ListCtrl::OnDraw( HDC hdc, const Rect& rcPaint )
{
	if (!Verify())
	{
		Rect rc = GetRect();
		WfxRender::DrawFrame(hdc, rc, WBTN_BKGND_MOUSE, m_pDispatch);
		return;
	}
	Cell* pCell = NULL;
	Rect rcCell;
	DWORD dwState = WCS_NORMAL;
	String strCellText;
	ULONG nLayer = 0;
	BOOL bRowExpanded = FALSE;
	COLORREF clrText = 0;
	LONG nLayerOffset = 0;
	LONG nTotalRow = GetTotalRows();
	LONG nTotalCol = GetTotalColumns();
	for (LONG nRow = m_nStartRow, nRowNum = 0; nRow <= m_nEndRow; nRow++, nRowNum++)
	{
		pCell = GetCell(0, 0);
		if (pCell != NULL)
		{
			strCellText.Format(L"%d", nRow);
			pCell->Draw(hdc, m_rgRowNumRect[nRowNum], 0, strCellText, WBTN_BKGND_MOUSE,
				DT_VCENTER | DT_SINGLELINE | DT_CENTER);
		}
		nLayer = GetLayer(nRow);
		nLayerOffset = (nLayer - 1) * 30;
		clrText = GetColor(nLayer);
		if (m_bHasSubItem)
		{
			bRowExpanded = IsExpanded(nRow);
		}
		for (LONG nCol = m_nStartCol; nCol <= m_nEndCol; nCol++)
		{
			pCell = GetCell(nRow, nCol);
			if (pCell == NULL)
			{
				continue;
			}
			CellID cellID(nRow, nCol);
			dwState = WCS_NORMAL;
			if (m_bHasSubItem && nCol == 0)
			{
				if (bRowExpanded)
				{
					dwState |= WCS_EXPAND;
				}
				else
				{
					dwState |= WCS_CLOSE;
				}
			}
			if (m_cellSelected == cellID)
			{
				dwState |= WCS_SELECTED;
			}
			if (m_cellMouseMove == cellID)
			{
				dwState |= WCS_MOUSEMOVE;
			}
			rcCell = GetCellRect(nRow, nCol);
			rcCell.left += nLayerOffset;
			strCellText.Format(L"Layer:%d ROW:%d COL:%d", nLayer, nRow, nCol);
			pCell->Draw(hdc, rcCell, dwState, 
				strCellText, WBTN_BKGND_MOUSE, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
		}
	}
	Rect rc = GetRect();
	WfxRender::DrawFrame(hdc, rc, WBTN_BKGND_MOUSE, m_pDispatch);
}

ULONG ListCtrl::GetTotalRows() const
{
	return GetTotalItems();
}

ULONG ListCtrl::GetTotalColumns() const
{
	return m_pHeadCtrl->GetTotalColumns();
}

LRESULT ListCtrl::OnHeadSizeChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

LRESULT ListCtrl::OnHScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (CalcPos(SB_HORZ, (BOOL)wParam))
	{
		CalcCellRect();
		m_pHeadCtrl->CalcCellRect();
	}
	InvalidWid();
	return 1;
}

LRESULT ListCtrl::OnVScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (CalcPos(SB_VERT, (BOOL)wParam))
	{
		CalcCellRect();
	}
	InvalidWid();
	return 1;
}

LRESULT ListCtrl::OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

LRESULT ListCtrl::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!Verify())
	{
		return 0;
	}
	Point pt(lParam);
	std::pair<CellID, Rect> cellpt;
	std::pair<Cell*, Rect> cell;
	CellID cellMouseLeave;
	CellID cellMouseMove;
	Cell* pCell = NULL;
	if (GetCellID(pt, cellpt))
	{
		cellMouseMove = cellpt.first;
		if (cellMouseMove != m_cellMouseMove)
		{
			cellMouseLeave = m_cellMouseMove;
			m_cellMouseMove = cellMouseMove;
			TRACE(L"cellMouseLeave.m_nRow = %d, cellMouseLeave.m_nCol = %d", cellMouseLeave.m_nRow, cellMouseLeave.m_nCol);
			TRACE(L"m_cellMouseMove.m_nRow = %d, m_cellMouseMove.m_nCol = %d", m_cellMouseMove.m_nRow, m_cellMouseMove.m_nCol);
		}
	}
	if (GetCellIndirect(cellMouseLeave, cell))
	{
		cell.first->SendWidMessage(WM_MOUSELEAVE, (WPARAM)&cell.second);
	}
	if (GetCellIndirect(m_cellMouseMove, cell))
	{
		cell.first->SendWidMessage(WM_MOUSEMOVE, (WPARAM)&cell.second);
	}
	return 1;
}

LRESULT ListCtrl::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!Verify())
	{
		return 0;
	}
	std::pair<Cell*, Rect> cell;
	if (GetCellIndirect(m_cellMouseMove, cell))
	{
		cell.first->SendWidMessage(WM_MOUSELEAVE, (WPARAM)&cell.second);
	}
	m_cellMouseMove.Clear();
	return 1;
}

LRESULT ListCtrl::OnCellDraw(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	InvalidWid();
	return 1;
}

LRESULT ListCtrl::OnCellExpand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bExpanded = IsExpanded(m_cellSelected.m_nRow);
	Expand(m_cellSelected.m_nRow, !bExpanded);
	InvalidWid();
	return 1;
}

LRESULT ListCtrl::OnGetProperties( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(wParam)
	{
	case LCP_START_ROW:
		return GetStartRow();
	case LCP_END_ROW:
		return GetEndRow();
	case LCP_START_COL:
		return GetStartCol();
	case LCP_END_COL:
		return GetEndCol();
	case LCP_ROWNUM_WIDTH:
		return GetRowNumBarWidth();
	default:
		return 0;
	}
}

ULONG ListCtrl::GetRowHeight( ULONG nRow ) const
{
	return 20;
}

BOOL ListCtrl::SetRowHeight( ULONG nRow, ULONG cy )
{
	return TRUE;
}

ULONG ListCtrl::GetRowNumBarWidth() const
{
	return m_nRowNumBarWidth;
}

void ListCtrl::SetRowNumBarWidth( ULONG nWidth )
{
	m_nRowNumBarWidth = nWidth;
}

ULONG ListCtrl::GetHeadHeight() const
{
	return m_nHeadHeight;
}

void ListCtrl::SetHeadHeight( ULONG nHeight )
{
	m_nHeadHeight = nHeight;
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

LONG ListCtrl::GetStartRow() const
{
	return m_nStartRow;
}

void ListCtrl::SetStartRow( LONG nRow )
{
	m_nStartRow = nRow;
}

LONG ListCtrl::GetEndRow() const
{
	return m_nEndRow;
}

void ListCtrl::SetEndRow( LONG nRow )
{
	m_nEndRow = nRow;
}

LONG ListCtrl::GetStartCol() const
{
	return m_nStartCol;
}

void ListCtrl::SetStartCol( LONG nCol )
{
	m_nStartCol = nCol;
}

LONG ListCtrl::GetEndCol() const
{
	return m_nEndCol;
}

void ListCtrl::SetEndCol( LONG nCol )
{
	m_nEndCol = nCol;
}

Rect ListCtrl::GetCellRect( ULONG nRow, ULONG nCol )
{
	Rect rcCell;
	LONG nOffsetRow = nRow - m_nStartRow;
	LONG nOffsetCol = nCol - m_nStartCol;
	if (nOffsetRow >= m_rgRectFast.size())
		return rcCell;
	if (nOffsetCol >= m_rgRectFast[nOffsetRow].size())
		return rcCell;
	rcCell = m_rgRectFast[nOffsetRow][nOffsetCol];
	return rcCell; 
}

LRESULT ListCtrl::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (!Verify())
	{
		return 0;
	}
	Point pt(lParam);
	std::pair<CellID, Rect> cellID;
	if (!GetCellID(pt, cellID))
	{
		return 0;
	}
	m_cellSelected = cellID.first;
	if (m_cellSelected.m_nCol == 0)
	{
	
	}
	TRACE(L"m_cellSelected.m_nRow=%d, m_cellSelected.m_nCol=%d", m_cellSelected.m_nRow, m_cellSelected.m_nCol);
	Cell* pCell = GetCell(cellID.first.m_nRow, cellID.first.m_nCol);
	if (pCell != NULL)
	{
		pCell->SetRect(cellID.second);
		return pCell->SendWidMessage(WM_LBUTTONUP, wParam, lParam);
	}
	return 0;
}

Size ListCtrl::EstimateVirualSize()
{
	Size sz;
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
	return sz;
}

BOOL ListCtrl::CalcCol()
{
	ULONG nOldStartCol = m_nStartCol;
	ULONG nOldEndCol = m_nEndCol;
	m_nStartCol = m_nEndCol = -1;
	Rect rcDraw = GetDrawRect();
	LONG nOffset = GetHOffset();
	TRACE(L"nOffset = %d", nOffset);
	ULONG nTotalCol = GetTotalColumns();
	LONG nSumWidth = 0;
	for (ULONG nCol = 0; nCol < nTotalCol; nCol++)
	{
		nSumWidth += GetColumnWidth(nCol);
		if (nSumWidth >= nOffset)
		{
			m_nStartCol = nCol;
			break;
		}
	}
	nSumWidth = 0;
	LONG nViewWidth = rcDraw.right - rcDraw.left;
	for (ULONG nCol = m_nStartCol; nCol < nTotalCol; nCol++)
	{
		nSumWidth += GetColumnWidth(nCol);
		if (nSumWidth > nViewWidth)
		{
			m_nEndCol = nCol;
			break;
		}
	}
	if (nSumWidth < nViewWidth)
	{
		m_nEndCol = nTotalCol - 1;
	}
	TRACE(L"m_nStartCol = %d", m_nStartCol);
	TRACE(L"m_nEndCol = %d", m_nEndCol);
	return (m_nStartCol != nOldStartCol || m_nEndCol != nOldEndCol);
}

BOOL ListCtrl::CalcRow()
{
	ULONG nOldStartRow = m_nStartRow;
	ULONG nOldEndRow = m_nEndRow;
	m_nStartRow = m_nEndRow = -1;
	Rect rcDraw = GetDrawRect();
	LONG nOffset = GetVOffset();
	TRACE(L"nOffset = %d", nOffset);
	ULONG nTotalRow = GetTotalRows();
	LONG nSumHeight = 0;
	for (ULONG nRow = 0; nRow < nTotalRow; nRow++)
	{
		nSumHeight += GetRowHeight(nRow);
		if (nSumHeight >= nOffset)
		{
			m_nStartRow = nRow;
			break;
		}
	}
	nSumHeight = 0;
	LONG nViewHeight = rcDraw.bottom - rcDraw.top;
	for (ULONG nRow = m_nStartRow; nRow < nTotalRow; nRow++)
	{
		nSumHeight += GetRowHeight(nRow);
		if (nSumHeight > nViewHeight)
		{
			m_nEndRow = nRow;
			break;
		}
	}

	if (nSumHeight < nViewHeight)
	{
		m_nEndRow = nTotalRow - 1;
	}
	TRACE(L"m_nStartRow = %d", m_nStartRow);
	TRACE(L"m_nEndRow = %d", m_nEndRow);
	return (m_nStartRow != nOldStartRow || m_nEndRow != nOldEndRow);
}

void ListCtrl::CalcCellRect()
{
	m_rgRectFast.clear();
	m_rgRect.clear();
	m_rgRowNumRect.clear();
	ULONG nTotalRow = GetTotalRows();
	ULONG nTotalCol = GetTotalColumns();
	ULONG nSeqBarWidth = GetRowNumBarWidth();
	ULONG nHeadHeight = GetHeadHeight();
	Rect rcWid = GetDrawRect();
	Rect rcCell = rcWid;
	Rect rcRowNum = rcWid;
	ULONG nWidth = 0;
	ULONG nHeight = 0;
	rcRowNum.right = rcRowNum.left + nSeqBarWidth;
	rcCell.left += nSeqBarWidth + 1;
	rcCell.top += nHeadHeight + 1;
	m_rgRectFast.resize(m_nEndRow - m_nStartRow + 1);
	for (LONG nRow = m_nStartRow; nRow <= m_nEndRow; nRow++)
	{
		nHeight = GetRowHeight(nRow);
		rcCell.bottom = rcCell.top + nHeight;
		rcRowNum.top = rcCell.top;
		rcRowNum.bottom = rcCell.bottom;
		m_rgRowNumRect.push_back(rcRowNum);
		if (rcCell.bottom > rcWid.bottom)
			rcCell.bottom = rcWid.bottom;
		m_rgRectFast[nRow - m_nStartRow].resize(m_nEndCol - m_nStartCol + 1);
		for (LONG nCol = m_nStartCol; nCol <= m_nEndCol; nCol++)
		{
			nWidth = GetColumnWidth(nCol);
			rcCell.right = rcCell.left + nWidth;
			if (rcCell.right > rcWid.right)
				rcCell.right = rcWid.right;
			m_rgRect.insert(std::make_pair(CellID(nRow, nCol), rcCell));
			m_rgRectFast[nRow - m_nStartRow][nCol - m_nStartCol] = rcCell;
			rcCell.left += rcCell.right - rcCell.left;
		}
		rcCell.left = rcWid.left + nSeqBarWidth + 1;
		rcCell.top += rcCell.bottom - rcCell.top;
	}
}

BOOL ListCtrl::CalcPos(int nBar, BOOL bFurther)
{
	if (nBar == SB_HORZ)
	{
		return CalcCol();
	}
	else
	{
		return CalcRow();
	}
}

BOOL ListCtrl::Verify() const
{
	ULONG nTotalRow = GetTotalRows();
	ULONG nTotalCol = GetTotalColumns();
	return nTotalRow > 0 && nTotalCol > 0;
}
