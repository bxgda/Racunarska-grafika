
// vezba1View.h : interface of the Cvezba1View class
//

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>


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

	// moje promeljive i funkcije ------------------------------------------
protected:
	bool mreza = true;
	float ugao1 = 0.0f;
	float ugao2 = 0.0f;
	float ugao3 = M_PI_2;

	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);

	void NacrtajCvor(CDC* pDC);
	void NacrtajDeoKaktusa(CDC* pDC, float ugao, float sirina, float visina, bool jeSvetao);

	void DrawFigure(CDC* pDC);

	void PlavaPozadina(CDC* pDC);
	void NacrtajMrezu(CDC* pDC);

	void NacrtajVazu(CDC* pDC, POINT *tacke);
	void IspisiTekst(CDC* pDC, CString tekst, int x, int y, int ugaoTeksta, int ugaoKaraktera);

	int Kockica(float brKockica);

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	// ---------------------------------------------------------------------
};

#ifndef _DEBUG  // debug version in vezba1View.cpp
inline Cvezba1Doc* Cvezba1View::GetDocument() const
   { return reinterpret_cast<Cvezba1Doc*>(m_pDocument); }
#endif

