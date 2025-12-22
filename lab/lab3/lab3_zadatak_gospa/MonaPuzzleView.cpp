// MonaPuzzleView.cpp : implementation of the CMonaPuzzleView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MonaPuzzle.h"
#endif

#include "MonaPuzzleDoc.h"
#include "MonaPuzzleView.h"
#include "MonaPuzzleViewHelpers.h"

#include <string>
#include <fstream>
#include <sstream>
#include <cwchar>
#include <cstdlib>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SCREEN_SIZE 500
#define SQUARE_SIZE (SCREEN_SIZE / 20)

// CMonaPuzzleView

IMPLEMENT_DYNCREATE(CMonaPuzzleView, CView)

BEGIN_MESSAGE_MAP(CMonaPuzzleView, CView)
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMonaPuzzleView construction/destruction

void CMonaPuzzleView::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
{
    XFORM xf = { 1, 0, 0, 1, dx, dy };
    pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMonaPuzzleView::Scale(CDC* pDC, float sx, float sy, bool rightMultiply)
{
    XFORM xf = { sx, 0, 0, sy, 0, 0 };
    pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMonaPuzzleView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
    float rad = angle * 3.1415926f / 180.0f;

    XFORM xf;
    xf.eM11 = cos(rad);
    xf.eM12 = sin(rad);
    xf.eM21 = -sin(rad);
    xf.eM22 = cos(rad);
    xf.eDx = 0;
    xf.eDy = 0;

    pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMonaPuzzleView::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
    XFORM xf = {
        mx ? -1.0f : 1.0f, 0,
        0, my ? -1.0f : 1.0f,
        0, 0
    };

    pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

// Filteri
void CMonaPuzzleView::ApplyGrayscaleFilter(PuzzlePiece& piece)
{
    if (!piece.image.isValid())
        return;

    unsigned char* bits = piece.image.GetDIBBits();
    if (!bits)
        return;

    int width = piece.image.Width();
    int height = piece.image.Height();
    int bpp = piece.image.BPP();

    if (bpp != 3 && bpp != 4) // DImage BPP returns bytes per pixel (3 for 24-bit, 4 for 32-bit)
        return;

    int stride = ((width * bpp + 3) / 4) * 4;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int offset = y * stride + x * bpp;

            BYTE B = bits[offset];
            BYTE G = bits[offset + 1];
            BYTE R = bits[offset + 2];

            // Skip transparent color (green: RGB(0, 255, 0))
            if (R == 0 && G == 255 && B == 0)
                continue;

            int gr = 64 + (R + G + B) / 3;
            if (gr > 255) gr = 255;

            bits[offset] = (BYTE)gr;
            bits[offset + 1] = (BYTE)gr;
            bits[offset + 2] = (BYTE)gr;
        }
    }

    piece.image.Update(); // Save posto je menjan objekat direktno
}

void CMonaPuzzleView::ApplyAlternativeFilter(PuzzlePiece& piece)
{
    if (!piece.image.isValid())
        return;

    unsigned char* bits = piece.image.GetDIBBits();
    if (!bits)
        return;

    int width = piece.image.Width();
    int height = piece.image.Height();
    int bpp = piece.image.BPP();

    if (bpp != 3 && bpp != 4)
        return;

    int stride = ((width * bpp + 3) / 4) * 4;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int offset = y * stride + x * bpp;

            BYTE B = bits[offset];
            BYTE G = bits[offset + 1];
            BYTE R = bits[offset + 2];

            // Skip transparent color (green: RGB(0, 255, 0))
            if (R == 0 && G == 255 && B == 0)
                continue;

            bits[offset] = B;
            bits[offset + 1] = 0;
            bits[offset + 2] = 0;
        }
    }

    piece.image.Update();
}

void CMonaPuzzleView::ReapplyFilters(PuzzlePiece& piece)
{
    if (piece.resourcePath.IsEmpty())
        return;

    piece.image.Load(piece.resourcePath);

    if (piece.customFilter)
        ApplyAlternativeFilter(piece);
    else if (piece.grayscale)
        ApplyGrayscaleFilter(piece);
}

// Iscrtavanje pojedinacnog dela slagalice sa svim transformacijama i filterima
void CMonaPuzzleView::DrawPuzzlePiece(CDC* pDC, PuzzlePiece& piece)
{
    if (!piece.image.isValid())
        return;

    int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);
    XFORM oldXForm;
    pDC->GetWorldTransform(&oldXForm);

    // 1. Dobavljanje boje piksela na poziciji (0,0) za transparentnost
    unsigned char* bits = piece.image.GetDIBBits();
    COLORREF transColor = RGB(0, 255, 0); // Default ako nešto nije u redu

    if (bits != nullptr)
    {
        // U DIB formatu raspored je Plava, Zelena, Crvena (BGR)
        BYTE b = bits[0];
        BYTE g = bits[1];
        BYTE r = bits[2];
        transColor = RGB(r, g, b);
    }

    // 2. Transformacije
    Translate(pDC, (float)piece.x, (float)piece.y, false);

    const float cx = piece.image.Width() / 2.0f;
    const float cy = piece.image.Height() / 2.0f;

    if (piece.mirrorX || piece.mirrorY || piece.angle != 0.0f)
    {
        Translate(pDC, cx, cy, false);

        if (piece.mirrorX || piece.mirrorY)
            Mirror(pDC, piece.mirrorX, piece.mirrorY, false);

        if (piece.angle != 0.0f)
            Rotate(pDC, piece.angle, false);

        Translate(pDC, -cx, -cy, false);
    }

    // 3. Iscrtavanje sa dinamičkom bojom transparentnosti
    MonaPuzzleViewHelpers::DrawTransparent(pDC, piece.image, 0, 0, transColor);

    pDC->SetWorldTransform(&oldXForm);
    pDC->SetGraphicsMode(oldMode);
}

// Konstruktor
CMonaPuzzleView::CMonaPuzzleView() noexcept
{
    m_activePieceIndex = 0;

    // Load images
    for (int i = 0; i < 9; i++)
    {
        CString path;
        path.Format(L"res\\%d.bmp", i + 1);

        m_puzzlePieces[i].resourcePath = path;
        m_puzzlePieces[i].image.Load(path);

        m_puzzlePieces[i].angle = 0.0f;
        m_puzzlePieces[i].mirrorX = false;
        m_puzzlePieces[i].mirrorY = false;
        m_puzzlePieces[i].grayscale = true;
        m_puzzlePieces[i].customFilter = false;
    }

    // Default state
    ResetToDefaultState();

    // If there's a saved state, restore it (otherwise keep defaults)
    LoadStateFromFile(m_stateFilePath);
}

CMonaPuzzleView::~CMonaPuzzleView()
{
}

BOOL CMonaPuzzleView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

// CMonaPuzzleView drawing

void CMonaPuzzleView::OnDraw(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);

    // 1. memorijski DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CBitmap bmp;
    bmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
    CBitmap* oldBmp = memDC.SelectObject(&bmp);

    // 2. očisti pozadinu
    memDC.FillSolidRect(&rc, RGB(255, 255, 255));

    // 3. CRTAJ SVE U memDC
    MonaPuzzleViewHelpers::DrawGridLines(&memDC, 10, 10, SCREEN_SIZE, SCREEN_SIZE, SQUARE_SIZE);
    DrawGrid(&memDC);
    DrawOverlayText(&memDC);

    // 4. kopiraj na ekran
    pDC->BitBlt(
        0, 0,
        rc.Width(), rc.Height(),
        &memDC,
        0, 0,
        SRCCOPY
    );

    // cleanup
    memDC.SelectObject(oldBmp);
}

void CMonaPuzzleView::DrawOverlayText(CDC* pDC)
{
    const int baseY = SCREEN_SIZE + SQUARE_SIZE * 2;

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0, 0, 0));

    CFont font;
    font.CreatePointFont(110, L"Segoe UI");
    CFont* oldFont = pDC->SelectObject(&font);

    int idx = m_activePieceIndex;
    if (idx < 0) idx = 0;
    if (idx > 8) idx = 8;

    const PuzzlePiece& p = m_puzzlePieces[idx];

    CString line1;
    line1.Format(L"Active piece: %d | x=%d y=%d angle=%.1f | mirrorX=%d mirrorY=%d | grayscale=%d custom=%d | fast=%d",
        idx + 1,
        p.x, p.y, p.angle,
        p.mirrorX ? 1 : 0,
        p.mirrorY ? 1 : 0,
        p.grayscale ? 1 : 0,
        p.customFilter ? 1 : 0,
        m_fastMoving == true ? 1 : 0);

    CString line2 = L"Keys: 1-9 select | Arrows move | F custom filter | G grayscale | Q/W rotate | E mirrorX | R mirrorY | S fast/slow | P save | O reset | I border";

    pDC->TextOutW(10, baseY, line1);
    pDC->TextOutW(10, baseY + SQUARE_SIZE * 2, line2);

    pDC->SelectObject(oldFont);
}

void CMonaPuzzleView::DrawSelection(CDC* pDC, PuzzlePiece& piece)
{
    if (!piece.image.isValid())
        return;

    CPen pen(PS_SOLID, 3, RGB(255, 0, 0));
    CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
    CPen* oldPen = pDC->SelectObject(&pen);

    pDC->Rectangle(piece.x, piece.y, piece.x + piece.image.Width(), piece.y + piece.image.Height());

    pDC->SelectObject(oldPen);
    pDC->SelectObject(oldBrush);
}

void CMonaPuzzleView::DrawGrid(CDC* pDC)
{
    int idx = m_activePieceIndex;
    if (idx < 0) idx = 0;
    if (idx > 8) idx = 8;

    for (int i = 0; i < 9; i++)
    {
        if (i == idx)
            continue;
        DrawPuzzlePiece(pDC, m_puzzlePieces[i]);
    }

    // Draw active piece last so it's always on top
    DrawPuzzlePiece(pDC, m_puzzlePieces[idx]);

    if (!m_showSelectionBorder)
        return;

    DrawSelection(pDC, m_puzzlePieces[idx]);
}

// CMonaPuzzleView printing

BOOL CMonaPuzzleView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void CMonaPuzzleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void CMonaPuzzleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}


// CMonaPuzzleView diagnostics

#ifdef _DEBUG
void CMonaPuzzleView::AssertValid() const
{
    CView::AssertValid();
}

void CMonaPuzzleView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CMonaPuzzleDoc* CMonaPuzzleView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMonaPuzzleDoc)));
    return (CMonaPuzzleDoc*)m_pDocument;
}
#endif //_DEBUG


// CMonaPuzzleView message handlers

void CMonaPuzzleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar >= '1' && nChar <= '9')
    {
        m_activePieceIndex = (int)(nChar - '1');
        Invalidate(FALSE);
        CView::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }

    if (m_activePieceIndex < 0) m_activePieceIndex = 0;
    if (m_activePieceIndex > 8) m_activePieceIndex = 8;

    PuzzlePiece& p = m_puzzlePieces[m_activePieceIndex];

    const int moveStep = m_fastMoving ? 5 : 1;
    const int rotStep = m_fastMoving ? 10 : 1;

    switch (nChar)
    {
    case VK_LEFT:  p.x -= moveStep; break;
    case VK_RIGHT: p.x += moveStep; break;
    case VK_UP:    p.y -= moveStep; break;
    case VK_DOWN:  p.y += moveStep; break;

    case 'F':
        p.customFilter = !p.customFilter;
        ReapplyFilters(p);
        break;
    case 'G':
        p.grayscale = !p.grayscale;
        if (!p.grayscale) p.customFilter = false;
        ReapplyFilters(p);
        break;

    case 'Q': p.angle -= rotStep; break;
    case 'W': p.angle += rotStep; break;

    case 'E': p.mirrorX = !p.mirrorX; break;
    case 'R': p.mirrorY = !p.mirrorY; break;

    case 'S': m_fastMoving = !m_fastMoving; break;
    case 'P':
        SaveStateToFile(m_stateFilePath);
        break;
    case 'O':
        ResetToDefaultState();
        break;
    case 'I':
        m_showSelectionBorder = !m_showSelectionBorder;
        break;

    default: break;
    }

    Invalidate(FALSE);
    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMonaPuzzleView::ResetToDefaultState()
{
    m_activePieceIndex = 0;

    int tileW = m_puzzlePieces[0].image.isValid() ? m_puzzlePieces[0].image.Width() : 0;
    int tileH = m_puzzlePieces[0].image.isValid() ? m_puzzlePieces[0].image.Height() : 0;

    const int startX = -SQUARE_SIZE;
    const int startY = -SQUARE_SIZE;

    float spacingFactor = 0.65f;
    int stepX = (int)(tileW * spacingFactor);
    int stepY = (int)(tileH * spacingFactor);

    for (int i = 0; i < 9; i++)
    {
        int row = i / 3;
        int col = i % 3;

        m_puzzlePieces[i].x = startX + col * stepX;
        m_puzzlePieces[i].y = startY + row * stepY;

        m_puzzlePieces[i].angle = 0.0f;
        m_puzzlePieces[i].mirrorX = false;
        m_puzzlePieces[i].mirrorY = false;
        m_puzzlePieces[i].grayscale = true;
        m_puzzlePieces[i].customFilter = false;

        ReapplyFilters(m_puzzlePieces[i]);
    }
}

bool CMonaPuzzleView::SaveStateToFile(const CString& path) const
{
    std::wofstream out((LPCWSTR)path);
    if (!out.is_open())
        return false;

    out << L"MonaPuzzleState v1\n";
    out << L"active=" << (m_activePieceIndex + 1) << L"\n";

    for (int i = 0; i < 9; i++)
    {
        const PuzzlePiece& p = m_puzzlePieces[i];
        out << L"piece " << (i + 1)
            << L" x=" << p.x
            << L" y=" << p.y
            << L" angle=" << p.angle
            << L" mirrorX=" << (p.mirrorX ? 1 : 0)
            << L" mirrorY=" << (p.mirrorY ? 1 : 0)
            << L" grayscale=" << (p.grayscale ? 1 : 0)
            << L" custom=" << (p.customFilter ? 1 : 0)
            << L"\n";
    }

    return true;
}

bool CMonaPuzzleView::LoadStateFromFile(const CString& path)
{
    std::wifstream in((LPCWSTR)path);
    if (!in.is_open())
        return false;

    std::wstring header;
    std::getline(in, header);
    if (header.rfind(L"MonaPuzzleState", 0) != 0)
        return false;

    std::wstring line;

    // active=...
    if (std::getline(in, line))
    {
        if (line.rfind(L"active=", 0) == 0)
        {
            int a = _wtoi(line.c_str() + wcslen(L"active="));
            if (a >= 1 && a <= 9)
                m_activePieceIndex = a - 1;
        }
    }

    while (std::getline(in, line))
    {
        if (line.empty())
            continue;

        std::wstringstream ss(line);
        std::wstring word;
        ss >> word;
        if (word != L"piece")
            continue;

        int pieceIdxOneBased = 0;
        ss >> pieceIdxOneBased;
        if (pieceIdxOneBased < 1 || pieceIdxOneBased > 9)
            continue;

        PuzzlePiece& p = m_puzzlePieces[pieceIdxOneBased - 1];

        std::wstring token;
        while (ss >> token)
        {
            (void)(MonaPuzzleViewHelpers::TryReadIntToken(token, L"x", p.x) ||
                MonaPuzzleViewHelpers::TryReadIntToken(token, L"y", p.y) ||
                MonaPuzzleViewHelpers::TryReadFloatToken(token, L"angle", p.angle) ||
                MonaPuzzleViewHelpers::TryReadBoolToken(token, L"mirrorX", p.mirrorX) ||
                MonaPuzzleViewHelpers::TryReadBoolToken(token, L"mirrorY", p.mirrorY) ||
                MonaPuzzleViewHelpers::TryReadBoolToken(token, L"grayscale", p.grayscale) ||
                MonaPuzzleViewHelpers::TryReadBoolToken(token, L"custom", p.customFilter));
        }

        ReapplyFilters(p);
    }

    return true;
}
