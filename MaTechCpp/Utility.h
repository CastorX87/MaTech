#pragma once
#include "stdafx.h"

#define SafeDelete(x) { if(x != nullptr) { delete (x); (x) = nullptr; }}

inline void DebugPrintInfo(const String text)
{
	auto str = text.toAnsiString();
	printf("I: %s\n", str.c_str());
}

inline void DebugPrintError(const String text)
{
	auto str = text.toAnsiString();
	printf("E: %s\n", str.c_str());
}