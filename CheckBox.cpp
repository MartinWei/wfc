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
#include "wfxgdi.h"

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

void CheckBoxItem::OnDraw( Gdiplus::Graphics& grph )
{
	SharedPtr<Gdiplus::Image> pImage = GetImage();
	if (pImage != NULL)
	{
		return;
	}
	DrawCheckBoxItem(this, grph);
}

SharedPtr<Gdiplus::Image> CheckBoxItem::GetImage() const
{
	return m_bChecked? m_pImageChecked : m_pImageUnCheck;
}

BOOL CheckBoxItem::DrawCheckBoxItem( Widget* pWid, Gdiplus::Graphics& grph )
{
	Button* pButton = dynamic_cast<Button*>(pWid);
	if (pButton == NULL)
	{
		return FALSE;
	}

	BOOL bChecked = pButton->IsCheck();
	Gdiplus::RectF rc;
	pButton->GetRect(rc);
	Gdiplus::RectF rcIn = rc;
	rcIn.X += 2 * WID_CKB_MARGIN;
	rcIn.Y += 2 * WID_CKB_MARGIN;
	rcIn.Width -= 4 * WID_CKB_MARGIN;
	rcIn.Height -= 4 * WID_CKB_MARGIN;
	Gdiplus::Color clr;
	clr.SetFromCOLORREF(WID_CKB_BKGND);
	Gdiplus::SolidBrush brsh(clr);
	grph.FillRectangle(&brsh, rc);

	clr.SetFromCOLORREF(WID_CKB_FRAMEO);
	Gdiplus::Pen pn(clr);
	grph.DrawRectangle(&pn, rc);

	clr.SetFromCOLORREF(WID_CKB_FRAMEI);
	pn.SetColor(clr);
	grph.DrawRectangle(&pn, rcIn);

	if (bChecked)
	{
		Gdiplus::PointF pts[3] = {
			Gdiplus::PointF(rcIn.X,
			rcIn.Y + rcIn.Height/2)
			, Gdiplus::PointF(rcIn.X + rcIn.Width/3,
			rcIn.Y + rcIn.Height - WID_CKB_MARGIN)
			, Gdiplus::PointF(rcIn.X + rcIn.Width,
			rcIn.Y)
		};
		clr.SetFromCOLORREF(WID_CKB_CHECK);
		pn.SetColor(clr);
		pn.SetWidth(2);
		grph.DrawLines(&pn, pts, 3);
	}

	return TRUE;
}

CheckBox::CheckBox()
: m_pItem(new CheckBoxItem)
, m_lOffset(0)
{

}

LRESULT CheckBox::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Gdiplus::RectF rc;
	m_pItem->Create(rc, m_pDispatch, this);
	return 0;
}

LRESULT CheckBox::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Gdiplus::RectF rc;
	GetRect(rc);
	rc.Width = WID_CKB_SIZE;
	rc.X += 2;
	rc.Y = rc.Y + (rc.Height - WID_CKB_SIZE) / 2;
	rc.Height = WID_CKB_SIZE;
	m_pItem->SetWidRect(rc);
	m_lOffset = WID_CKB_SIZE + 5;
	return 0;
}

void CheckBox::OnDraw( Gdiplus::Graphics& grph )
{
	Gdiplus::RectF rc;
	GetRect(rc);
	Gdiplus::SolidBrush brsh(m_clrBkgnd);
	grph.FillRectangle(&brsh, rc.X, rc.Y, rc.Width, rc.Height);
	Gdiplus::Pen pn(m_clrFrame);
	grph.DrawRectangle(&pn, rc);
	brsh.SetColor(m_clrText);
	rc.X += m_lOffset;
	rc.Width -= m_lOffset;
	grph.DrawString(m_strText.c_str(), m_strText.size(), m_pFont.get(),
		rc, m_pFormat.get(), &brsh);
}
