#pragma once

#include "framework.h"
#include "DImage.h"

#include <string>

namespace MonaPuzzleViewHelpers
{
	void DrawGridLines(CDC* pDC, int left, int top, int width, int height, int spacing);

	void DrawTransparent(
		CDC* pDC,
		DImage& img,
		int x,
		int y,
		COLORREF clrTransparent);

	bool TryReadBoolToken(const std::wstring& token, const wchar_t* key, bool& valueOut);
	bool TryReadIntToken(const std::wstring& token, const wchar_t* key, int& valueOut);
	bool TryReadFloatToken(const std::wstring& token, const wchar_t* key, float& valueOut);
}
