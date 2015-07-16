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

CheckBoxItem::CheckBoxItem()
: Button(TRUE)
{

}

CheckBoxItem::CheckBoxItem(const std::wstring& strChecked,
						   const std::wstring& strUnCheck)
						   : m_pImageChecked(Gdiplus::Image::FromFile(strChecked.c_str()))
						   , m_pImageUnCheck(Gdiplus::Image::FromFile(strUnCheck.c_str()))
{

}

CheckBoxItem::~CheckBoxItem()
{

}

void CheckBoxItem::OnDraw( HDC hdc, const RECT& rcPaint )
{
	SharedPtr<Gdiplus::Image> pImage = GetImage();
	if (pImage != NULL)
	{
		return;
	}
	RECT rc = GetRect();
	WfxRender::DrawCheckBox(hdc, rc, GetState(), m_pDispatch);
}

SharedPtr<Gdiplus::Image> CheckBoxItem::GetImage() const
{
	return m_bChecked? m_pImageChecked : m_pImageUnCheck;
}

CheckBox::CheckBox()
: m_pItem(new CheckBoxItem)
, m_lOffset(0)
{

}

LRESULT CheckBox::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	RECT rc;
	m_pItem->Create(rc, m_pDispatch, this);
	return 0;
}

LRESULT CheckBox::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	RECT rc = GetRect();
	rc.right = rc.left + WID_CKB_SIZE;
	rc.left += 2;
	rc.top = rc.top + (rc.bottom - rc.top - WID_CKB_SIZE) / 2;
	rc.bottom = rc.top + WID_CKB_SIZE;
	m_pItem->SetRect(rc);
	m_lOffset = WID_CKB_SIZE + 5;
	return 0;
}

void CheckBox::OnDraw( HDC hdc, const RECT& rcPaint )
{
	/*RECT rc;
	GetRect(rc);
	Gdiplus::SolidBrush brsh(m_clrBkgnd);
	grph.FillRectangle(&brsh, rc.X, rc.Y, rc.Width, rc.Height);
	Gdiplus::Pen pn(m_clrFrame);
	grph.DrawRectangle(&pn, rc);
	brsh.SetColor(m_clrText);
	rc.X += m_lOffset;
	rc.Width -= m_lOffset;
	grph.DrawString(m_strText.c_str(), m_strText.size(), m_pFont.get(),
		rc, m_pFormat.get(), &brsh);*/
}
