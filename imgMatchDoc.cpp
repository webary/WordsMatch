// imgMatchDoc.cpp : implementation of the CImgMatchDoc class
//

#include "stdafx.h"
#include "imgMatch.h"

#include "imgMatchDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImgMatchDoc

IMPLEMENT_DYNCREATE(CImgMatchDoc, CDocument)

BEGIN_MESSAGE_MAP(CImgMatchDoc, CDocument)
	//{{AFX_MSG_MAP(CImgMatchDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgMatchDoc construction/destruction

CImgMatchDoc::CImgMatchDoc()
{
	// TODO: add one-time construction code here

}

CImgMatchDoc::~CImgMatchDoc()
{
}

BOOL CImgMatchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImgMatchDoc serialization

void CImgMatchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImgMatchDoc diagnostics

#ifdef _DEBUG
void CImgMatchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImgMatchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImgMatchDoc commands
