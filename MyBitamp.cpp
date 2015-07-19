// MyBitamp.cpp: implementation of the CMyBitamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBitamp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBitmap::CMyBitmap()
{
	m_hasBMP=FALSE;

}

CMyBitmap::~CMyBitmap()
{

}

BOOL CMyBitmap::LoadResource(UINT ID)
{
	m_hasBMP=this->LoadBitmap(ID);
	return m_hasBMP;
}

int CMyBitmap::GetWidth() 
{
    BITMAP bm;

	ASSERT(m_hasBMP);
	GetBitmap(&bm);
	return bm.bmWidth;
}

int CMyBitmap::GetHeight() 
{
    BITMAP bm;

	ASSERT(m_hasBMP);
	GetBitmap(&bm);
	return bm.bmHeight;
}


void CMyBitmap::DrawBMP( CDC* pDC, int x, int y, int width, int height ) 
{
    ASSERT(pDC);
	CDC memDC;
    CBitmap *oldMemBitmap;

	memDC.CreateCompatibleDC(pDC);
    oldMemBitmap=memDC.SelectObject(this);
   
    pDC->StretchBlt( x,  y, width,height,&memDC, 0, 0, GetWidth(), GetHeight(), SRCCOPY );
    memDC.SelectObject(oldMemBitmap);
}

void CMyBitmap::DrawBMP( CDC* pDC, int x, int y ) {
	DrawBMP( pDC, x, y, GetWidth(), GetHeight() );
}
