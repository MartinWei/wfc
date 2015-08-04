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

View::View()
: m_pDoc(NULL)
{

}

Document* View::GetDocument() const
{
	return m_pDoc;
}

void View::SetDocument( Document* pDoc )
{
	m_pDoc = pDoc;
}
