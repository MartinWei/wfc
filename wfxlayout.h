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

class MsgMap;
class Widget;
class WidDispatch;
class Grid;
class Layout;

class Layout : public MsgMap
{
	WFX_DECLARE_FACTORY(Layout);

protected:
	Layout();
public:
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
public:
	struct Desc
	{
		INT		m_nPos;
		Rect	m_rcMargin;
	};
public:
	virtual void DoLayout(Widget* pWid, Desc* pDesc, const Rect& rcGrid);

};

typedef SharedPtr<Layout>				PLayout;

typedef Layout::Desc					LayoutDesc;
typedef const Layout::Desc				CLayoutDesc;
typedef std::pair<Widget*, LayoutDesc>	Wid2Desc;
typedef std::vector<Wid2Desc>			Wid2DescList;

typedef Factory<Layout>					LayoutFactory;
typedef SharedPtr<LayoutFactory>		PLayoutFactory;
typedef const PLayoutFactory			CPLayoutFactory;

class WFX_API Grid : public Widget
{
public:
	Grid(CPLayoutFactory& pFactory);
public:
	void AddWidget(Widget* pWidget, CLayoutDesc& desc);
	ULONG GetWidLayout(Wid2DescList& rgWidLayout);
	void DoLayout();
	WFX_BEGIN_MSG_MAP(Grid)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
	BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	Wid2DescList m_rgWidLayout;
	PLayout m_pLayout;
	PLayoutFactory m_pLayoutFactory;
};

typedef SharedPtr<Grid> PGrid;

class WFX_API LayoutDispatch : public WidDispatch
{
public:
	LayoutDispatch();
public:
	void SetGrid(Grid* pGrid);
	Grid* GetGrid() const;
public:
	WFX_BEGIN_MSG_MAP(LayoutDispatch)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(WidDispatch)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	Grid* m_pRoot;
};

typedef SharedPtr<LayoutDispatch> PLayoutDispatch;

END_NAMESPACE_WFX
