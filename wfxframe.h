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

class Window;
class Document;
class View;

class WFX_API Frame : public Window
{
public:
	virtual Document* GetActiveDoc();
public:
	View* GetActiveView();
};

END_NAMESPACE_WFX
