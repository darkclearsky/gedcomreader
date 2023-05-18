#pragma once
#include <afxwin.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "GedData.h"

#define ARC_WIDTH 50
#define ARC_GAP 5
#define ARC_ANGLE_GAP (1)
#define ARC_LAYERS (6.0)

#define DEG2RAD(x) (x*(M_PI/180.0))

class CChart : public CStatic
{
public:
    CChart();
    ~CChart();
    void SetHeadIndi(CIndi* indi);

protected:
    void DrawTextAtAngle(CDC* pDC, const CString& text, CPoint position, float angle);
    CPen* redPen, * bluePen, * transPen;
    CFont* font;
    CIndi* headIndi = NULL;

    DECLARE_MESSAGE_MAP()
        afx_msg void OnPaint();
};

