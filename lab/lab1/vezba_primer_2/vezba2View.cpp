
// vezba2View.cpp : implementation of the Cvezba2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "vezba2.h"
#endif

#include "vezba2Doc.h"
#include "vezba2View.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define START_X 10
#define START_Y 10
#define KOCKICA 25
#define SIZE 501

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cvezba2View

IMPLEMENT_DYNCREATE(Cvezba2View, CView)

BEGIN_MESSAGE_MAP(Cvezba2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// Cvezba2View construction/destruction

Cvezba2View::Cvezba2View() noexcept
{
	this->mreza = true;
}

Cvezba2View::~Cvezba2View()
{
}

BOOL Cvezba2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cvezba2View drawing

void Cvezba2View::OnDraw(CDC* pDC)
{
	NacrtajSveOblike(pDC);

	if (this->mreza)
		NacrtajMrezu(pDC);

	SetFocus();
}


// Cvezba2View printing

BOOL Cvezba2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cvezba2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cvezba2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cvezba2View diagnostics

#ifdef _DEBUG
void Cvezba2View::AssertValid() const
{
	CView::AssertValid();
}

void Cvezba2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cvezba2Doc* Cvezba2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cvezba2Doc)));
	return (Cvezba2Doc*)m_pDocument;
}
#endif //_DEBUG


// Cvezba2View message handlers

void Cvezba2View::NacrtajSveOblike(CDC* pDC)
{
	CPen* oldPen = nullptr;
	CBrush* oldBrush = nullptr;

	SivaPozadina(pDC, oldPen, oldBrush);

	COLORREF plava = RGB(0, 0, 255);

	// -- Narandzasti trougao gornji
	POINT narandzastiTrougao[3] = { Tacka(1, 1), Tacka(1, 7), Tacka(7, 7) };
	NacrtajOblik(pDC, narandzastiTrougao, 3, plava, RGB(255, 27, 0), -1, 8, 20, 0);

	// -- Ljubicasti trougao
	POINT ljubicastiTrougao[3] = { Tacka(1, 1), Tacka(13, 1), Tacka(13, 13) };
	NacrtajOblik(pDC, ljubicastiTrougao, 3, plava, RGB(143, 0, 255), -1, 6, 40, 0);

	// -- Roze paralelogram
	POINT rozeParalelogram[4] = { Tacka(1, 7), Tacka(1, 13), Tacka(7, 19), Tacka(7, 13) };
	NacrtajOblik(pDC, rozeParalelogram, 4, plava, RGB(255, 197, 254), -1, 0, 0, 0);

	// -- Narandzasti trougao donji
	POINT narandzastiTrougaoDonji[3] = { Tacka(1, 13), Tacka(7, 19), Tacka(1, 19) };
	NacrtajOblik(pDC, narandzastiTrougaoDonji, 3, plava, RGB(255, 182, 57), -1, 4, 20, 0);

	// -- Zeleni trougao
	POINT zeleniTrougao[3] = { Tacka(7, 7), Tacka(13, 13), Tacka(7, 19) };
	NacrtajOblik(pDC, zeleniTrougao, 3, plava, RGB(0, 255, 0), -1, 5, 30, 20);

	// -- Srafirani trougao
	POINT srafiraniTrougao[3] = { Tacka(19, 7), Tacka(19, 19), Tacka(7, 19) };
	NacrtajOblik(pDC, srafiraniTrougao, 3, plava, plava, HS_CROSS, 7, 40, 52);

	// -- Zuti kvadrat
	CPen olovka(PS_SOLID, 4, plava);
	CBrush cetka(RGB(255, 255, 0));
	SelektujOlovkuiCetkicu(pDC, olovka, cetka, oldPen, oldBrush);

	POINT ck = Tacka(16, 4);
	DrawRegularPolygon(pDC, ck.x, ck.y, 3 * KOCKICA * sqrt(2), 4, M_PI_4);

	VratiOlovkuiCetkicu(pDC, oldPen, oldBrush);

}

void Cvezba2View::SivaPozadina(CDC* pDC, CPen*& oldPen, CBrush*& oldBrush)
{
	CPen newPen(PS_SOLID, 1, RGB(200, 200, 200));
	CBrush newBrush(RGB(230, 230, 230));
	SelektujOlovkuiCetkicu(pDC, newPen, newBrush, oldPen, oldBrush);

	pDC->Rectangle(START_X, START_Y, START_X + SIZE, START_Y + SIZE);

	VratiOlovkuiCetkicu(pDC, oldPen, oldBrush);
}

void Cvezba2View::NacrtajMrezu(CDC* pDC)
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

POINT Cvezba2View::Tacka(float brKockicaX, float brKockicaY)
{
	POINT X;
	X.x = START_X + (int)(KOCKICA * brKockicaX);
	X.y = START_Y + (int)(KOCKICA * brKockicaY);

	return X;
}

POINT Cvezba2View::VratiCentarTrougla(POINT* temena)
{
	double a = this->Razdaljina(temena[1], temena[2]);
	double b = this->Razdaljina(temena[0], temena[2]);
	double c = this->Razdaljina(temena[0], temena[1]);

	POINT Centar = {};
	Centar.x = (temena[0].x * a + temena[1].x * b + temena[2].x * c) / (a + b + c);
	Centar.y = (temena[0].y * a + temena[1].y * b + temena[2].y * c) / (a + b + c);

	return Centar;
}

double Cvezba2View::Razdaljina(POINT A, POINT B)
{
	return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

void Cvezba2View::SelektujOlovkuiCetkicu(CDC* pDC, CPen& pen, CBrush& brush, CPen*& oldPen, CBrush*& oldBrush)
{
	oldPen = pDC->SelectObject(&pen);
	oldBrush = pDC->SelectObject(&brush);
}

void Cvezba2View::VratiOlovkuiCetkicu(CDC* pDC, CPen* oldPen, CBrush* oldBrush)
{
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba2View::NacrtajOblik(CDC* pDC, POINT* temena, int brTemena, COLORREF bojaOlovke, COLORREF bojaCetke, int tipSrafure, int polyN, int polyR, float polyRot)
{
	CPen novaOlovka(PS_SOLID, 4, bojaOlovke);

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

void Cvezba2View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
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


void Cvezba2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
		this->mreza = !this->mreza;

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Cvezba2View::OnLButtonDown(UINT nFlags, CPoint point)
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
