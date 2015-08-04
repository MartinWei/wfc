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
#include "wfxfrmwk.h"

USING_NAMESPACE_WFX;

Document::Document()
: m_pView(NULL)
{

}

BOOL Document::ProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	return TRUE;
}

BOOL Document::Open()
{
	return TRUE;
}

BOOL Document::Close()
{
	return TRUE;
}

View* Document::GetView() const
{
	return m_pView;
}

void Document::SetView( View* pView )
{
	m_pView = pView;
}
