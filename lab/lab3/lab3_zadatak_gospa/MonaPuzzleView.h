// MonaPuzzleView.h : interface of the CMonaPuzzleView class
//

#pragma once

#include "DImage.h"

struct PuzzlePiece
{
	DImage image;
	CString resourcePath;
	int x;
	int y;
	float angle;
	bool mirrorX;
	bool mirrorY;
	bool grayscale;
	bool customFilter;

	PuzzlePiece()
		: x(0), y(0), angle(0.0f),
		mirrorX(false), mirrorY(false),
		grayscale(true), customFilter(false)
	{
	}
};

class CMonaPuzzleView : public CView
{
protected: // create from serialization only
	CMonaPuzzleView() noexcept;
	DECLARE_DYNCREATE(CMonaPuzzleView)

	// Attributes
public:
	CMonaPuzzleDoc* GetDocument() const;

	PuzzlePiece m_puzzlePieces[9];
	int m_activePieceIndex = 0;

	bool m_fastMoving = true;
	bool m_showSelectionBorder = true;

	CString m_stateFilePath = L"puzzle_state.txt";

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CMonaPuzzleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Svetske transformacije
	void Translate(CDC* pDC, float dx, float dy, bool rightMultiply = false);
	void Scale(CDC* pDC, float sx, float sy, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = false);

	// Piece drawing and filtering
	void DrawPuzzlePiece(CDC* pDC, PuzzlePiece& piece);
	void DrawSelection(CDC* pDC, PuzzlePiece& piece);
	void ApplyGrayscaleFilter(PuzzlePiece& piece);
	void ApplyAlternativeFilter(PuzzlePiece& piece);
	void ReapplyFilters(PuzzlePiece& piece);
	void ResetToDefaultState();

	void FilterBitaMapa(CBitmap* map, int kanal);
	void NacrtajDeoSlagalice(CDC* pDC, CString deo, int filter);

	CString GetStateFilePath() const;
	bool LoadStateFromFile(const CString& path);
	bool SaveStateToFile(const CString& path) const;

	// Additional drawing functions
	void DrawGrid(CDC* pDC);
	void DrawOverlayText(CDC* pDC);

	// State loading/saving

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in MonaPuzzleView.cpp
inline CMonaPuzzleDoc* CMonaPuzzleView::GetDocument() const
{
	return reinterpret_cast<CMonaPuzzleDoc*>(m_pDocument);
}
#endif

