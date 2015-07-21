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

void RadioButtonItem::OnDraw( HDC hdc, const RECT& rcPaint )
{
	SharedPtr<Gdiplus::Image> pImage = GetImage();
	if (pImage != NULL)
	{
		return;
	}
}

RadioButton::RadioButton()
{
	m_pItem.reset(new RadioButtonItem);
}
