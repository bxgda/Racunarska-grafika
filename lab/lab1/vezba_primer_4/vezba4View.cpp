
// vezba4View.cpp : implementation of the Cvezba4View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "vezba4.h"
#endif

#include "vezba4Doc.h"
#include "vezba4View.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define START_X 10
#define START_Y 10
#define KOCKICA 25
#define SIZE 501

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cvezba4View

IMPLEMENT_DYNCREATE(Cvezba4View, CView)

BEGIN_MESSAGE_MAP(Cvezba4View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// Cvezba4View construction/destruction

Cvezba4View::Cvezba4View() noexcept
{
	// TODO: add construction code here

}

Cvezba4View::~Cvezba4View()
{
}

BOOL Cvezba4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cvezba4View drawing

void Cvezba4View::OnDraw(CDC* pDC)
{
	NacrtajSveOblike(pDC);

	if (this->mreza)
		NacrtajMrezu(pDC);

	SetFocus();
}


// Cvezba4View printing

BOOL Cvezba4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cvezba4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cvezba4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cvezba4View diagnostics

#ifdef _DEBUG
void Cvezba4View::AssertValid() const
{
	CView::AssertValid();
}

void Cvezba4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cvezba4Doc* Cvezba4View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cvezba4Doc)));
	return (Cvezba4Doc*)m_pDocument;
}
#endif //_DEBUG


// Cvezba4View message handlers

void Cvezba4View::NacrtajSveOblike(CDC* pDC)
{
	CPen* oldPen = nullptr;
	CBrush* oldBrush = nullptr;

	SivaPozadina(pDC, oldPen, oldBrush);

	COLORREF zuta = RGB(255, 255, 0);

	// Zuti trougao
	POINT zutiTrougao[3] = { Tacka(4, 3), Tacka(1.25, 5.75), Tacka(6.75, 5.75) };
	NacrtajOblik(pDC, zutiTrougao, 3, zuta, zuta, -1, 0, 0, 0);

	// Narandzsti paralelogram
	POINT narandzstiParalelogram[4] = { Tacka(4, 5.75), Tacka(6.75, 5.75), Tacka(9.5, 8.5), Tacka(6.75, 8.5) };
	NacrtajOblik(pDC, narandzstiParalelogram, 4, zuta, RGB(255, 140, 0), -1, 0, 0, 0);

	// Ljubicasti trougao
	POINT ljubicastiTrougao[3] = { Tacka(6.75, 8.5), Tacka(14.75, 8.5), Tacka(10.75, 12.75) };
	NacrtajOblik(pDC, ljubicastiTrougao, 3, zuta, RGB(160, 32, 240), -1, 7, 20, 0);

	// Crveni trougao
	POINT crveniTrougao[3] = { Tacka(13, 10.5), Tacka(17, 6.5), Tacka(17, 14.5) };
	NacrtajOblik(pDC, crveniTrougao, 3, zuta, RGB(255, 0, 0), -1, 8, 20, 0);

	// Srafirani kvadrat -- moze i ovako, ne mora DrawRegularPolygon
	POINT srafiraniKvadrat[4] = { Tacka(10.75, 12.75), Tacka(13, 10.5), Tacka(15.5, 13), Tacka(13, 15) };
	NacrtajOblik(pDC, srafiraniKvadrat, 4, zuta, RGB(0, 191, 255), HS_HORIZONTAL, 0, 0, 0);

	// Roze trougao
	POINT rozeTrougao[3] = { Tacka(13.5, 14.5), Tacka(13.5, 17), Tacka(10.75, 17) };
	NacrtajOblik(pDC, rozeTrougao, 3, zuta, RGB(255, 20, 147), -1, 5, 10, 0);

	// Zeleni trougao
	POINT zeleniTrougao[3] = { Tacka(17, 13), Tacka(19, 15), Tacka(17, 17) };
	NacrtajOblik(pDC, zeleniTrougao, 3, zuta, RGB(0, 255, 0), -1, 4, 10, 0);
}

void Cvezba4View::SivaPozadina(CDC* pDC, CPen*& oldPen, CBrush*& oldBrush)
{
	CPen newPen(PS_SOLID, 1, RGB(200, 200, 200));
	CBrush newBrush(RGB(230, 230, 230));
	SelektujOlovkuiCetkicu(pDC, newPen, newBrush, oldPen, oldBrush);

	pDC->Rectangle(START_X, START_Y, START_X + SIZE, START_Y + SIZE);

	VratiOlovkuiCetkicu(pDC, oldPen, oldBrush);
}

void Cvezba4View::NacrtajMrezu(CDC* pDC)
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

POINT Cvezba4View::Tacka(float brKockicaX, float brKockicaY)
{
	POINT X;
	X.x = START_X + (int)(KOCKICA * brKockicaX);
	X.y = START_Y + (int)(KOCKICA * brKockicaY);

	return X;
}

POINT Cvezba4View::VratiCentarTrougla(POINT* temena)
{
	double a = this->Razdaljina(temena[1], temena[2]);
	double b = this->Razdaljina(temena[0], temena[2]);
	double c = this->Razdaljina(temena[0], temena[1]);

	POINT Centar = {};
	Centar.x = (temena[0].x * a + temena[1].x * b + temena[2].x * c) / (a + b + c);
	Centar.y = (temena[0].y * a + temena[1].y * b + temena[2].y * c) / (a + b + c);

	return Centar;
}

double Cvezba4View::Razdaljina(POINT A, POINT B)
{
	return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

void Cvezba4View::SelektujOlovkuiCetkicu(CDC* pDC, CPen& pen, CBrush& brush, CPen*& oldPen, CBrush*& oldBrush)
{
	oldPen = pDC->SelectObject(&pen);
	oldBrush = pDC->SelectObject(&brush);
}

void Cvezba4View::VratiOlovkuiCetkicu(CDC* pDC, CPen* oldPen, CBrush* oldBrush)
{
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba4View::NacrtajOblik(CDC* pDC, POINT* temena, int brTemena, COLORREF bojaOlovke, COLORREF bojaCetke, int tipSrafure, int polyN, int polyR, float polyRot)
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

	if (polyN > 0 && brTemena == 3)
	{
		CPen tanjaOlovka(PS_SOLID, 2, bojaOlovke);
		pDC->SelectObject(&tanjaOlovka);

		POINT centar = VratiCentarTrougla(temena);
		DrawRegularPolygon(pDC, centar.x, centar.y, polyR, polyN, polyRot);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba4View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	POINT* Points = new POINT[n];

	float alfa = rotAngle; 

	float factor = 2 * M_PI / (double)n; 

	for (int i = 0; i < n; i++)
	{
		Points[i].x = (int)(cx + r * cos(alfa));
		Points[i].y = (int)(cy + r * sin(alfa));

		alfa += factor;
	}

	pDC->Polygon(Points, n);

	delete[] Points;
}

void Cvezba4View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		this->mreza = !this->mreza;

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Cvezba4View::OnLButtonDown(UINT nFlags, CPoint point)
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

			// 2. Zaokruzivanje na najblizih 0.5
			double kolona = floor(rawX * 2 + 0.5) / 2.0;
			double vrsta = floor(rawY * 2 + 0.5) / 2.0;

			CString msg;
			msg.Format(L"(%.1f, %.1f)", kolona, vrsta);
			AfxMessageBox(msg);
		}
	}

	CView::OnLButtonDown(nFlags, point);
}
