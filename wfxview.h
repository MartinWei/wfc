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
class Widget;
class Document;

class WFX_API View : public Widget
{
protected:
	View();
public:
	Document* GetDocument() const;
protected:
	Document* m_pDoc;
};


END_NAMESPACE_WFX
