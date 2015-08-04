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
CheckBoxItem::CheckBoxItem()
: Button(TRUE)
{

}

CheckBoxItem::CheckBoxItem(const String& strChecked,
						   const String& strUnCheck)
						   : m_pImageChecked(Gdiplus::Image::FromFile(strChecked.c_str()))
						   , m_pImageUnCheck(Gdiplus::Image::FromFile(strUnCheck.c_str()))
{

}

void CheckBoxItem::OnDraw( HDC hdc, const Rect& rcPaint )
{
	PImage pImage = GetImage();
	if (pImage != NULL)
	{
		return;
	}
	Rect rc = GetRect();
	WfxRender::DrawCheckBoxItem(hdc, rc, GetState(), IsChecked(), m_pDispatch);
}

PImage CheckBoxItem::GetImage() const
{
	return m_bChecked? m_pImageChecked : m_pImageUnCheck;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
CheckBox::CheckBox()
: m_pItem(new CheckBoxItem)
, m_lOffset(0)
{
	SetText(L"CheckBox");
}

LRESULT CheckBox::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	m_pItem->Create(rc, m_pDispatch, this);
	return 0;
}

LRESULT CheckBox::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc = GetRect();
	rc.left += 2;
	rc.right = rc.left + WID_CKB_SIZE;
	rc.top = rc.top + (rc.bottom - rc.top - WID_CKB_SIZE) / 2;
	rc.bottom = rc.top + WID_CKB_SIZE;
	m_pItem->SetRect(rc);
	m_lOffset = rc.right - rc.left + 2;
	return 0;
}

void CheckBox::OnDraw( HDC hdc, const Rect& rcPaint )
{
 WfxRender::DrawCheckBox(hdc, GetText(), GetRect(), GetState(), m_lOffset, m_pDispatch);
}
