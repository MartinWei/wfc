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

#pragma once

#include "wfx.h"

BEGIN_NAMESPACE_WFX

class WidgetBase;

class WFX_API App : public WidgetBase
{
public:
	App(HINSTANCE hInstance, Frame* pFrame = NULL);
	virtual ~App();
public:
	virtual BOOL LoadFrame(Frame* pFrame);
public:
	virtual BOOL run();
protected:
	HINSTANCE m_hInstance;
	Frame* m_pFrame;
};

END_NAMESPACE_WFX
