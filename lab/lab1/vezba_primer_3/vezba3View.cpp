
// vezba3View.cpp : implementation of the Cvezba3View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "vezba3.h"
#endif

#include "vezba3Doc.h"
#include "vezba3View.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define START_X 10
#define START_Y 10
#define KOCKICA 25
#define SIZE 501

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cvezba3View

IMPLEMENT_DYNCREATE(Cvezba3View, CView)

BEGIN_MESSAGE_MAP(Cvezba3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// Cvezba3View construction/destruction

Cvezba3View::Cvezba3View() noexcept
{
	// TODO: add construction code here

}

Cvezba3View::~Cvezba3View()
{
}

BOOL Cvezba3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cvezba3View drawing

void Cvezba3View::OnDraw(CDC* pDC)
{
	NacrtajSveOblike(pDC);

	if (this->mreza)
		NacrtajMrezu(pDC);

	SetFocus();
}


// Cvezba3View printing

BOOL Cvezba3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cvezba3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cvezba3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cvezba3View diagnostics

#ifdef _DEBUG
void Cvezba3View::AssertValid() const
{
	CView::AssertValid();
}

void Cvezba3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cvezba3Doc* Cvezba3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cvezba3Doc)));
	return (Cvezba3Doc*)m_pDocument;
}
#endif //_DEBUG


// Cvezba3View message handlers

void Cvezba3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		this->mreza = !this->mreza;

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Cvezba3View::OnLButtonDown(UINT nFlags, CPoint point)
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

void Cvezba3View::NacrtajSveOblike(CDC* pDC)
{
	CPen* oldPen = nullptr;
	CBrush* oldBrush = nullptr;

	this->SivaPozadina(pDC, oldPen, oldBrush);

	COLORREF cyan = RGB(0, 255, 255);

	// -- Narandzasti trougao
	POINT narandzastiTrougao[3] = { Tacka(1, 10), Tacka(5, 6), Tacka(5, 14) };
	NacrtajOblik(pDC, narandzastiTrougao, 3, cyan, RGB(255, 149, 0), -1, 8, 20, 0);

	// -- Zeleni kvadrat
	CPen olovka(PS_SOLID, 4, cyan);
	CBrush cetka(RGB(0, 255, 0));
	SelektujOlovkuiCetkicu(pDC, olovka, cetka, oldPen, oldBrush);

	POINT ck = Tacka(6.25, 10);
	DrawRegularPolygon(pDC, ck.x, ck.y, 1.25 * KOCKICA * sqrt(2), 4, M_PI_4);

	VratiOlovkuiCetkicu(pDC, oldPen, oldBrush);

	// -- Crveni trougao
	POINT crveniTrougao[3] = { Tacka(7.5, 8.75), Tacka(7.5, 11.25), Tacka(10, 11.25) };
	NacrtajOblik(pDC, crveniTrougao, 3, cyan, RGB(255, 0, 0), -1, 6, 10, 0);

	// -- Srafirani trougao
	POINT srafiraniTrougao[3] = { Tacka(7.5, 8.75), Tacka(10, 11.25), Tacka(12.5, 8.75) };
	NacrtajOblik(pDC, srafiraniTrougao, 3, cyan, RGB(0, 0, 255), HS_CROSS, 4, 15, 0);

	// -- Roze paralelogram
	POINT rozeParalelogram[4] = { Tacka(12.5, 8.75), Tacka(10, 11.25), Tacka(13, 11.25), Tacka(15.25, 8.75) };
	NacrtajOblik(pDC, rozeParalelogram, 4, cyan, RGB(255, 0, 255), -1, 0, 0, 0);

	// -- Ljubicasti trougao
	POINT ljubicastiTrougao[3] = { Tacka(15.25, 8.75), Tacka(13, 11.25), Tacka(15.25, 11.25) };
	NacrtajOblik(pDC, ljubicastiTrougao, 3, cyan, RGB(149, 0, 255), -1, 7, 10, M_PI / 6);

	// zuti trougao
	POINT zutiTrougao[3] = { Tacka(15.25, 6), Tacka(15.25, 14), Tacka(19, 10) };
	NacrtajOblik(pDC, zutiTrougao, 3, cyan, RGB(255, 255, 0), -1, 5, 20, 10);
}

void Cvezba3View::SivaPozadina(CDC* pDC, CPen*& oldPen, CBrush*& oldBrush)
{
	CPen newPen(PS_SOLID, 1, RGB(200, 200, 200));
	CBrush newBrush(RGB(230, 230, 230));
	SelektujOlovkuiCetkicu(pDC, newPen, newBrush, oldPen, oldBrush);

	pDC->Rectangle(START_X, START_Y, START_X + SIZE, START_Y + SIZE);

	VratiOlovkuiCetkicu(pDC, oldPen, oldBrush);
}

void Cvezba3View::NacrtajMrezu(CDC* pDC)
{
	CPen SiviGrid(PS_SOLID, 1, RGB(250, 250, 250));
	CPen* oldPen = pDC->SelectObject(&SiviGrid);

	for (int i = START_X; i < START_X + SIZE; i += KOCKICA) {
		// crtanje vertikalnih linija
		pDC->MoveTo(i, START_Y);
		pDC->LineTo(i, START_Y + SIZE);

		// crtanje horizontalnih linija
		pDC->MoveTo(START_Y, i);
		pDC->LineTo(START_Y + SIZE, i);
	}

	pDC->SelectObject(oldPen);
}

POINT Cvezba3View::Tacka(float brKockicaX, float brKockicaY)
{
	POINT X;
	X.x = START_X + (int)(KOCKICA * brKockicaX);
	X.y = START_Y + (int)(KOCKICA * brKockicaY);

	return X;
}

POINT Cvezba3View::VratiCentarTrougla(POINT* temena)
{
	double a = this->Razdaljina(temena[1], temena[2]);
	double b = this->Razdaljina(temena[0], temena[2]);
	double c = this->Razdaljina(temena[0], temena[1]);

	POINT Centar = {};
	Centar.x = (temena[0].x * a + temena[1].x * b + temena[2].x * c) / (a + b + c);
	Centar.y = (temena[0].y * a + temena[1].y * b + temena[2].y * c) / (a + b + c);

	return Centar;
}

double Cvezba3View::Razdaljina(POINT A, POINT B)
{
	return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

void Cvezba3View::SelektujOlovkuiCetkicu(CDC* pDC, CPen& pen, CBrush& brush, CPen*& oldPen, CBrush*& oldBrush)
{
	oldPen = pDC->SelectObject(&pen);
	oldBrush = pDC->SelectObject(&brush);
}

void Cvezba3View::VratiOlovkuiCetkicu(CDC* pDC, CPen* oldPen, CBrush* oldBrush)
{
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba3View::NacrtajOblik(CDC* pDC, POINT* temena, int brTemena, COLORREF bojaOlovke, COLORREF bojaCetke, int tipSrafure, int polyN, int polyR, float polyRot)
{
	CPen novaOlovka(PS_SOLID, 6, bojaOlovke);

	// 2. Kreiranje četke (Solid ili Hatch zavisno od parametra)
	CBrush novaCetka;
	if (tipSrafure == -1) {
		// Ako je -1, crtamo punu boju
		novaCetka.CreateSolidBrush(bojaCetke);
	}
	else {
		// U suprotnom pravimo srafuru (HS_HORIZONTAL, HS_VERTICAL, itd.)
		novaCetka.CreateHatchBrush(tipSrafure, bojaCetke);
	}

	// 3. Selektovanje
	CPen* oldPen = pDC->SelectObject(&novaOlovka);
	CBrush* oldBrush = pDC->SelectObject(&novaCetka);

	// 4. Crtanje spoljasnjeg oblika
	pDC->Polygon(temena, brTemena);

	// 5. Crtanje unutrašnjeg poligona (samo ako postoji polyN > 0)
	if (polyN > 0 && brTemena == 3)
	{
		// Unutrasnji oblik crtamo istom bojom olovke, ali tanjom (debljina 2)
		CPen tanjaOlovka(PS_SOLID, 2, bojaOlovke);
		pDC->SelectObject(&tanjaOlovka);

		POINT centar = VratiCentarTrougla(temena);
		DrawRegularPolygon(pDC, centar.x, centar.y, polyR, polyN, polyRot);
	}

	// 6. Cleanup
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba3View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	POINT* Points = new POINT[n];

	float alfa = rotAngle; // ugao za koji treba da se rotira

	float factor = 2 * M_PI / (double)n; // kolko svaki ugao ima stepeni unutar figure sa n stranica

	for (int i = 0; i < n; i++)
	{
		// nalazimo koordinate svakog temena
		Points[i].x = (int)(cx + r * cos(alfa));
		Points[i].y = (int)(cy + r * sin(alfa));

		// svako teme ce da bude pomereno za factor pa samim tim povecavamo alfa 
		alfa += factor;
	}

	pDC->Polygon(Points, n);

	delete[] Points;
}


