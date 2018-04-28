#pragma once

#include "../define/zaku_namespace_def.h"

ZAKU_LIB_BEGIN

class uncopyable
{
protected:
	uncopyable() {}
	~uncopyable() {}

private:
	uncopyable(const uncopyable& other) {}
	uncopyable& operator=(uncopyable other) {}

	uncopyable(uncopyable &&other) {}
	uncopyable& operator=(uncopyable &&other) {}
};

ZAKU_LIB_END