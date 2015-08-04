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

ProcessBar::ProcessBar()
: m_nMax(0)
, m_nPos(0)
{

}

void ProcessBar::SetRange( ULONG nMax )
{
	m_nMax = nMax;
}

void ProcessBar::SetPos( ULONG nPos, BOOL bDraw /*= TRUE*/ )
{
	m_nPos = nPos;
	if (bDraw)
	{
		InvalidWid();
	}
}

ULONG ProcessBar::GetPos() const
{
	return m_nPos;
}

void ProcessBar::OnDraw( HDC hdc, const Rect& rc )
{
	WfxRender::DrawProcessBar(hdc, GetRect(), GetState(), m_nMax, m_nPos, m_pDispatch);
}
