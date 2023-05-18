#include "pch.h"
#include "CChart.h"
#define _USE_MATH_DEFINES
#include "math.h"
BEGIN_MESSAGE_MAP(CChart, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

CChart::CChart()
{
	// bluePen = new CPen(PS_SOLID, ARC_WIDTH, RGB(0, 0, 0x7f));
	// redPen = new CPen(PS_SOLID, ARC_WIDTH, RGB(0x7f, 0, 0));
	LOGBRUSH lb;
	lb.lbColor = RGB(0x00, 0x00, 0xff);
	// lb.lbStyle = BS_HATCHED;
	lb.lbStyle = BS_SOLID;
	lb.lbHatch = HS_BDIAGONAL;
	bluePen = new CPen(PS_GEOMETRIC | PS_ENDCAP_FLAT, ARC_WIDTH, &lb, 0, NULL);

	LOGBRUSH lbr;
	lbr.lbColor = RGB(0xff, 0x00, 0x00);
	// lbr.lbStyle = BS_HATCHED;
	lbr.lbStyle = BS_SOLID;
	lbr.lbHatch = HS_BDIAGONAL;
	redPen = new CPen(PS_GEOMETRIC | PS_ENDCAP_FLAT, ARC_WIDTH, &lbr, 0, NULL);

	LOGPEN lp;
	lp.lopnStyle = PS_NULL;
	lp.lopnColor = RGB(0, 0, 0);
	transPen = new CPen();
	transPen->CreatePenIndirect(&lp);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 10;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	wcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("ARIAL"));
	font = new CFont();
	font->CreateFontIndirect(&lf);
}

CChart::~CChart()
{
	delete bluePen;
	delete redPen;
	delete transPen;
	font->DeleteObject();
	delete font;
}

#define TEXT_BOUNDS 250
void CChart::DrawTextAtAngle(CDC* pDC, const CString& text, CPoint position, float angle)
{
#if 0
	int halfBounds = TEXT_BOUNDS >> 1;
	CDC memDC;
	CBitmap memBM;

	memDC.CreateCompatibleDC(pDC);
	memBM.CreateCompatibleBitmap(pDC, TEXT_BOUNDS, TEXT_BOUNDS);
	memDC.SelectObject(memBM);
	memDC.SaveDC();
	memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM xform;
	memDC.GetWorldTransform(&xform);
	xform.eDx -= cos(angle) * TEXT_BOUNDS;
	xform.eDy -= sin(angle) * TEXT_BOUNDS;
	memDC.SetWorldTransform(&xform);
	xform.eM11 = cos(angle);
	xform.eM12 = sin(angle);
	xform.eM21 = sin(angle) * -1;
	xform.eM22 = cos(angle);
	// xform.eDx -= cos(angle) * halfBounds;
	// xform.eDy -= sin(angle) * halfBounds;
	memDC.SetWorldTransform(&xform);
	memDC.SetTextAlign(TA_CENTER | TA_BASELINE);
	memDC.TextOut(halfBounds, halfBounds, text);
	memDC.RestoreDC(-1);
	pDC->BitBlt(position.x-halfBounds, position.y-halfBounds, TEXT_BOUNDS, TEXT_BOUNDS, &memDC, 0, 0, SRCPAINT);
	// pDC->BitBlt(position.x - halfBounds, position.y - halfBounds, TEXT_BOUNDS, TEXT_BOUNDS, &memDC, 0, 0, MERGEPAINT);
	memBM.DeleteObject();
	memDC.DeleteDC();
#endif
	// Save the current state of the device context
	pDC->SaveDC();
	// Set the text angle by applying a transformation to the device context
	pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM xform;
	pDC->GetWorldTransform(&xform);
	xform.eM11 = cos(angle);
	xform.eM12 = sin(angle) * -1;
	xform.eM21 = sin(angle);
	xform.eM22 = cos(angle);
	xform.eDx = position.x;
	xform.eDy = position.y;
	pDC->SetWorldTransform(&xform);

	CFont* oldFont = pDC->SelectObject(font);
	pDC->SetTextColor(RGB(0, 0, 0xff));
	pDC->TextOut(0, 0, text);
	pDC->SelectObject(oldFont);

	pDC->RestoreDC(-1);
}

void CChart::OnPaint()
{
	float x, y;
	CPaintDC dc(this); // device context for painting
	RECT rc;
	GetClientRect(&rc);
	dc.SelectStockObject(BLACK_PEN);
	dc.Rectangle(&rc);

	if (headIndi == NULL)
		return;

	POINT center;
	center.x = rc.right / 2;
	center.y = rc.bottom / 2;
	int radius = rc.right > rc.bottom ? (rc.bottom / 2)  : (rc.right / 2);
	radius -= ARC_WIDTH;
	dc.MoveTo(center.x + radius, center.y);
	dc.SelectObject(bluePen);
	for (y = 0; y < ARC_LAYERS; y++) {
		float angleInc = 360.0 / pow(2, ARC_LAYERS);
		// float curAngle = angleInc * (y+1);
		float curAngle = angleInc * pow(2, y);
		dc.MoveTo(center.x + radius - (y*(ARC_WIDTH+ARC_GAP)), center.y);
		for (x = 0; x < 360; x += curAngle) {
			dc.SelectObject(bluePen);
			dc.AngleArc(center.x, center.y, radius - (y * (ARC_WIDTH + ARC_GAP)), x, curAngle - ARC_ANGLE_GAP);
			dc.SelectObject(transPen);
			dc.AngleArc(center.x, center.y, radius - (y * (ARC_WIDTH + ARC_GAP)), x + curAngle - ARC_ANGLE_GAP, ARC_ANGLE_GAP);
		}
	}

#if 0
	DrawTextAtAngle(&dc, _T("Angled Text - -20"), center, DEG2RAD(-20));
	DrawTextAtAngle(&dc, _T("Angled Text - -10"), center, DEG2RAD(-10));
	DrawTextAtAngle(&dc, _T("Angled Text - 0"), center, DEG2RAD(0));
	DrawTextAtAngle(&dc, _T("Angled Text - 10"), center, DEG2RAD(10));
	DrawTextAtAngle(&dc, _T("Angled Text - 20"), center, DEG2RAD(20));
	DrawTextAtAngle(&dc, _T("Angled Text - 30"), center, DEG2RAD(30));
	DrawTextAtAngle(&dc, _T("Angled Text - 40"), center, DEG2RAD(40));
	DrawTextAtAngle(&dc, _T("Angled Text - 50"), center, DEG2RAD(50));
	DrawTextAtAngle(&dc, _T("Angled Text - 0"), CPoint(10,10), DEG2RAD(0));
	DrawTextAtAngle(&dc, _T("Angled Text - 10"), CPoint(10, 10), DEG2RAD(10));
	DrawTextAtAngle(&dc, _T("Angled Text - 20"), CPoint(10, 10), DEG2RAD(20));
#endif
}

void CChart::SetHeadIndi(CIndi* indi)
{
	headIndi = indi;
}