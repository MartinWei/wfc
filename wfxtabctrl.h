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

class Widget;
class UnitBase;

class WFX_API TabItem : public UnitBase
{
	friend class TabCtrl;
public:
	TabItem(const ULONG nTabID, const String& strLabel, 
		Widget* pWid, Widget* pParent = NULL);
	void Draw(HDC hdc, const Rect& rcPaint, BOOL bSelected = FALSE);
protected:
	const ULONG m_nTabID;
	BOOL m_bVisible;
	Widget* m_pWid;
};

typedef SharedPtr<TabItem> PTabItem;

class WFX_API TabCtrl : public Widget
{
public:
	TabCtrl();
	enum Location
	{
		LOCACTION_BOTTOM = 0,
		LOCACTION_TOP = 1
	};
public:
	WFX_BEGIN_MSG_MAP(TabCtrl)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WUM_LBUTTONCLICK, OnLButtonClik)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	void SetTabHeight(ULONG nTabHeight);
	ULONG GetTabHeight() const;
	Rect GetTabRect() const;
	ULONG AddTab(Widget* pTabWid, const String& strTabLabel);
	ULONG InsertTab(Widget* pTabWid, 
		const String& strTabLabel, ULONG nInsertAt);
	ULONG RemoveTab(ULONG nTab, BOOL bRecalcLayout = TRUE);
	void RemoveAllTabs();
	ULONG GetVisibleTabCount() const;
	BOOL ShowTab(ULONG nTab, BOOL bShow = TRUE, 
		BOOL bRecalcLayout = TRUE, BOOL bActivate = TRUE);
	ULONG GetTabCount() const;
	String GetTabLabel(ULONG nTab) const;
	void SetTabLabel(ULONG nTab, const String& strTabLabel);
	void RecalcLayout();
	ULONG GetTabPt(const Point& pt) const;
	BOOL IsValidTab(ULONG nTab) const;
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonClik(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	int m_nLocation;
	std::vector<PTabItem> m_rgpTabs;
	ULONG m_nTabHeight;
	ULONG m_nTabWidth;
	ULONG m_nNextTabID;
	ULONG m_nCurrentTab;
	Rect m_rcActivate;
	Rect m_rcTabArea;
};

END_NAMESPACE_WFX
