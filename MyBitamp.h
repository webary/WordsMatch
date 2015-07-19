// MyBitamp.h: interface for the CMyBitamp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBITAMP_H__DBD504C4_D73B_11D4_907E_D65BA5305A40__INCLUDED_)
#define AFX_MYBITAMP_H__DBD504C4_D73B_11D4_907E_D65BA5305A40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyBitmap : public CBitmap  
{
public:
	BOOL LoadResource(UINT ID);
	void DrawBMP(CDC* pDC, int x, int y, int width, int height);
	void DrawBMP(CDC* pDC, int x=0, int y=0);
	CMyBitmap();
	virtual ~CMyBitmap();
	int	GetWidth();
	int	GetHeight();
private:
	BOOL m_hasBMP;
};

#endif // !defined(AFX_MYBITAMP_H__DBD504C4_D73B_11D4_907E_D65BA5305A40__INCLUDED_)
