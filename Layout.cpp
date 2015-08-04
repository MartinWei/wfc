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
#include "wfxlayout.h"

USING_NAMESPACE_WFX;

Layout::Layout()
{

}

BOOL Layout::ProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	return TRUE;
}

void Layout::DoLayout( Widget* pWid, Desc* pDesc, const Rect& rcGrid )
{

}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Grid::Grid( CPLayoutFactory& pFactory )
: m_pLayoutFactory(pFactory)
{

}

void Grid::AddWidget( Widget* pWidget, CLayoutDesc& desc )
{
	m_rgWidLayout.push_back(std::make_pair(pWidget, desc));
}

ULONG Grid::GetWidLayout( Wid2DescList& rgWidLayout )
{
	rgWidLayout = m_rgWidLayout;
	return rgWidLayout.size();
}

void Grid::DoLayout()
{
	if (m_pLayout == NULL)
	{
		m_pLayout = m_pLayoutFactory->CreateObject();
	}
	Rect rcGrid = GetRect();
	ULONG nItems = m_rgWidLayout.size();
	for (ULONG i = 0; i < nItems; i++)
	{
		m_pLayout->DoLayout(m_rgWidLayout[i].first, 
			&(m_rgWidLayout[i].second), rcGrid);
	}
}

LRESULT Grid::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	ULONG nItems = m_rgWidLayout.size();
	for (ULONG i = 0; i < nItems; i++)
	{
		WFX_CONDITION(m_rgWidLayout[i].first != NULL);
		m_rgWidLayout[i].first->Create(rc, m_pDispatch, this);
	}
	return 1;
}

LRESULT Grid::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	DoLayout();
	return 1;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
LayoutDispatch::LayoutDispatch()
{

}

void LayoutDispatch::SetGrid( Grid* pGrid )
{
	m_pRoot = pGrid;
}

Grid* LayoutDispatch::GetGrid() const
{
	return m_pRoot;
}

LRESULT LayoutDispatch::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
						 BOOL& bHandled)
{
	WFX_CONDITION(m_pRoot != NULL);
	if (m_pRoot != NULL)
	{
		Rect rc;
		m_pRoot->Create(rc, this);
	}
	return 1;
}

LRESULT LayoutDispatch::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
					   BOOL& bHandled)
{
	WFX_CONDITION(m_pRoot != NULL);
	if (m_pRoot != NULL)
	{
		WFX_CONDITION(GetHwnd() != NULL);
		Rect rcClient;
		::GetClientRect(GetHwnd(), &rcClient);
		m_pRoot->SetRect(rcClient);
	}
	return 1;
}

