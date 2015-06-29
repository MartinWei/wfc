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
#include "Widget.h"

USING_NAMESPACE_WFX;

void RadioButtonItem::OnDraw( Gdiplus::Graphics& grph )
{
	SharedPtr<Gdiplus::Image> pImage = GetImage();
	if (pImage != NULL)
	{
		return;
	}
	DrawRadioButtonItem(this, grph);
}

BOOL RadioButtonItem::DrawRadioButtonItem( Widget* pWid, Gdiplus::Graphics& grph )
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
	Gdiplus::RectF rcCheck = rcIn;
	rcCheck.X += WID_CKB_MARGIN;
	rcCheck.Y += WID_CKB_MARGIN;
	rcCheck.Width -= 2 * WID_CKB_MARGIN;
	rcCheck.Height -= 2 * WID_CKB_MARGIN;
	Gdiplus::Color clr;
	clr.SetFromCOLORREF(WID_CKB_BKGND);
	Gdiplus::SolidBrush brsh(clr);

	clr.SetFromCOLORREF(WID_CKB_FRAMEO);
	Gdiplus::Pen pn(clr);
	Gdiplus::GraphicsPath path;
	path.AddArc(rc, 0, 360);
	grph.DrawPath(&pn, &path);

	clr.SetFromCOLORREF(WID_CKB_FRAMEI);
	pn.SetColor(clr);
	path.Reset();
	path.AddArc(rcIn, 0, 360);
	grph.DrawPath(&pn, &path);

	if (bChecked)
	{
		path.Reset();
		path.AddArc(rcIn, 0, 360);
		clr.SetFromCOLORREF(WID_RB_CHECK);
		brsh.SetColor(clr);
		grph.FillPath(&brsh, &path);
	}

	return TRUE;
}

RadioButton::RadioButton()
{
	m_pItem.reset(new RadioButtonItem);
}
