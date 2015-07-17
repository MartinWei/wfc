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

struct WFX_API HeaderInfo
{
	ULONG						cx;
	std::wstring				strText;
	WfxImagePtr					pImage;
	DWORD						dwFormat;
	ULONG						nOrder;
	DWORD						dwState;
	RECT						rcPos;
	HeaderInfo()
		: cx(0)
		, strText(L"HeaderInfo")
		, dwFormat(DT_CENTER | DT_SINGLELINE | DT_VCENTER)
		, nOrder(0)
		, dwState(WCS_NORMAL)
	{
		memset(&rcPos, 0, sizeof(RECT));
	}
	void SetImage(const std::wstring& strFile)
	{
		pImage.reset(Gdiplus::Image::FromFile(strFile.c_str()));
	}
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
	ULONG InsertItem(ULONG nPos, const HeaderInfo& hdi);
	BOOL RemoveItem(ULONG nPos);
protected:
	virtual void OnDraw(HDC hdc, const RECT& rcPaint);
	Cell* GetCell(ULONG nCol);
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
public:
	LONG m_nRow;
	LONG m_nCol;
};

class WFX_API ListCtrl : public Widget
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
		WFX_MESSAGE_HANDLER(WM_MOUSEHWHEEL, OnMouseWheel)
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
public:
	HeaderCtrl* GetHeaderCtrl() const;
	void SetHeaderCtrl(HeaderCtrl* pHeader);
	Cell* GetCell(ULONG nRow, ULONG nCol);
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
	int InsertColumn(const std::wstring& strName, 
		UINT nWidth, DWORD dwFormat, ULONG nCellType, BOOL bAdjust = FALSE);
	int InsertRow(int nHeight = -1, BOOL bAdjust = FALSE);
protected:
	BOOL GetCellID( POINT pt, std::pair<CellID, RECT>& cellID);
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
	ULONG m_nTotalRow;
	ULONG m_nRowNumBarWidth;
	LONG m_nStartRow;
	LONG m_nEndRow;
	LONG m_nStartCol;
	LONG m_nEndCol;
	LONG m_nFixedRow;
	LONG m_nFixedCol;
	CellID m_cellSelected;
};

class WFX_API GridCtrl : public ListCtrl
{

};

class WFX_API TreeNode : public WidgetBase
{

};

class WFX_API TreeCtrl : public Widget
{
};

class WFX_API RichCtrl : public Widget
{

};

END_NAMESPACE_WFX
