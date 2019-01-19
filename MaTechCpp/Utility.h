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

inline Vector2f ScreenPosToHPos(const Vector2i& pixelPos, const Vector2i& viewportSize, float zoomFactor)
{
	float asp = viewportSize.x / (float)viewportSize.y;
	Vector2f TP = Vector2f(pixelPos.x, viewportSize.y - pixelPos.y);
	TP.x /= viewportSize.x;
	TP.y /= viewportSize.x;

	TP.x = (TP.x - 0.5) / zoomFactor;
	TP.y = (TP.y - (1 / asp) / 2.0f) / zoomFactor;

	return TP;
}