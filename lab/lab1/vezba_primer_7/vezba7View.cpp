
// vezba7View.cpp : implementation of the Cvezba7View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "vezba7.h"
#endif

#include "vezba7Doc.h"
#include "vezba7View.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define START_X 10
#define START_Y 10
#define KOCKICA 25
#define SIZE 501

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cvezba7View

IMPLEMENT_DYNCREATE(Cvezba7View, CView)

BEGIN_MESSAGE_MAP(Cvezba7View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// Cvezba7View construction/destruction

Cvezba7View::Cvezba7View() noexcept
{
	// TODO: add construction code here

}

Cvezba7View::~Cvezba7View()
{
}

BOOL Cvezba7View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cvezba7View drawing

void Cvezba7View::OnDraw(CDC* pDC)
{
	NacrtajSveOblike(pDC);

	if (this->mreza)
		NacrtajMrezu(pDC);

	SetFocus();
}


// Cvezba7View printing

BOOL Cvezba7View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cvezba7View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cvezba7View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cvezba7View diagnostics

#ifdef _DEBUG
void Cvezba7View::AssertValid() const
{
	CView::AssertValid();
}

void Cvezba7View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cvezba7Doc* Cvezba7View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cvezba7Doc)));
	return (Cvezba7Doc*)m_pDocument;
}
#endif //_DEBUG


// Cvezba7View message handlers

void Cvezba7View::NacrtajSveOblike(CDC* pDC)
{
	CPen* oldPen = nullptr;
	CBrush* oldBrush = nullptr;

	this->SivaPozadina(pDC, oldPen, oldBrush);

	COLORREF ljubicasta = RGB(215, 0, 255);

	// srafirani trougao
	POINT srafiraniTrougao[3] = { Tacka(1, 12), Tacka(10, 12), Tacka(10, 3) };
	this->NacrtajOblik(pDC, srafiraniTrougao, 3, ljubicasta, RGB(0, 0, 255), HS_FDIAGONAL, 6, 30, 0);

	// crveni trougao
	POINT crveniTrougao[3] = { Tacka(10, 12), Tacka(10, 3), Tacka(19, 12) };
	this->NacrtajOblik(pDC, crveniTrougao, 3, ljubicasta, RGB(255, 0, 0), -1, 5, 30, 0);

	// zuti trougao
	POINT zutiTrougao[3] = { Tacka(1, 12), Tacka(1, 17), Tacka(5.5, 17) };
	this->NacrtajOblik(pDC, zutiTrougao, 3, ljubicasta, RGB(255, 255, 0), -1, 7, 20, 0);

	// zeleni trougao
	POINT zeleniTrougao[3] = { Tacka(1, 12), Tacka(5.5, 17), Tacka(10, 12) };
	this->NacrtajOblik(pDC, zeleniTrougao, 3, ljubicasta, RGB(0, 255, 0), -1, 4, 25, 0);

	// roze paralelogram
	POINT rozeParalelogram[4] = { Tacka(10, 12), Tacka(5.5, 17), Tacka(10, 17), Tacka(14.5, 12) };
	this->NacrtajOblik(pDC, rozeParalelogram, 4, ljubicasta, RGB(250, 118, 233), -1, 0, 0, 0);

	// narandzsti trougao
	POINT narandzstiTrougao[3] = { Tacka(14.5, 12), Tacka(10, 17), Tacka(14.5, 17) };
	this->NacrtajOblik(pDC, narandzstiTrougao, 3, ljubicasta, RGB(255, 165, 0), -1, 8, 20, 0);

	// ljubicasti kvadrat
	POINT ljubicastiKvadrat[4] = { Tacka(14.5, 12), Tacka(14.5, 17), Tacka(19, 17), Tacka(19, 12) };
	this->NacrtajOblik(pDC, ljubicastiKvadrat, 4, ljubicasta, RGB(160, 32, 240), -1, 0, 0, 0);
}

void Cvezba7View::SivaPozadina(CDC* pDC, CPen*& oldPen, CBrush*& oldBrush)
{
	CPen newPen(PS_SOLID, 1, RGB(200, 200, 200));
	CBrush newBrush(RGB(230, 230, 230));
	SelektujOlovkuiCetkicu(pDC, newPen, newBrush, oldPen, oldBrush);

	pDC->Rectangle(START_X, START_Y, START_X + SIZE, START_Y + SIZE);

	VratiOlovkuiCetkicu(pDC, oldPen, oldBrush);
}

void Cvezba7View::NacrtajMrezu(CDC* pDC)
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

POINT Cvezba7View::Tacka(float brKockicaX, float brKockicaY)
{
	POINT X;
	X.x = START_X + (int)(KOCKICA * brKockicaX);
	X.y = START_Y + (int)(KOCKICA * brKockicaY);

	return X;
}

POINT Cvezba7View::VratiCentarTrougla(POINT* temena)
{
	double a = this->Razdaljina(temena[1], temena[2]);
	double b = this->Razdaljina(temena[0], temena[2]);
	double c = this->Razdaljina(temena[0], temena[1]);

	POINT Centar = {};
	Centar.x = (temena[0].x * a + temena[1].x * b + temena[2].x * c) / (a + b + c);
	Centar.y = (temena[0].y * a + temena[1].y * b + temena[2].y * c) / (a + b + c);

	return Centar;
}

double Cvezba7View::Razdaljina(POINT A, POINT B)
{
	return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

void Cvezba7View::SelektujOlovkuiCetkicu(CDC* pDC, CPen& pen, CBrush& brush, CPen*& oldPen, CBrush*& oldBrush)
{
	oldPen = pDC->SelectObject(&pen);
	oldBrush = pDC->SelectObject(&brush);
}

void Cvezba7View::VratiOlovkuiCetkicu(CDC* pDC, CPen* oldPen, CBrush* oldBrush)
{
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba7View::NacrtajOblik(CDC* pDC, POINT* temena, int brTemena, COLORREF bojaOlovke, COLORREF bojaCetke, int tipSrafure, int polyN, int polyR, float polyRot)
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

		// sad ovde radimo sa null brush jer do sada sam iscrtavao uvek sa cetkom iste boja kao i spoljasnji oblik 
		// pa je samo izgledalo da je prazno
		CBrush* tempBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		POINT centar = VratiCentarTrougla(temena);
		DrawRegularPolygon(pDC, centar.x, centar.y, polyR, polyN, polyRot);

		pDC->SelectObject(tempBrush);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba7View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
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

void Cvezba7View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		this->mreza = !this->mreza;

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Cvezba7View::OnLButtonDown(UINT nFlags, CPoint point)
{
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
