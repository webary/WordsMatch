// imgMatchDoc.h : interface of the CImgMatchDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGMATCHDOC_H__E953EDB3_870A_419B_AE0C_1827D81B5288__INCLUDED_)
#define AFX_IMGMATCHDOC_H__E953EDB3_870A_419B_AE0C_1827D81B5288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImgMatchDoc : public CDocument
{
protected: // create from serialization only
	CImgMatchDoc();
	DECLARE_DYNCREATE(CImgMatchDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgMatchDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImgMatchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImgMatchDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGMATCHDOC_H__E953EDB3_870A_419B_AE0C_1827D81B5288__INCLUDED_)
