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
class View;
class Frame;

class WFX_API Document : public WidgetBase
{
public:
	virtual BOOL Open() = 0;
	virtual BOOL Close() = 0;
};

template<class T>
class Factory
{
public:
	virtual T* CreateObject()
	{
		return new T();
	}
};


class WFX_API DocumentMgr : public WidgetBase
{
public:
	virtual int AddDoc(Factory<Document>* pDocFactory,
		Factory<Frame>* pFrameFactory, 
		Factory<View>* pViewFactory);
	virtual Document* Next();
	virtual Document* Pre();
	virtual Document* GetAt(int i);
	virtual BOOL Open(int i);
	virtual BOOL Close(int i);
};

END_NAMESPACE_WFX
