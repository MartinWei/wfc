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

#pragma once

#include "wfx.h"

BEGIN_NAMESPACE_WFX

class ListCtrl;
class TreeCtrl;
class TNode;

struct WFX_API HeaderInfo
{
	ULONG						cx;
	std::wstring				strText;
	WfxImagePtr					pImage;
	DWORD						dwFormat;
	ULONG						nOrder;
	DWORD						dwState;
	RECT						rcPos;
	ULONG						m_nType;
	HeaderInfo()
		: cx(0)
		, strText(L"HeaderInfo")
		, dwFormat(DT_CENTER | DT_SINGLELINE | DT_VCENTER)
		, nOrder(0)
		, dwState(WCS_NORMAL)
		, m_nType(WCT_TEXT)
	{
		memset(&rcPos, 0, sizeof(RECT));
	}
	void SetImage(const std::wstring& strFile)
	{
		pImage.reset(Gdiplus::Image::FromFile(strFile.c_str()));
	}
};

typedef SharedPtr<TNode> PTNode;

class WFX_API TNode /*: public WidgetBase*/
{
public:
	TNode();
public:
	ULONG AddChild(const PTNode& pNode);
	ULONG InsertChild(ULONG nPos, const PTNode& pNode);
	ULONG GetChildren(std::vector<PTNode>& rgChilren) const;
	ULONG GetChildren() const;
	BOOL HasChild() const;
	ULONG GetMyLayer() const;
	BOOL HasLayers() const;
	TNode* GetAt(ULONG nItem, BOOL bItemExpanded = TRUE);
	TNode* operator[](ULONG nItem);
	TNode* GetParent() const;
	void SetParent(TNode* pParent);
	BOOL IsExpanded() const;
	void Expand(BOOL bExpand = TRUE);
	BOOL IsExpanded(ULONG nItem, BOOL bItemExpanded = TRUE);
	ULONG Expand(ULONG nItem, BOOL bItemExpanded = TRUE, BOOL bExpand = TRUE);
	ULONG GetItems(BOOL bItemExpanded = TRUE) const;
protected:
	ULONG GetItems_All(const TNode* pNode) const;
	ULONG GetItems_Expanded(const TNode* pNode) const;
	TNode* GetAt_All(ULONG nItem) const;
	TNode* GetAt_Expanded(ULONG nItem);
	TNode* GetAt_Expanded(ULONG nItem, ULONG& nItems, TNode* pNode);
protected:
	TNode* m_pParent;
	BOOL m_bExpanded;
	std::vector<PTNode> m_rgpChildren;
};

class WFX_API Cell : public WidgetBase
{
public:
	Cell();
	virtual ~Cell();
public:
	WFX_BEGIN_MSG_MAP(Cell)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual void Draw(HDC hdc,const RECT& rcPaint, DWORD dwState,
		const std::wstring& strText, COLORREF clrText, DWORD dwFormat);
public:
	void SetParent(Widget* pParent);
	void SetRect(const RECT& rc);
protected:
	Widget* m_pParent;
	RECT m_rc;
};

class WFX_API LayerCell : public Cell
{
public:
	virtual void Draw(HDC hdc,const RECT& rcPaint, DWORD dwState,
		const std::wstring& strText, COLORREF clrText, DWORD dwFormat);
public:
	WFX_BEGIN_MSG_MAP(LayerCell)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_CHAIN_MSG_MAP(Cell)
		WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
};

class WFX_API HeadCell : public Cell
{
	
};

class WFX_API InPlaceCell : public Cell
{

};

class WFX_API TextCell : public InPlaceCell
{

};

class WFX_API ComboBoxCell : public InPlaceCell
{

};

class WFX_API CalendarCell : public InPlaceCell
{

};

class WFX_API LinkCell : public Cell
{

};

class WFX_API HeaderCtrl : public Widget
{
	friend class ListCtrl;
public:
	HeaderCtrl();
public:
	ULONG GetItemCount() const;
	BOOL GetItemRect(ULONG nIndex, RECT& rc);
	ULONG AddItem(const HeaderInfo& hdi);
	ULONG InsertItem(ULONG nPos, const HeaderInfo& hdi);
	BOOL RemoveItem(ULONG nPos);
protected:
	virtual void OnDraw(HDC hdc, const RECT& rcPaint);
	Cell* GetCell(ULONG nCol);
public:
	ULONG GetCellType(ULONG nCol) const;
public:
	WFX_BEGIN_MSG_MAP(HeaderCtrl)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
protected:
	LONG GetSelected() const;
	void SetSelected(LONG nSelected);
	BOOL IsAscendSort() const;
	ULONG GetTotalColumns() const;
	ULONG GetColumnWidth(ULONG nCol) const;
	LONG GetSelected(POINT pt);
	void CalcCellRect();
public:
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	std::vector<SharedPtr<HeaderInfo> > m_rgpHdi;
	RECT m_rgRowNumRect;
	LONG m_nSelected;
	BOOL m_bAscendSort;
};

class WFX_API CellID
{
public:
	CellID(LONG nRow = -1, LONG nCol = -1);
public:
	BOOL IsValidID() const;
	bool operator()(const CellID& id1, const CellID& id2) const;
	bool operator<(const CellID& other) const;
	bool operator==(const CellID& other) const;
	bool operator!=(const CellID& other) const;
	void Clear();
public:
	LONG m_nRow;
	LONG m_nCol;
};

class WFX_API VorticalLayerCtrl : public Widget
{
public:
	VorticalLayerCtrl();
protected:
	ULONG AddItem();
	ULONG InsertItem(ULONG nItem);
	ULONG InsertSubItem(ULONG nItem);
	ULONG Expand(ULONG nItem, BOOL bExpand = TRUE);
	BOOL IsExpanded(ULONG nItem) const;
	ULONG GetLayer(ULONG nItem) const;
	ULONG GetTotalItems() const;
protected:
	ULONG m_nTotalItems;
	PTNode m_pRoot;
};

class WFX_API ListCtrl : public VorticalLayerCtrl
{
public:
	ListCtrl();
	virtual ~ListCtrl();
public:
	WFX_BEGIN_MSG_MAP(ListCtrl)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WUM_LC_GET_PROPERTIES, OnGetProperties)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WUM_LC_HITCOL, OnColChanged)
		WFX_MESSAGE_HANDLER(WUM_LC_HEADSIZE, OnHeadSizeChanged)
		WFX_MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		WFX_MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		WFX_MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_MESSAGE_HANDLER(WUM_LC_CELL_DRAW, OnCellDraw)
		WFX_MESSAGE_HANDLER(WUM_LC_CELL_EXPAND, OnCellExpand)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnColChanged(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnHeadSizeChanged(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnGetProperties(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnCellDraw(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnCellExpand(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	HeaderCtrl* GetHeaderCtrl() const;
	void SetHeaderCtrl(HeaderCtrl* pHeader);
	Cell* GetCell(ULONG nRow, ULONG nCol);
	Cell* GetCell(const CellID& cellID);
	Cell* GetCell(ULONG nCellType);
	BOOL GetCellIndirect(const CellID& cellID, 
		std::pair<Cell*, RECT>& cell);
	RECT GetCellRect(ULONG nRow, ULONG nCol);
public:
	ULONG GetColumnWidth(ULONG nCol) const;
	BOOL SetColumnWidth(ULONG nCol, ULONG cx);
	ULONG GetRowHeight(ULONG nRow) const;
	BOOL SetRowHeight(ULONG nRow, ULONG cy);
	std::wstring GetItemText(ULONG nItem, ULONG nSubItem) const;
	BOOL SetItemText(ULONG nItem, ULONG nSubItem, 
		const std::wstring& strText);
public:
	ULONG GetTotalRows() const;
	ULONG GetTotalColumns() const;

	ULONG GetRowNumBarWidth() const;
	void SetRowNumBarWidth(ULONG nWidth);
	ULONG GetHeadHeight() const;
	void SetHeadHeight(ULONG nHeight);
	LONG GetFixedRow() const;
	void SetFixedRow(LONG nRow);
	LONG GetFixedCol() const;
	void SetFixedCol(LONG nCol);
	LONG GetStartRow() const;
	void SetStartRow(LONG nRow);
	LONG GetEndRow() const;
	void SetEndRow(LONG nRow);
	LONG GetStartCol() const;
	void SetStartCol(LONG nCol);
	LONG GetEndCol() const;
	void SetEndCol(LONG nCol);
public:
	ULONG AddColumn(const std::wstring& strName, 
		UINT nWidth, DWORD dwFormat, ULONG nCellType, BOOL bAdjust = FALSE);
	ULONG InsertColumn(ULONG nCol, const std::wstring& strName, 
		UINT nWidth, DWORD dwFormat, ULONG nCellType, BOOL bAdjust = FALSE);
	ULONG AddRow(LONG nHeight = -1, BOOL bAdjust = FALSE);
	ULONG InsertRow(ULONG nRow, LONG nHeight = -1, BOOL bAdjust = FALSE);
	ULONG InsertSubRow(ULONG nRow, LONG nHeight = -1, BOOL bAdjust = FALSE);
	ULONG Expand(ULONG nRow, BOOL bExpand = TRUE, BOOL bAdjust = FALSE);
	BOOL IsExpanded(ULONG nRow) const;
protected:
	BOOL GetCellID( POINT pt, std::pair<CellID, RECT>& cellID);
	BOOL IsValidCellID(const CellID& cellID) const;
protected:
	virtual void OnDraw(HDC hdc, const RECT& rcPaint);
protected:
	virtual SIZE EstimateVirualSize();
protected:
	BOOL CalcCol();
	BOOL CalcRow();
	void CalcCellRect();
	BOOL CalcPos(int nBar, BOOL bFurther);
protected:
	SharedPtr<HeaderCtrl> m_pHeadCtrl;
	std::map<CellID, RECT> m_rgRect;
	std::vector<RECT> m_rgRowNumRect;
	ULONG m_nHeadHeight;
	ULONG m_nRowNumBarWidth;
	LONG m_nStartRow;
	LONG m_nEndRow;
	LONG m_nStartCol;
	LONG m_nEndCol;
	LONG m_nFixedRow;
	LONG m_nFixedCol;
	CellID m_cellSelected;
	CellID m_cellMouseMove;
	BOOL m_bHasSubItem;
	std::vector<std::vector<RECT> > m_rgRectFast;
};


class WFX_API TreeCtrl : public VorticalLayerCtrl
{
};

class WFX_API RichCtrl : public Widget
{

};

END_NAMESPACE_WFX
