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
#include "wfxtabctrl.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
TabItem::TabItem( const ULONG nTabID, const String& strLabel, 
				 Widget* pWid, Widget* pParent /*= NULL*/ )
: UnitBase(pParent)
, m_nTabID(nTabID)
, m_bVisible(TRUE)
, m_pWid(pWid)
{
	SetText(strLabel);
}

void TabItem::Draw( HDC hdc, const Rect& rcPaint, BOOL bSelected /*= FALSE*/ )
{
	COLORREF clr = bSelected? WBTN_BKGND_MOUSE : WBTN_BKGND_STATIC;
	WfxRender::DrawSolidRect(hdc, GetRect(), clr);
	WfxRender::DrawText(hdc, GetRect(), GetText(), 
		WID_TEXT_STATIC, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	WfxRender::DrawFrame(hdc ,GetRect(), WBTN_BKGND_MOUSE);
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
TabCtrl::TabCtrl()
: m_nLocation(LOCACTION_TOP)
, m_nTabHeight(20)
, m_nNextTabID(0)
, m_nCurrentTab(0)
, m_nTabWidth(80)
{

}

void TabCtrl::SetTabHeight( ULONG nTabHeight )
{
	m_nTabHeight = nTabHeight;
}

ULONG TabCtrl::GetTabHeight() const
{
	return m_nTabHeight;
}

Rect TabCtrl::GetTabRect() const
{
	Rect rcWid = GetRect();
	return rcWid;
}

ULONG TabCtrl::AddTab( Widget* pTabWid, const String& strTabLabel )
{
	return InsertTab(pTabWid, strTabLabel, m_rgpTabs.size());
}

ULONG TabCtrl::InsertTab( Widget* pTabWid, 
						 const String& strTabLabel, ULONG nInsertAt )
{
	WFX_CONDITION(pTabWid != NULL);
	m_rgpTabs.insert(m_rgpTabs.begin() + nInsertAt, 
		PTabItem(new TabItem(m_nNextTabID, strTabLabel, pTabWid, this)));
	m_nCurrentTab = m_nNextTabID;
	m_nNextTabID++;
	return m_rgpTabs.size();
}

ULONG TabCtrl::RemoveTab( ULONG nTab, BOOL bRecalcLayout /*= TRUE*/ )
{
	WFX_CONDITION(nTab < m_rgpTabs.size());
	if (nTab < m_rgpTabs.size())
	{
		m_rgpTabs.erase(m_rgpTabs.begin() + nTab);
	}
	return m_rgpTabs.size();
}

void TabCtrl::RemoveAllTabs()
{
	m_rgpTabs.clear();
	m_nNextTabID = 0;
}

ULONG TabCtrl::GetVisibleTabCount() const
{
	Rect rcWid = GetRect();
	return rcWid.GetWidth();
}

BOOL TabCtrl::ShowTab( ULONG nTab, BOOL bShow /*= TRUE*/, 
					  BOOL bRecalcLayout /*= TRUE*/, BOOL bActivate /*= TRUE*/ )
{
	WFX_CONDITION(nTab < m_rgpTabs.size());
	if (IsValidTab(m_nCurrentTab))
	{
		Rect rcOldTab;
		m_rgpTabs[m_nCurrentTab]->m_pWid->SetRect(rcOldTab);
		m_rgpTabs[m_nCurrentTab]->m_pWid->ShowWid(SW_HIDE);
	}
	m_nCurrentTab = nTab;
	m_rgpTabs[m_nCurrentTab]->m_pWid->ShowWid(SW_SHOW);
	m_rgpTabs[m_nCurrentTab]->m_pWid->SetRect(m_rcActivate);
	RecalcLayout();
	InvalidWid();
	m_rgpTabs[m_nCurrentTab]->m_pWid->InvalidWid();
	return FALSE;
}

ULONG TabCtrl::GetTabCount() const
{
	return m_rgpTabs.size();
}

String TabCtrl::GetTabLabel( ULONG nTab ) const
{
	WFX_CONDITION(nTab < m_rgpTabs.size());
	if (nTab < m_rgpTabs.size())
	{
		return m_rgpTabs[nTab]->GetText();
	}
	return L"";
}

void TabCtrl::SetTabLabel( ULONG nTab, const String& strTabLabel )
{
	WFX_CONDITION(nTab < m_rgpTabs.size());
	if (nTab < m_rgpTabs.size())
	{
		m_rgpTabs[nTab]->SetText(strTabLabel);
	}
}

void TabCtrl::RecalcLayout()
{
	SendWidMessage(WM_SIZE);
}

LRESULT TabCtrl::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ULONG nItems = m_rgpTabs.size();
	Rect rcItem;
	for (ULONG nTab = 0; nTab < nItems; nTab++)
	{
		m_rgpTabs[nTab]->m_pWid->Create(rcItem, m_pDispatch, this);
	}
	return 1;
}

LRESULT TabCtrl::OnLButtonClik( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	ULONG nTabCliked = 0;
	if (m_rcTabArea.PtInRect(pt))
	{
		nTabCliked = GetTabPt(pt);
	}
	if (IsValidTab(nTabCliked))
	{
		ShowTab(nTabCliked);
	}
	return 1;
}

LRESULT TabCtrl::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

LRESULT TabCtrl::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid = GetRect();
	Rect rcTabItem = rcWid;
	rcTabItem.bottom = rcWid.top + m_nTabHeight;
	m_rcTabArea = rcTabItem;
	ULONG nItems = m_rgpTabs.size();
	for (ULONG nTab = 0; nTab < nItems; nTab++)
	{
		rcTabItem.right = rcTabItem.left + m_nTabWidth;
		m_rgpTabs[nTab]->SetRect(rcTabItem);
		rcTabItem.left += m_nTabWidth;
	}
	m_rcActivate = rcWid;
	m_rcActivate.top = rcWid.top + m_nTabHeight;
	WFX_CONDITION(m_nCurrentTab < m_rgpTabs.size());
	if (m_nCurrentTab < m_rgpTabs.size())
	{
		WFX_CONDITION(m_rgpTabs[m_nCurrentTab]->m_pWid != NULL);
		m_rgpTabs[m_nCurrentTab]->m_pWid->SetRect(m_rcActivate);
		m_rgpTabs[m_nCurrentTab]->m_pWid->ShowWid(SW_SHOW);
	}
	return 1;
}

void TabCtrl::OnDraw( HDC hdc, const Rect& rcPaint )
{
	ULONG nItems = m_rgpTabs.size();
	for (ULONG nTab = 0; nTab < nItems; nTab++)
	{
		m_rgpTabs[nTab]->Draw(hdc, rcPaint, nTab == m_nCurrentTab);
	}
}

ULONG TabCtrl::GetTabPt( const Point& pt ) const
{
	ULONG nItems = m_rgpTabs.size();
	for (ULONG nTab = 0; nTab < nItems; nTab++)
	{
		if (m_rgpTabs[nTab]->GetRect().PtInRect(pt))
		{
			return nTab;
		}
	}
	return 0;
}

BOOL TabCtrl::IsValidTab( ULONG nTab ) const
{
	return nTab < m_rgpTabs.size();
}
