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
	ULONG						cxy;
	std::wstring				strText;
	WfxImagePtr					pImage;
	DWORD						dwFormat;
	ULONG						nOrder;
	DWORD						dwState;
	RECT						rcPos;
	HeaderInfo()
		: cxy(0)
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
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual void Draw(HDC hdc,const RECT& rcPaint, DWORD dwState,
		const std::wstring& strText, COLORREF clrText, DWORD dwFormat);
protected:
	Widget* m_pParent;
};

class WFX_API HeadCell : public Cell
{
protected:
	
};

class WFX_API HeaderCtrl : public Widget
{
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
		//WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	//wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		//BOOL& bHandled);
	LONG GetSelected(POINT pt);
	LONG GetSelected() const;
	void SetSelected(LONG nSelected);
	BOOL IsAscendSort() const;
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	std::vector<SharedPtr<HeaderInfo> > m_rgpHdi;
	LONG m_nSelected;
	BOOL m_bAscendSort;
};

class WFX_API ListCtrl : public Widget
{
public:
	ListCtrl();
	virtual ~ListCtrl();
public:
	WFX_BEGIN_MSG_MAP(ListCtrl)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WFXM_LC_HITCOL, OnColChanged)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnColChanged(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
public:
	ULONG GetColumnWidth(ULONG nCol) const;
	BOOL SetColumnWidth(ULONG nCol, ULONG cx);
	HeaderCtrl* GetHeaderCtrl() const;
	void SetHeaderCtrl(HeaderCtrl* pHeader);
	std::wstring GetItemText(ULONG nItem, ULONG nSubItem) const;
	BOOL SetItemText(ULONG nItem, ULONG nSubItem, 
		const std::wstring& strText);
	int InsertColumn(const std::wstring& strName, 
		UINT nWidth, DWORD dwFormat, ULONG nCellType);
	Cell* GetCell(ULONG nRow, ULONG nCol);

protected:
	SharedPtr<HeaderCtrl> m_pHeadCtrl;
	ULONG m_nHeadHeight;
	ULONG m_nTotalRow;
};

END_NAMESPACE_WFX
