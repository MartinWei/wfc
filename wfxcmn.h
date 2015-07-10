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
	WfxFormatPtr				pFormat;
	ULONG						nOrder;
	UINT						uState;
	HeaderInfo()
		: cxy(0)
		, strText(L"HeaderInfo")
		, pFormat(new Gdiplus::StringFormat)
		, nOrder(0)
		, uState(0)
	{
		pFormat->SetAlignment(Gdiplus::StringAlignmentCenter);
		pFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	}
	void SetImage(const std::wstring& strFile)
	{
		pImage.reset(Gdiplus::Image::FromFile(strFile.c_str()));
	}
	void SetFormat(DWORD dwFormat)
	{
		if (dwFormat & DT_CENTER || dwFormat & DT_VCENTER)
		{
			pFormat->SetAlignment(Gdiplus::StringAlignmentCenter);
			pFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
		}
		if (dwFormat & DT_LEFT)
		{

		}
		if (dwFormat & DT_RIGHT)
		{

		}
	}
};

class WFX_API HeaderCtrl : public Widget
{
public:
	ULONG GetItemCount() const;
	BOOL GetItemRect(ULONG nIndex, Gdiplus::RectF& rc);
	ULONG InsertItem(ULONG nPos, const HeaderInfo& hdi);
	BOOL RemoveItem(ULONG nPos);
protected:
	std::vector<SharedPtr<HeaderInfo> > m_rgpHdi;
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
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	ULONG GetColumnWidth(ULONG nCol) const;
	BOOL SetColumnWidth(ULONG nCol, ULONG cx);
	HeaderCtrl* GetHeaderCtrl() const;
	void SetHeaderCtrl(HeaderCtrl* pHeader);
	std::wstring GetItemText(ULONG nItem, ULONG nSubItem) const;
	BOOL SetItemText(ULONG nItem, ULONG nSubItem, const std::wstring& strText);
protected:
	SharedPtr<HeaderCtrl> m_pHeadCtrl;
	ULONG m_nHeadHeight;
};

END_NAMESPACE_WFX
