
// vezba1View.h : interface of the Cvezba1View class
//

#pragma once

#include "DImage.h"


class Cvezba1View : public CView
{
protected: // create from serialization only
	Cvezba1View() noexcept;
	DECLARE_DYNCREATE(Cvezba1View)

// Attributes
public:
	Cvezba1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cvezba1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	// moje promenljive i funkcije ------------------------------------------------------
protected:
	DImage m_slike[10];

	void Scale(CDC* pDC, float sX, float sY, bool desnoMnozenje);
	void Rotate(CDC* pDC, float ugao, bool desnoMnozenje);
	void Translate(CDC* pDC, float dX, float dY, bool desnoMnozenje);
	void Mirror(CDC* pDC, bool mx, bool my, bool desnoMnozenje);

	void NacrtajMrezu(CDC* pDC);

	void FilterBitaMapa(CBitmap* map, int kanal);
	void NacrtajDeoSlagalice(CDC* pDC, int deo, int filter);

	// ----------------------------------------------------------------------------------
};

#ifndef _DEBUG  // debug version in vezba1View.cpp
inline Cvezba1Doc* Cvezba1View::GetDocument() const
   { return reinterpret_cast<Cvezba1Doc*>(m_pDocument); }
#endif

