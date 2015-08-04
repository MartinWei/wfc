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

DocManager::DocManager()
: m_nCurrent(-1)
{

}

BOOL DocManager::ProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	return TRUE;
}

ULONG DocManager::AddDoc( CPDocFactory& pDocFactory, CPViewFactroy& pViewFactory )
{
	m_rgpDocToView.push_back(std::make_pair(pDocFactory, pViewFactory));
	return m_rgpDocToView.size();
}

Document* DocManager::Next()
{
	return NULL;
}

Document* DocManager::Pre()
{
	return NULL;
}

Document* DocManager::GetAt( int i )
{
	return NULL;
}

BOOL DocManager::Open( int i )
{
	return FALSE;
}

BOOL DocManager::Close( int i )
{
	return FALSE;
}
