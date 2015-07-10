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

class Widget;
class Grid;

class WFX_API LayoutAttribtes
{

};

class WFX_API LayoutBase
{
public:
	virtual ~LayoutBase() = 0;
public:
	virtual Grid* Parse(const std::wstring& strXml) = 0;
};

class WFX_API Grid : public Widget
{

};


END_NAMESPACE_WFX
