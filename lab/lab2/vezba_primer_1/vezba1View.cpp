
// vezba1View.cpp : implementation of the Cvezba1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "vezba1.h"
#endif

#include "vezba1Doc.h"
#include "vezba1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define START_X 10
#define START_Y 10
#define SIZE 500
#define KOCKICA 25
#define VISINA_KAKTUSA 75	// velcina pojedinacnih delova kaktusa
#define SIRINA_KAKTUSA 50	// velicina pojedinacnih delova kaktusa
#define _USE_MATH_DEFINES
#include <math.h>


// Cvezba1View

IMPLEMENT_DYNCREATE(Cvezba1View, CView)

BEGIN_MESSAGE_MAP(Cvezba1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Cvezba1View construction/destruction

Cvezba1View::Cvezba1View() noexcept
{
	// TODO: add construction code here

}

Cvezba1View::~Cvezba1View()
{
}

BOOL Cvezba1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cvezba1View drawing

void Cvezba1View::OnDraw(CDC* pDC)
{
	Cvezba1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	PlavaPozadina(pDC);

	pDC->SetGraphicsMode(GM_ADVANCED);

	XFORM oldWT;
	pDC->GetWorldTransform(&oldWT);

	Translate(pDC, Kockica(10), Kockica(10), false);
	Rotate(pDC, ugao3, false);

	DrawFigure(pDC);

	POINT tackeVaze1[4] = { { -Kockica(2.1), Kockica(7.8) }, { Kockica(2.1), Kockica(7.8) }, { Kockica(1.6), Kockica(10) }, { -Kockica(1.6), Kockica(10) } };
	NacrtajVazu(pDC, tackeVaze1);

	POINT tackeVaze2[4] = { { -Kockica(2.5), Kockica(7.2) }, { -Kockica(2.5), Kockica(8) }, { Kockica(2.5), Kockica(8) }, { Kockica(2.5), Kockica(7.2) } };
	NacrtajVazu(pDC, tackeVaze2);

	Rotate(pDC, -ugao3, false);

	pDC->SetWorldTransform(&oldWT);

	if (mreza)
		NacrtajMrezu(pDC);

	IspisiTekst(pDC, CString("Kaktusss"), Kockica(19), Kockica(1), -90, -90);
}


// Cvezba1View printing

BOOL Cvezba1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cvezba1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cvezba1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cvezba1View diagnostics

#ifdef _DEBUG
void Cvezba1View::AssertValid() const
{
	CView::AssertValid();
}

void Cvezba1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void Cvezba1View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM transMatrix;
	transMatrix.eM11 = sX;
	transMatrix.eM12 = 0;
	transMatrix.eM21 = 0;
	transMatrix.eM22 = sY;
	transMatrix.eDx = 0;
	transMatrix.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transMatrix, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transMatrix, MWT_LEFTMULTIPLY);
}

void Cvezba1View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM transMatrix;
	transMatrix.eM11 = cos(angle);
	transMatrix.eM12 = sin(angle);
	transMatrix.eM21 = -sin(angle);
	transMatrix.eM22 = cos(angle);
	transMatrix.eDx = 0;
	transMatrix.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transMatrix, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transMatrix, MWT_LEFTMULTIPLY);
}

void Cvezba1View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM transMatrix;
	transMatrix.eM11 = 1;
	transMatrix.eM12 = 0;
	transMatrix.eM21 = 0;
	transMatrix.eM22 = 1;
	transMatrix.eDx = dX;
	transMatrix.eDy = dY;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transMatrix, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transMatrix, MWT_LEFTMULTIPLY);
}

void Cvezba1View::NacrtajCvor(CDC* pDC)
{
	int r = KOCKICA / 2; // poluprecnik

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0)); // crni okvir
	CBrush brush(RGB(0, 204, 0));        // zelena ispuna (kao kaktus)

	CPen* oldPen = pDC->SelectObject(&pen);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Ellipse(-r, -r, r, r);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba1View::NacrtajDeoKaktusa(CDC* pDC, float ugao, float sirina, float visina, bool jeSvetao)
{
	XFORM oldWT;
	pDC->GetWorldTransform(&oldWT);

	this->Rotate(pDC, ugao, false);

	this->Scale(pDC, sirina, visina, false);

	HENHMETAFILE Cactus;
	if (jeSvetao)
		Cactus = GetEnhMetaFile(CString("cactus_part_light.emf"));
	else
		Cactus = GetEnhMetaFile(CString("cactus_part.emf"));

	int definisanaSirina = SIRINA_KAKTUSA;
	int definisanaVisina = VISINA_KAKTUSA;

	CRect rect(-definisanaSirina / 2, -definisanaVisina, definisanaSirina / 2, 0);
	pDC->PlayMetaFile(Cactus, rect);

	DeleteEnhMetaFile(Cactus);
	
	pDC->SetWorldTransform(&oldWT);
}

void Cvezba1View::DrawFigure(CDC* pDC)
{
	XFORM oldWT;
	pDC->GetWorldTransform(&oldWT);

	Translate(pDC, 0, Kockica(7), false);
	Rotate(pDC, ugao1, false);

	NacrtajDeoKaktusa(pDC, 0, 1, 1, true);
	NacrtajCvor(pDC);

	Translate(pDC, 0, -Kockica(3), false);

	NacrtajDeoKaktusa(pDC, 0, 0.35f, 1, false);
	NacrtajDeoKaktusa(pDC, M_PI_4, 0.35f, 1, false);
	NacrtajDeoKaktusa(pDC, -M_PI_4, 0.35f, 1, false);
	NacrtajCvor(pDC);

	Translate(pDC, -Kockica(2.15), -Kockica(2.15), false);

	NacrtajDeoKaktusa(pDC, -M_PI_2, 0.8f, 1, false);
	NacrtajDeoKaktusa(pDC, 0, 0.8f, 1, false);
	NacrtajCvor(pDC);

	Translate(pDC, 0, -Kockica(3), false);

	NacrtajDeoKaktusa(pDC, 0, 1, 1, false);
	NacrtajCvor(pDC);

	Translate(pDC, Kockica(4.3), Kockica(3), false);

	Rotate(pDC, ugao2, false);
	NacrtajDeoKaktusa(pDC, 0, 0.8f, 1, true);
	Rotate(pDC, -ugao2, false);
	
	NacrtajDeoKaktusa(pDC, M_PI_2, 0.8f, 1, false);
	NacrtajCvor(pDC);

	Translate(pDC, Kockica(3), 0, false);

	NacrtajDeoKaktusa(pDC, 3 * M_PI_4, 0.8f, 1, false);
	NacrtajDeoKaktusa(pDC, M_PI_4, 0.8f, 1, false);
	NacrtajCvor(pDC);

	pDC->SetWorldTransform(&oldWT);
}

void Cvezba1View::PlavaPozadina(CDC* pDC)
{
	CPen* oldPen = nullptr;
	CBrush* oldBrush = nullptr;

	CPen* bluePen = new CPen(PS_SOLID, 1, RGB(135, 206, 255));
	CBrush* blueBrush = new CBrush(RGB(135, 206, 255));

	oldPen = pDC->SelectObject(bluePen);
	oldBrush = pDC->SelectObject(blueBrush);

	pDC->Rectangle(0, 0, SIZE, SIZE);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba1View::NacrtajMrezu(CDC* pDC)
{
	CPen SiviGrid(PS_SOLID, 1, RGB(250, 250, 250));

	CPen* oldPen = pDC->SelectObject(&SiviGrid);

	for (int i = 0; i <= SIZE; i += KOCKICA) {
		// crtanje vertikalnih linija
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 0 + SIZE);

		// crtanje horizontalnih linija
		pDC->MoveTo(0, i);
		pDC->LineTo(0 + SIZE, i);
	}

	pDC->SelectObject(oldPen);
}

void Cvezba1View::NacrtajVazu(CDC* pDC, POINT* tacke)
{
	CPen* oldPen = nullptr;
	CBrush* oldBrush = nullptr;

	CPen novaOlovka(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush novaCetka(RGB(230, 111, 2));

	oldPen = pDC->SelectObject(&novaOlovka);
	oldBrush = pDC->SelectObject(&novaCetka);

	pDC->Polygon(tacke, 4);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void Cvezba1View::IspisiTekst(CDC* pDC, CString tekst, int x, int y, int ugaoTeksta, int ugaoKaraktera)
{
	CFont font, * oldFont;
	LOGFONT lf = { 0 };

	lf.lfHeight = (LONG)(1.1 * KOCKICA);	// visina fonta
	lf.lfWidth = 15;                        // sirina fonta
	lf.lfWeight = 500;                      // srednja debljina
	lf.lfEscapement = ugaoTeksta * 10;      // rotacija teksta u desetinkama stepena
	lf.lfOrientation = ugaoKaraktera * 10;  // orijentacija samih karaktera u desetinkama stepena
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy_s(lf.lfFaceName, LF_FACESIZE, L"Arial");

	font.CreateFontIndirectW(&lf);
	oldFont = pDC->SelectObject(&font);

	int oldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldColor = pDC->SetTextColor(RGB(0, 0, 0));

	pDC->TextOutW(x, y, tekst);

	// sencenje
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOutW(x - 2, y - 2, tekst);

	// vracanje starih vrednosti
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(oldMode);
	pDC->SelectObject(oldFont);
}

int Cvezba1View::Kockica(float brKockica)
{
	return (int)(KOCKICA * brKockica);
}

Cvezba1Doc* Cvezba1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cvezba1Doc)));
	return (Cvezba1Doc*)m_pDocument;
}
#endif //_DEBUG


// Cvezba1View message handlers

void Cvezba1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool ponovoCrtanje = false;

	switch (nChar) {
	case 'G':
		mreza = !mreza;
		this->Invalidate();
		break;
	case VK_UP:
		this->ugao2 -= M_PI_4 / 2;
		this->Invalidate();
		break;
	case VK_DOWN:
		this->ugao2 += M_PI_4 / 2;
		this->Invalidate();
		break;
	case VK_LEFT:
		this->ugao1 -= M_PI_4 / 2;
		this->Invalidate();
		break;
	case VK_RIGHT:
		this->ugao1 += M_PI_4 / 2;
		this->Invalidate();
		break;
	case 'A':
	case 'a':
		this->ugao3 += M_PI_2;
		this->Invalidate();
		break;
	case 'D':
	case 'd':
		this->ugao3 -= M_PI_2;
		this->Invalidate();
		break;
	case 'R':
	case 'r':
		this->ugao1 = 0.0f;
		this->ugao2 = 0.0f;
		this->ugao3 = 0.0f;
		this->Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
