
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

#include "DImage.h"

#define START_X 10
#define START_Y 10
#define KOCKICA 25
#define SIZE 500


// Cvezba1View

IMPLEMENT_DYNCREATE(Cvezba1View, CView)

BEGIN_MESSAGE_MAP(Cvezba1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Cvezba1View construction/destruction

Cvezba1View::Cvezba1View() noexcept
{
	// u konstruktoru ucitavamo slike da se ne bi svaki put ucitavale pri crtanju
	for (int i = 1; i <= 9; i++)
	{
		CString ime;
		ime.Format(_T("%d.bmp"), i);
		m_slike[i].Load(ime);
	}

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
	if (!pDoc) return;

	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);

	CBitmap* slikaMem = new CBitmap();
	slikaMem->CreateCompatibleBitmap(pDC, SIZE + 1, SIZE + 1);
	CBitmap* oldMemBmp = MemDC->SelectObject(slikaMem);

	CBrush brush(RGB(255, 255, 255));
	MemDC->FillRect(CRect(0, 0, SIZE + 1, SIZE + 1), &brush);

	MemDC->SetGraphicsMode(GM_ADVANCED);
	this->NacrtajMrezu(MemDC);

	const float RAD = 3.14159265f / 180.0f;
	float offX = 150.0f;
	float offY = 150.0f;
	
	// gornji red -------------------------------------------------

	// gore levo
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	this->Rotate(MemDC, -56.0f * RAD, true);
	this->Mirror(MemDC, true, false, true);
	this->Translate(MemDC, -37.0f + offX, -50.0f + offY, true);
	this->NacrtajDeoSlagalice(MemDC, 3, 0);

	// gore sredina
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	this->Rotate(MemDC, -163.0f * RAD, true);
	this->Mirror(MemDC, true, false, true);
	this->Translate(MemDC, 114.0f + offX, -36.0f + offY, true);
	this->NacrtajDeoSlagalice(MemDC, 4, 0);

	// gore desno
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	this->Rotate(MemDC, -177.0f * RAD, true);
	this->Mirror(MemDC, false, true, true);
	this->Translate(MemDC, 256.0f + offX, -51.0f + offY, true);
	this->NacrtajDeoSlagalice(MemDC, 5, 1);

	// srednji red ------------------------------------------------

	// sredina levo
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	this->Rotate(MemDC, -199.0f * RAD, true);
	this->Mirror(MemDC, false, true, true);
	this->Translate(MemDC, -43.0f + offX, 99.0f + offY, true);
	this->NacrtajDeoSlagalice(MemDC, 6, 0);

	// sredina sredina
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	this->Rotate(MemDC, 189.0f * RAD, true);
	this->Mirror(MemDC, true, false, true);
	this->Translate(MemDC, 112.0f + offX, 115.0f + offY, true);
	this->NacrtajDeoSlagalice(MemDC, 8, 0);

	// sredina desno
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	this->Rotate(MemDC, 26.0f * RAD, true);
	this->Mirror(MemDC, true, false, true);
	this->Translate(MemDC, 252.0f + offX, 102.0f + offY, true);
	this->NacrtajDeoSlagalice(MemDC, 9, 0);

	// donji red --------------------------------------------------

	// dole levo
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	this->Rotate(MemDC, -25.0f * RAD, true);
	this->Mirror(MemDC, true, false, true);
	this->Translate(MemDC, -42.0f + offX, 250.0f + offY, true);
	this->NacrtajDeoSlagalice(MemDC, 2, 0);

	// dole sredina
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	this->Rotate(MemDC, -2.0f * RAD, true);
	this->Mirror(MemDC, true, false, true);
	this->Translate(MemDC, 105.0f + offX, 250.0f + offY, true);
	this->NacrtajDeoSlagalice(MemDC, 1, 0);

	// dole desno
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	this->Rotate(MemDC, -461.0f * RAD, true);
	this->Mirror(MemDC, false, true, true);
	this->Translate(MemDC, 250.0f + offX, 260.0f + offY, true);
	this->NacrtajDeoSlagalice(MemDC, 7, 0);


	// kopiranje na ekran
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	pDC->BitBlt(START_X, START_Y, SIZE + 1, SIZE + 1, MemDC, 0, 0, SRCCOPY);

	MemDC->SelectObject(oldMemBmp);
	slikaMem->DeleteObject();
	delete slikaMem;
	MemDC->DeleteDC();
	delete MemDC;
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

void Cvezba1View::Scale(CDC* pDC, float sX, float sY, bool desnoMnozenje)
{
	XFORM transMatrix;
	transMatrix.eM11 = sX;
	transMatrix.eM12 = 0;
	transMatrix.eM21 = 0;
	transMatrix.eM22 = sY;
	transMatrix.eDx = 0;
	transMatrix.eDy = 0;

	if (desnoMnozenje)
		pDC->ModifyWorldTransform(&transMatrix, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transMatrix, MWT_LEFTMULTIPLY);
}

void Cvezba1View::Rotate(CDC* pDC, float ugao, bool desnoMnozenje)
{
	XFORM transMatrix;
	transMatrix.eM11 = cos(ugao);
	transMatrix.eM12 = sin(ugao);
	transMatrix.eM21 = -sin(ugao);
	transMatrix.eM22 = cos(ugao);
	transMatrix.eDx = 0;
	transMatrix.eDy = 0;

	if (desnoMnozenje)
		pDC->ModifyWorldTransform(&transMatrix, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transMatrix, MWT_LEFTMULTIPLY);
}

void Cvezba1View::Translate(CDC* pDC, float dX, float dY, bool desnoMnozenje)
{
	XFORM transMatrix;
	transMatrix.eM11 = 1;
	transMatrix.eM12 = 0;
	transMatrix.eM21 = 0;
	transMatrix.eM22 = 1;
	transMatrix.eDx = dX;
	transMatrix.eDy = dY;

	if (desnoMnozenje)
		pDC->ModifyWorldTransform(&transMatrix, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transMatrix, MWT_LEFTMULTIPLY);
}

void Cvezba1View::Mirror(CDC* pDC, bool mx, bool my, bool desnoMnozenje)
{
	XFORM transMatrix;
	transMatrix.eM11 = mx ? (FLOAT)(-1.0) : (FLOAT)(1.0);
	transMatrix.eM12 = (FLOAT)0.0;
	transMatrix.eM21 = (FLOAT)0.0;
	transMatrix.eM22 = my ? (FLOAT)(-1.0) : (FLOAT)(1.0);
	transMatrix.eDx = (FLOAT)0.0;
	transMatrix.eDy = (FLOAT)0.0;

	if (desnoMnozenje)
		pDC->ModifyWorldTransform(&transMatrix, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transMatrix, MWT_LEFTMULTIPLY);
}

void Cvezba1View::NacrtajMrezu(CDC* pDC)
{
	CPen SiviGrid(PS_SOLID, 1, RGB(210, 210, 210));

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

void Cvezba1View::FilterBitaMapa(CBitmap* map, int kanal)
{
	BITMAP bmp;
	map->GetBitmap(&bmp);

	int velicina = bmp.bmHeight * bmp.bmWidthBytes;

	unsigned char* bitovi = new unsigned char[velicina];
	map->GetBitmapBits(velicina, bitovi);

	int novaVrednost;

	for (int i = 0; i <= velicina - 4; i += 4)
	{
		novaVrednost = 64 + (bitovi[i] + bitovi[i + 1] + bitovi[i + 2]) / 3;

		if (novaVrednost > 255)
			novaVrednost = 255;

		switch (kanal)
		{
		case 1: // crvena
			bitovi[i] = 0; // B
			bitovi[i + 1] = 0; // G
			bitovi[i + 2] = novaVrednost;
			break;

		case 2: // zelena
			bitovi[i] = 0; // B
			bitovi[i + 1] = novaVrednost;
			bitovi[i + 2] = 0; // R
			break;

		case 3: // plava
			bitovi[i] = novaVrednost;
			bitovi[i + 1] = 0; // G
			bitovi[i + 2] = 0; // R
			break;

		default:
			bitovi[i] = novaVrednost;
			bitovi[i + 1] = novaVrednost;
			bitovi[i + 2] = novaVrednost;
			break;
		}
	}

	map->SetBitmapBits(velicina, bitovi);
	delete[] bitovi;
}

void Cvezba1View::NacrtajDeoSlagalice(CDC* pDC, int deo, int filter)
{
	DImage& Dslika = m_slike[deo];

	// pravimo 2 bitmape, jedna sa 32bpp (slika koju filtriramo) i jedna sa 1bpp (maska)
	CBitmap* maska = new CBitmap();
	CBitmap* slika = new CBitmap();

	maska->CreateBitmap(Dslika.Width(), Dslika.Height(), 1, 1, NULL);
	slika->CreateBitmap(Dslika.Width(), Dslika.Height(), 1, 32, Dslika.GetDIBBits());

	// pravimo dodatne DC za masku i sliku
	CDC* srcDC = new CDC(); 
	CDC* dstDC = new CDC();

	srcDC->CreateCompatibleDC(pDC); 
	dstDC->CreateCompatibleDC(pDC);

	// selektujemo bitmape u DC
	CBitmap* oldSrcBmp = srcDC->SelectObject(slika);
	CBitmap* oldDstBmp = dstDC->SelectObject(maska);

	// odredjujemo boju pozadine i postavljamo tu boju u srcDC
	COLORREF bgColor = srcDC->GetPixel(0, 0);
	COLORREF oldSrcBgColor = srcDC->SetBkColor(bgColor);

	// pravimo masku: gde je pozadina, vrednost = 0, gde je figura vrednost = 1
	dstDC->BitBlt(0, 0, Dslika.Width(), Dslika.Height(), srcDC, 0, 0, SRCCOPY);

	// pripremamo srcDC za AND operaciju, crna pozadina, bela boja figure
	COLORREF oldSrcFrontColor = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));

	this->FilterBitaMapa(slika, filter);

	// uklanjamo pozadinu slike pomocu maske
	srcDC->BitBlt(0, 0, Dslika.Width(), Dslika.Height(), dstDC, 0, 0, SRCAND);

	// vracamo stare vrednosti i cistimo DC
	srcDC->SetBkColor(oldSrcBgColor);
	srcDC->SetTextColor(oldSrcFrontColor);

	srcDC->SelectObject(oldSrcBmp);
	dstDC->SelectObject(oldDstBmp);

	srcDC->DeleteDC();
	dstDC->DeleteDC();

	delete srcDC;
	delete dstDC;

	// DC za crtanje na glavnom pDC
	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);

	// crtamo masku na pDC da "izrezemo rupu" u pozadini
	CBitmap* oldMemBmp = MemDC->SelectObject(maska);
	pDC->BitBlt((int)(-Dslika.Width() / 2), (int)(-Dslika.Height() / 2), Dslika.Width(), Dslika.Height(), MemDC, 0, 0, SRCAND);

	// crtamo sliku preko maske
	oldMemBmp = MemDC->SelectObject(slika);
	pDC->BitBlt((int)(-Dslika.Width() / 2), (int)(-Dslika.Height() / 2), Dslika.Width(), Dslika.Height(), MemDC, 0, 0, SRCPAINT);

	// vracamo sve i cistimo sve
	MemDC->SelectObject(oldMemBmp);
	MemDC->DeleteDC();

	delete MemDC;

	maska->DeleteObject();
	delete maska;
	slika->DeleteObject();
	delete slika;
}

Cvezba1Doc* Cvezba1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cvezba1Doc)));
	return (Cvezba1Doc*)m_pDocument;
}
#endif //_DEBUG


// Cvezba1View message handlers
