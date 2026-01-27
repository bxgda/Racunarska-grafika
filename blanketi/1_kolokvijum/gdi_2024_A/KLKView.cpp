
// KLKView.cpp : implementation of the CKLKView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KLK.h"
#endif

#include "KLKDoc.h"
#include "KLKView.h"

#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKLKView

IMPLEMENT_DYNCREATE(CKLKView, CView)

BEGIN_MESSAGE_MAP(CKLKView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKLKView construction/destruction

CKLKView::CKLKView() noexcept
{
	m_imgBackground = new DImage();
	m_imgBackground->Load(CString("images/Background.jpg"));

	m_imgBasket = new DImage();
	m_imgBasket->Load(CString("images/Basket.png"));

	m_imgArm = new DImage();
	m_imgArm->Load(CString("images/Arm.png"));
}

CKLKView::~CKLKView()
{
	delete m_imgArm;
	delete m_imgBasket;
	delete m_imgBackground;
}

BOOL CKLKView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKLKView drawing

void CKLKView::DrawBackground(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);

    int imgW = m_imgBackground->Width();
    int imgH = m_imgBackground->Height();

    int x = (rc.Width() - imgW) / 2;
    int y = rc.Height() - imgH;

    CRect rcImg(0, 0, imgW, imgH);              // ceo image
    CRect rcDC(x, y, x + imgW, y + imgH);       // gde se crta

    m_imgBackground->Draw(pDC, rcImg, rcDC);

	// P.S. ovo crta sliku centrirano horizonalno i zalepljenu za dnu ekrana
	// Moze da bude i jednostavnije, da se samo iscrta, onda je dovoljno:
	// m_imgBackground->Draw(pDC, CRect(0,0,imgW,imgH), rc);
}

void CKLKView::DrawImageTransparent(CDC* pDC, DImage* pImage)
{
	const int imgW = pImage->Width();
	const int imgH = pImage->Height();

	unsigned char* pBits = pImage->GetDIBBits();
	if (!pBits) return;
	
	COLORREF transparent = RGB(pBits[2], pBits[1], pBits[0]);

	CRect rcImg(0, 0, imgW, imgH);
	CRect rcDC(0, 0, imgW, imgH);
	
	pImage->DrawTransparent(pDC, rcImg, rcDC, transparent);
}

void CKLKView::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
{
	XFORM xf = { 1, 0, 0, 1, dx, dy };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKLKView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float rad = ToRad(angle);
	XFORM xf = { cos(rad), sin(rad), -sin(rad), cos(rad), 0, 0 };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKLKView::Scale(CDC* pDC, float sx, float sy, bool rightMultiply)
{
	XFORM xf = { sx, 0, 0, sy, 0, 0 };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKLKView::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM xf = {
		mx ? -1.0f : 1.0f, 0,
		0, my ? -1.0f : 1.0f,
		0, 0
	};
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKLKView::OnDraw(CDC* pDC)
{
	CKLKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&memBmp);

	memDC.FillSolidRect(rcClient, RGB(255, 255, 255));
	
	int oldMode =  memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM oldXf;
	memDC.GetWorldTransform(&oldXf);
	
	DrawScene(&memDC);
	
	memDC.SetWorldTransform(&oldXf);
	memDC.SetGraphicsMode(oldMode);


	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);
}

void CKLKView::DrawScene(CDC* pDC)
{
	DrawBackground(pDC);

	CRect rcClient;
	GetClientRect(&rcClient);

	Translate(pDC, rcClient.Width() / 2, rcClient.Height() - 50);
		
	DrawCarousel(pDC, 182, 50, offset, alpha, beta, angle);
}

void CKLKView::DrawArm(CDC* pDC)
{
	XFORM oldXf{};
	pDC->GetWorldTransform(&oldXf);

	Translate(pDC, -10, -10);

	DrawImageTransparent(pDC, m_imgArm);

	pDC->SetWorldTransform(&oldXf);
}

void CKLKView::DrawBasket(CDC* pDC, int r)
{
	XFORM oldXf{};
	pDC->GetWorldTransform(&oldXf);

	int centarX = m_imgBasket->Width() / 2;
	int centarY = m_imgBasket->Height() / 2;


	Scale(pDC, 0.675, 0.675);
	Translate(pDC, -centarX, -centarY);

	DrawImageTransparent(pDC, m_imgBasket);

	DrawBasketLabel(pDC, r);
	
	pDC->SetWorldTransform(&oldXf);
}

void CKLKView::DrawBasketCouple(CDC* pDC, int l, int r, double angle)
{
	XFORM oldXf{};
	pDC->GetWorldTransform(&oldXf);
	
	Rotate(pDC, angle);

	DrawBasket(pDC, r);

	Rotate(pDC, -angle);

	Translate(pDC, l, 0);

	Rotate(pDC, angle);

	DrawBasket(pDC, r);

	Rotate(pDC, -angle);

	Translate(pDC, -l, 0);

	Rotate(pDC, angle);

	Translate(pDC, 0.8 * 0.675 * (m_imgBasket->Width() / 2), 0);

	Rotate(pDC, -angle);

	Rotate(pDC, -90);

	DrawArm(pDC);
	
	pDC->SetWorldTransform(&oldXf);
}

void CKLKView::DrawPlatform(CDC* pDC, int l, int r, double angle)
{
	XFORM oldXf{};
	pDC->GetWorldTransform(&oldXf);

	Rotate(pDC, angle);

	DrawBasket(pDC, r);

	for (int i = 1; i <= 6; i++)
	{
		Translate(pDC, 0.675 * m_imgBasket->Width(), 0);
		DrawBasketCouple(pDC, l, r, -angle);
		Translate(pDC, 0.675 * -m_imgBasket->Width(), 0);
		Rotate(pDC, 60);
	}

	pDC->SetWorldTransform(&oldXf);
}

void CKLKView::DrawCarousel(CDC* pDC, int h, int r, double offset, double alpha, double beta, double angle)
{
	XFORM oldXf{};
	pDC->GetWorldTransform(&oldXf);

	Translate(pDC, offset, 0);

	float ugaoTocka = (float)((offset / (double)(m_imgBasket->Width() / 2)) * (180 / PI));

	Rotate(pDC, ugaoTocka);

	DrawBasket(pDC, r);

	Rotate(pDC, -ugaoTocka);

	Rotate(pDC, -180 + alpha);

	DrawArm(pDC);

	Translate(pDC, 0, m_imgArm->Height() - 20);

	Rotate(pDC, -beta);

	DrawArm(pDC);

	Translate(pDC, 0, m_imgArm->Height() - 20);

	DrawPlatform(pDC, h, r, angle);

	pDC->SetWorldTransform(&oldXf);
}

void CKLKView::DrawBasketLabel(CDC* pDC, int r)
{
	XFORM oldXf{};
	pDC->GetWorldTransform(&oldXf);

	const int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF oldTextColor = pDC->SetTextColor(RGB(0, 0, 139)); // dark blue

	// DrawBasket radi Scale(0.675), pa kompenzuj da na ekranu bude ~0.9*r
	const double basketScale = 0.675;
	const int fontHeight = (int)lround((0.9 * (double)r) / basketScale);

	CFont font;
	VERIFY(font.CreateFont(
		-fontHeight, // char height
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		_T("Verdana")));

	CFont* pOldFont = pDC->SelectObject(&font);

	const CString text(_T("R_G"));
	const CSize textSize = pDC->GetTextExtent(text);

	const float cx = (float)(m_imgBasket->Width() / 2);
	const float cy = (float)(m_imgBasket->Height() / 2);

	Translate(pDC, cx, cy);
	Rotate(pDC, -30.0f);
	Translate(pDC, (float)(-textSize.cx / 2), (float)(-textSize.cy / 2));

	pDC->TextOut(0, 0, text);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(oldBkMode);
	pDC->SetWorldTransform(&oldXf);
}

BOOL CKLKView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

// CKLKView printing

BOOL CKLKView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKLKView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKLKView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKLKView diagnostics

#ifdef _DEBUG
void CKLKView::AssertValid() const
{
	CView::AssertValid();
}

void CKLKView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKLKDoc* CKLKView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKLKDoc)));
	return (CKLKDoc*)m_pDocument;
}
#endif //_DEBUG


// CKLKView message handlers

void CKLKView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'Q')
	{
		offset -= 5;
	}
	if (nChar == 'W')
	{
		offset += 5;
	}
	if (nChar == 'A')
	{
		alpha += 2;
	}
	if (nChar == 'S')
	{
		alpha -= 2;
	}
	if (nChar == 'D')
	{
		beta += 2;
	}
	if (nChar == 'F')
	{
		beta -= 2;
	}
	if (nChar == 'E')
	{
		angle += 2;
	}
	if (nChar == 'R')
	{
		angle -= 2;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
