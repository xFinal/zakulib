#pragma once

#include "../define/zaku_namespace_def.h"

ZAKU_LIB_BEGIN

class interface_base
{
protected:
	virtual void Dispose() noexcept = 0;

public:
	virtual ~interface_base() = 0 {};
};

ZAKU_LIB_END
