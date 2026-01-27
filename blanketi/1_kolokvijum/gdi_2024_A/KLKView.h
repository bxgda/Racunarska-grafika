
// KLKView.h : interface of the CKLKView class
//

#pragma once

#define PI 3.14159265358979f

class DImage;

class CKLKView : public CView
{
protected: // create from serialization only
	CKLKView() noexcept;
	DECLARE_DYNCREATE(CKLKView)

// Attributes
public:
	CKLKDoc* GetDocument() const;

// Operations
public:
	// Dodate metode

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
	virtual ~CKLKView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()


// moje funkcije i promenljive --------------------------------------------
protected:
	DImage* m_imgBackground;
	DImage* m_imgBasket;
	DImage* m_imgArm;

	float offset = 0.0f; 
	float angle = 0.0f;  
	float alpha = 0.0f;  
	float beta = 0.0f;   

	void DrawBackground(CDC* pDC);
	void DrawImageTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dx, float dy, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void Scale(CDC* pDC, float sx, float sy, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = false);
	inline float ToRad(float degrees) { return degrees * PI / 180.0f; }
	void DrawScene(CDC* pDC);

	void DrawArm(CDC* pDC);
	void DrawBasket(CDC* pDC, int r = 1); 
	void DrawBasketCouple(CDC* pDC, int l, int r, double angle);
	void DrawPlatform(CDC* pDC, int l, int r, double angle);
	void DrawCarousel(CDC* pDC, int h, int r, double offset, double alpha, double beta, double angle);

	void DrawBasketLabel(CDC* pDC, int r);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in KLKView.cpp
inline CKLKDoc* CKLKView::GetDocument() const
   { return reinterpret_cast<CKLKDoc*>(m_pDocument); }
#endif

