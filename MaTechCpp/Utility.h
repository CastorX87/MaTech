#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "SceneObject.h"

#define SafeDelete(x) { if(x != nullptr) { delete (x); (x) = nullptr; }}

using namespace sf;

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

