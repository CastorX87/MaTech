#pragma once
#include "stdafx.h"

struct NonCopyableClass
{
	NonCopyableClass() = default;
	NonCopyableClass(const NonCopyableClass&) = delete;
	NonCopyableClass & operator=(const NonCopyableClass&) = delete;
};