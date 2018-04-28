//  Copyright (c) 2014, Wentao Xin. All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.

#pragma once

#include "zaku_namespace_def.h"

ZAKU_LIB_BEGIN

#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif

ZAKU_LIB_END