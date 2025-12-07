
// vezba5View.cpp : implementation of the Cvezba5View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "vezba5.h"
#endif

#include "vezba5Doc.h"
#include "vezba5View.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define START_X 10
#define START_Y 10
#define KOCKICA 25
#define SIZE 501

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cvezba5View

IMPLEMENT_DYNCREATE(Cvezba5View, CView)

BEGIN_MESSAGE_MAP(Cvezba5View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// Cvezba5View construction/destruction

Cvezba5View::Cvezba5View() noexcept
{
	// TODO: add construction code here

}

Cvezba5View::~Cvezba5View()
{
}

BOOL Cvezba5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cvezba5View drawing

void Cvezba5View::OnDraw(CDC* pDC)
{
	NacrtajSveOblike(pDC);

	if (this->mreza)
		NacrtajMrezu(pDC);

	SetFocus();
}


// Cvezba5View printing

BOOL Cvezba5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cvezba5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cvezba5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cvezba5View diagnostics

#ifdef _DEBUG
void Cvezba5View::AssertValid() const
{
	CView::AssertValid();
}

void Cvezba5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cvezba5Doc* Cvezba5View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cvezba5Doc)));
	return (Cvezba5Doc*)m_pDocument;
}
#endif //_DEBUG


// Cvezba5View message handlers

void Cvezba5View::NacrtajSveOblike(CDC* pDC)
{
	CPen* oldPen = nullptr;
	CBrush* oldBrush = nullptr;

	this->SivaPozadina(pDC, oldPen, oldBrush);

	COLORREF zelena = RGB(0, 255, 0);

	// zuti trougao
	POINT zutiTrougao[3] = { Tacka(1, 7), Tacka(7, 7), Tacka(7, 13) };
	NacrtajOblik(pDC, zutiTrougao, 3, zelena, RGB(255, 255, 0), -1, 7, 20, 0);

	// narandzsti kvadrat -- moze i bez poziva DrawRegularPolygon
	POINT narandzastiKvadrat[4] = { Tacka(7, 7), Tacka(7, 10), Tacka(10, 10), Tacka(10, 7)};
	NacrtajOblik(pDC, narandzastiKvadrat, 4, zelena, RGB(255, 165, 0), -1, 0, 0, 0);

	// crveni trougao
	POINT crveniTrougao[3] = { Tacka(7, 10), Tacka(7, 13), Tacka(10, 13) };
	NacrtajOblik(pDC, crveniTrougao, 3, zelena, RGB(255, 0, 0), -1, 4, 10, 0);

	// srafirani paralelogram
	POINT srafiraniParalelogram[4] = { Tacka(7, 10), Tacka(10, 13), Tacka(13, 13), Tacka(10, 10) };
	NacrtajOblik(pDC, srafiraniParalelogram, 4, zelena, RGB(0, 0, 255), HS_DIAGCROSS, 0, 0, 0);

	// ljubicasti trougao
	POINT ljubicastiTrougao[3] = { Tacka(10, 7), Tacka(13, 7), Tacka(10, 10) };
	NacrtajOblik(pDC, ljubicastiTrougao, 3, zelena, RGB(128, 0, 128), -1, 8, 10, M_PI / 6);

	// zeleni trougao
	POINT zeleniTrougao[3] = { Tacka(10, 10), Tacka(13, 7), Tacka(13, 13) };
	NacrtajOblik(pDC, zeleniTrougao, 3, zelena, RGB(0, 163, 2), -1, 5, 15, 0);

	// roze trougao
	POINT rozeTrougao[3] = { Tacka(13, 7), Tacka(13, 13), Tacka(19, 13) };
	NacrtajOblik(pDC, rozeTrougao, 3, zelena, RGB(255, 192, 203), -1, 6, 20, 0);
}

void Cvezba5View::SivaPozadina(CDC* pDC, CPen*& oldPen, CBrush*& oldBrush)
{
	CPen newPen(PS_SOLID, 1, RGB(200, 200, 200));
	CBrush newBrush(RGB(230, 230, 230));
	SelektujOlovkuiCetkicu(pDC, newPen, newBrush, oldPen, oldBrush);

	pDC->Rectangle(START_X, START_Y, START_X + SIZE, START_Y + SIZE);

	VratiOlovkuiCetkicu(pDC, oldPen, oldBrush);
}

void Cvezba5View::NacrtajMrezu(CDC* pDC)
{
	CPen SiviGrid(PS_SOLID, 1, RGB(250, 250, 250));
	CPen* oldPen = pDC->SelectObject(&SiviGrid);

	for (int i = START_X; i < START_X + SIZE; i += KOCKICA) {
		pDC->MoveTo(i, START_Y);
		pDC->LineTo(i, START_Y + SIZE);

		pDC->MoveTo(START_Y, i);
		pDC->LineTo(START_Y + SIZE, i);
	}

	pDC->SelectObject(oldPen);
}

POINT Cvezba5View::Tacka(float brKockicaX, float brKockicaY)
{
	POINT X;
	X.x = START_X + (int)(KOCKICA * brKockicaX);
	X.y = START_Y + (int)(KOCKICA * brKockicaY);

	return X;
}

POINT Cvezba5View::VratiCentarTrougla(POINT* temena)
{
	double a = this->Razdaljina(temena[1], temena[2]);
	double b = this->Razdaljina(temena[0], temena[2]);
	double c = this->Razdaljina(temena[0], temena[1]);

	POINT Centar = {};
	Centar.x = (temena[0].x * a + temena[1].x * b + temena[2].x * c) / (a + b + c);
	Centar.y = (temena[0].y * a + temena[1].y * b + temena[2].y * c) / (a + b + c);

	return Centar;
}

double Cvezba5View::Razdaljina(POINT A, POINT B)
{
	return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

void Cvezba5View::SelektujOlovkuiCetkicu(CDC* pDC, CPen& pen, CBrush& brush, CPen*& oldPen, CBrush*& oldBrush)
{
	oldPen = pDC->SelectObject(&pen);
	oldBrush = pDC->SelectObject(&brush);
}

void Cvezba5View::VratiOlovkuiCetkicu(CDC* pDC, CPen* oldPen, CBrush* oldBrush)
{
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba5View::NacrtajOblik(CDC* pDC, POINT* temena, int brTemena, COLORREF bojaOlovke, COLORREF bojaCetke, int tipSrafure, int polyN, int polyR, float polyRot)
{
	CPen novaOlovka(PS_SOLID, 6, bojaOlovke);

	CBrush novaCetka;
	if (tipSrafure == -1) 
		novaCetka.CreateSolidBrush(bojaCetke);
	else 
		novaCetka.CreateHatchBrush(tipSrafure, bojaCetke);

	CPen* oldPen = pDC->SelectObject(&novaOlovka);
	CBrush* oldBrush = pDC->SelectObject(&novaCetka);

	pDC->Polygon(temena, brTemena);

	if (polyN > 0 && brTemena == 3) {
		CPen tanjaOlovka(PS_SOLID, 2, bojaOlovke);
		pDC->SelectObject(&tanjaOlovka);

		POINT centar = VratiCentarTrougla(temena);
		DrawRegularPolygon(pDC, centar.x, centar.y, polyR, polyN, polyRot);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba5View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	POINT* Points = new POINT[n];

	float alfa = rotAngle; 

	float factor = 2 * M_PI / (double)n; 

	for (int i = 0; i < n; i++) {
		Points[i].x = (int)(cx + r * cos(alfa));
		Points[i].y = (int)(cy + r * sin(alfa));

		alfa += factor;
	}

	pDC->Polygon(Points, n);

	delete[] Points;
}



void Cvezba5View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		this->mreza = !this->mreza;

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Cvezba5View::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (this->mreza)
	{
		int x = point.x;
		int y = point.y;

		if (x >= START_X && x <= START_X + SIZE &&
			y >= START_Y && y <= START_Y + SIZE)
		{

			double rawX = (double)(x - START_X) / KOCKICA;
			double rawY = (double)(y - START_Y) / KOCKICA;

			double kolona = floor(rawX * 2 + 0.5) / 2.0;
			double vrsta = floor(rawY * 2 + 0.5) / 2.0;

			CString msg;
			msg.Format(L"(%.1f, %.1f)", kolona, vrsta);
			AfxMessageBox(msg);
		}
	}

	CView::OnLButtonDown(nFlags, point);
}
