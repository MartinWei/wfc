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

App::App( HINSTANCE hInstance, PFrame pFrame /*= PFrame()*/ )
: m_hInstance(hInstance)
, m_pDocMgr(new DocManager)
, m_pFrame(pFrame)
{

}

App::~App()
{

}

void App::AddDocument( CPDocFactory& pDocFactory, CPViewFactroy& pViewFactory )
{

}

BOOL App::InitInstance()
{
	return FALSE;
}

int App::ExitInstance()
{
	return 1;
}

int App::Run()
{
	return 1;
}
