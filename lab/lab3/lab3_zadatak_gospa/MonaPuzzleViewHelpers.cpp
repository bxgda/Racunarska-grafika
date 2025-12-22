#include "pch.h"
#include "framework.h"

#include "MonaPuzzleViewHelpers.h"

#include <cwchar>
#include <cstdlib>

namespace MonaPuzzleViewHelpers
{
	void DrawGridLines(CDC* pDC, int left, int top, int width, int height, int spacing)
	{
		CPen penGrid;
		penGrid.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
		CPen* oldPen = pDC->SelectObject(&penGrid);

		for (int x = left; x <= left + width; x += spacing)
		{
			pDC->MoveTo(x, top);
			pDC->LineTo(x, top + height);
		}

		for (int y = top; y <= top + height; y += spacing)
		{
			pDC->MoveTo(left, y);
			pDC->LineTo(left + width, y);
		}

		pDC->SelectObject(oldPen);
	}

	void DrawTransparent(CDC* pDC, DImage& img, int x, int y, COLORREF clrTransparent)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);

		CBitmap* oldBmp = memDC.SelectObject(img.GetBitmap());

		TransparentBlt(
			pDC->m_hDC,
			x, y,
			img.Width(), img.Height(),
			memDC.m_hDC,
			0, 0,
			img.Width(), img.Height(),
			clrTransparent);

		memDC.SelectObject(oldBmp);
	}

	bool TryReadBoolToken(const std::wstring& token, const wchar_t* key, bool& valueOut)
	{
		const std::wstring prefix = std::wstring(key) + L"=";
		if (token.rfind(prefix, 0) != 0)
			return false;
		int v = _wtoi(token.c_str() + prefix.size());
		valueOut = (v != 0);
		return true;
	}

	bool TryReadIntToken(const std::wstring& token, const wchar_t* key, int& valueOut)
	{
		const std::wstring prefix = std::wstring(key) + L"=";
		if (token.rfind(prefix, 0) != 0)
			return false;
		valueOut = _wtoi(token.c_str() + prefix.size());
		return true;
	}

	bool TryReadFloatToken(const std::wstring& token, const wchar_t* key, float& valueOut)
	{
		const std::wstring prefix = std::wstring(key) + L"=";
		if (token.rfind(prefix, 0) != 0)
			return false;
		valueOut = (float)_wtof(token.c_str() + prefix.size());
		return true;
	}
}
