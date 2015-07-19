#pragma once
#include "afxwin.h"
#include "MyBitamp.h"
class AdvancedSet :	public CDialog
{
public:
	AdvancedSet(CWnd* pParent = NULL);
	enum { IDD = IDD_AdvancedSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg int  check_redo(int check,int redo);	//改变窗口背景图片
	afx_msg void OnHelpAbs();	//对偏向性系数的说明
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	//改变窗口背景图片
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);//对静态文本区域重绘，达到透明效果

	DECLARE_MESSAGE_MAP()
public:
	CMyBitmap bg2;	//创建画刷变量
	int waitTime;	//等待时间
	double permitError;	//允许误差
	int EvoCol;		//演化模板列数
	double absolut;	//偏向系数
	static const int ratio_num = 7;//当前使用的特征个数
	double ratio[ratio_num];//各特征所占比例系数
	afx_msg void OnEnChangeRatio();
	double balance_ratio;
	afx_msg void OnBnClickedhelpbalance();
};

